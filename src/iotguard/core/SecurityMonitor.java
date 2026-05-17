package iotguard.core;

import iotguard.devices.Device;
import iotguard.events.SecurityEvent;

import java.util.ArrayList;
import java.util.List;

public class SecurityMonitor {
    private List<Device> registeredDevices;
    private List<SecurityEvent> eventLog;

    public SecurityMonitor() {
        this.registeredDevices = new ArrayList<>();
        this.eventLog = new ArrayList<>();
        LogManager.writeSystemHeader("SYSTEM BOOT - YENI OTURUM");
    }

    public void registerDevice(Device device) {
        registeredDevices.add(device);
        System.out.println("Cihaz sisteme eklendi: " + device.getDeviceId());
    }

    // Thread-safe metot (Senkronizasyon)
    public synchronized void receiveEvent(SecurityEvent event) {
        eventLog.add(event);
        LogManager.saveLogToFile(event); // Dosyaya yazma işlemi (File I/O)
        
        if (event.getSeverity().toString().equals("CRITICAL")) {
            System.err.println(">>> ACIL DURUM: " + event.toString());
        } else {
            System.out.println("Log Kaydedildi: " + event.toString());
        }
    }

    public void runFullSystemScan() {
        System.out.println("\n--- Sistem Guvenlik Taramasi Baslatiliyor (Multi-Threaded) ---");
        LogManager.writeSystemHeader("SISTEM TARAMASI BASLATILDI");
        
        List<Thread> threads = new ArrayList<>();

        for (Device device : registeredDevices) {
            // Coklu is parcacigi (Multi-threading) - Ayni anda tum cihazlar taranir
            Thread t = new Thread(() -> {
                try {
                    device.performSelfDiagnostic();
                } catch (iotguard.events.DeviceFailureException e) {
                    System.err.println(" DONANIM HATASI YAKALANDI (Exception Handling): " + e.getMessage() + " (Cihaz: " + e.getDeviceId() + ")");
                }
                device.scanForThreats();
            });
            threads.add(t);
            t.start();
        }

        // Ana program diger thread'lerin bitmesini bekler
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                System.err.println("Tarama sirasinda is parcacigi hatasi: " + e.getMessage());
            }
        }
        
        System.out.println("--- Sistem Taramasi Tamamlandi ---\n");
    }

    public void displayAllEvents() {
        System.out.println("\n--- Merkezi Olay Gunlugu (" + eventLog.size() + " kayit) ---");
        System.out.println("Not: Tum loglar ayni zamanda 'security_logs.txt' dosyasina yazilmistir.");
    }
}
