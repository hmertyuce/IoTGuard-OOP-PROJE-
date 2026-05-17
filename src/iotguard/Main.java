package iotguard;

import iotguard.core.SecurityMonitor;
import iotguard.devices.Camera;
import iotguard.devices.Device;
import iotguard.devices.Router;
import iotguard.devices.SmartLock;

import javax.swing.*;
import java.awt.*;
import java.io.OutputStream;
import java.io.PrintStream;

public class Main extends JFrame {

    private SecurityMonitor centralMonitor;
    private JTextArea logArea;
    private JButton btnScan;
    private JButton btnClear;

    public Main() {
        super("IoTGuard - Güvenlik Monitörü (A+ Versiyon)");

        centralMonitor = new SecurityMonitor();
        Device frontCamera = new Camera("CAM-FRONT-01", "192.168.1.10", "1080p", centralMonitor);
        Device mainRouter = new Router("RTR-MAIN-01", "192.168.1.1", "v1.0.0", centralMonitor);
        Device frontDoorLock = new SmartLock("LCK-DOOR-01", "192.168.1.20", centralMonitor);

        centralMonitor.registerDevice(frontCamera);
        centralMonitor.registerDevice(mainRouter);
        centralMonitor.registerDevice(frontDoorLock);

        setSize(850, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());

        JPanel topPanel = new JPanel();
        topPanel.setBackground(new Color(30, 39, 46));
        topPanel.setPreferredSize(new Dimension(800, 60));
        topPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 20, 10));
        
        btnScan = new JButton("SİSTEMİ TARA (MULTI-THREAD)");
        btnScan.setFont(new Font("Arial", Font.BOLD, 14));
        btnScan.setBackground(new Color(11, 232, 129));
        btnScan.setForeground(Color.BLACK);
        btnScan.setFocusPainted(false);
        btnScan.setCursor(new Cursor(Cursor.HAND_CURSOR));
        
        btnClear = new JButton("EKRANI TEMİZLE");
        btnClear.setFont(new Font("Arial", Font.BOLD, 14));
        btnClear.setBackground(new Color(255, 94, 87));
        btnClear.setForeground(Color.WHITE);
        btnClear.setFocusPainted(false);
        btnClear.setCursor(new Cursor(Cursor.HAND_CURSOR));

        topPanel.add(btnScan);
        topPanel.add(btnClear);
        add(topPanel, BorderLayout.NORTH);

        logArea = new JTextArea();
        logArea.setEditable(false);
        logArea.setBackground(new Color(45, 52, 54));
        logArea.setForeground(new Color(0, 210, 211));
        logArea.setFont(new Font("Consolas", Font.PLAIN, 15));
        logArea.setMargin(new Insets(10, 10, 10, 10));
        
        JScrollPane scrollPane = new JScrollPane(logArea);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        add(scrollPane, BorderLayout.CENTER);

        redirectSystemStreams();

        System.out.println("================================================");
        System.out.println("       IoTGuard SISTEMI BASARIYLA KURULDU       ");
        System.out.println("================================================");
        System.out.println("Aramaya baslamak icin yukaridaki 'SISTEMI TARA' butonuna tiklayin.\n");

        btnScan.addActionListener(e -> startScan());
        btnClear.addActionListener(e -> logArea.setText(""));
    }

    private void startScan() {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                btnScan.setEnabled(false);
                btnScan.setText("TARANIYOR...");
                centralMonitor.runFullSystemScan();
                btnScan.setText("SİSTEMİ TARA (MULTI-THREAD)");
                btnScan.setEnabled(true);
            }
        });
        t.start();
    }

    private void updateTextArea(String text) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                logArea.append(text);
                logArea.setCaretPosition(logArea.getDocument().getLength());
            }
        });
    }

    private void redirectSystemStreams() {
        OutputStream out = new OutputStream() {
            @Override
            public void write(int b) {
                updateTextArea(String.valueOf((char) b));
            }
            @Override
            public void write(byte[] b, int off, int len) {
                updateTextArea(new String(b, off, len));
            }
            @Override
            public void write(byte[] b) {
                write(b, 0, b.length);
            }
        };
        System.setOut(new PrintStream(out, true));
        System.setErr(new PrintStream(out, true));
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                Main app = new Main();
                app.setVisible(true);
            }
        });
    }
}
