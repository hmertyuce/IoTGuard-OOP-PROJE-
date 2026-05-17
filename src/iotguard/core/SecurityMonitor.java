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
    }

    public void registerDevice(Device device) {
        registeredDevices.add(device);
        System.out.println("Device registered: " + device.getDeviceId());
    }

    public void receiveEvent(SecurityEvent event) {
        eventLog.add(event);
        // Instant response to critical threats
        if (event.getSeverity().toString().equals("CRITICAL")) {
            System.err.println(">>> ALERT TRIGGERED: " + event.toString());
        } else {
            System.out.println("Event Logged: " + event.toString());
        }
    }

    public void runFullSystemScan() {
        System.out.println("\n--- Initiating Full System Security Scan ---");
        for (Device device : registeredDevices) {
            device.performSelfDiagnostic();
            device.scanForThreats();
        }
        System.out.println("--- System Scan Complete ---\n");
    }

    public void displayAllEvents() {
        System.out.println("\n--- Security Event Log (" + eventLog.size() + " events) ---");
        for (SecurityEvent event : eventLog) {
            System.out.println(event.toString());
        }
    }
}
