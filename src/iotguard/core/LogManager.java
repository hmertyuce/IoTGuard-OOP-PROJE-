package iotguard.core;

import iotguard.events.SecurityEvent;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class LogManager {
    private static final String LOG_FILE = "security_logs.txt";

    public static void saveLogToFile(SecurityEvent event) {
        // Try-with-resources (Advanced Java feature for File I/O)
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(LOG_FILE, true))) {
            writer.write(event.toString());
            writer.newLine();
        } catch (IOException e) {
            System.err.println("Log dosyasına yazılamadı: " + e.getMessage());
        }
    }

    public static void writeSystemHeader(String header) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(LOG_FILE, true))) {
            writer.newLine();
            writer.write("=========================================");
            writer.newLine();
            writer.write(header);
            writer.newLine();
            writer.write("=========================================");
            writer.newLine();
        } catch (IOException e) {
            System.err.println("Log dosyasına yazılamadı: " + e.getMessage());
        }
    }
}
