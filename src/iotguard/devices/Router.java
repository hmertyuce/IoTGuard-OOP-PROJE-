package iotguard.devices;

import iotguard.core.SecurityMonitor;
import iotguard.events.EventSeverity;
import iotguard.events.SecurityEvent;

import java.util.Random;

public class Router extends Device {
    private int connectedDevices;
    private String firmwareVersion;

    public Router(String deviceId, String ipAddress, String firmwareVersion, SecurityMonitor monitor) {
        super(deviceId, ipAddress, monitor);
        this.firmwareVersion = firmwareVersion;
        this.connectedDevices = 5;
    }

    @Override
    public void performSelfDiagnostic() throws iotguard.events.DeviceFailureException {
        System.out.println("[DIAGNOSTIC] Router " + getDeviceId() + " checking firmware and connections...");
        Random rand = new Random();
        if (rand.nextInt(100) < 5) {
            throw new iotguard.events.DeviceFailureException("Router ag karti arizasi!", getDeviceId());
        }
        if (firmwareVersion.equals("v1.0.0")) {
            reportEvent(new SecurityEvent("EVT-RTR-01", "Outdated firmware detected. Vulnerable to exploits.", EventSeverity.WARNING, getDeviceId()));
        }
    }

    @Override
    public void scanForThreats() {
        Random rand = new Random();
        int newConnections = rand.nextInt(5);
        this.connectedDevices += newConnections;
        
        if (newConnections > 3) {
            reportEvent(new SecurityEvent("EVT-RTR-02", "Unusual spike in network connections detected", EventSeverity.HIGH, getDeviceId()));
        } else {
             reportEvent(new SecurityEvent("EVT-RTR-03", "Network traffic normal", EventSeverity.INFO, getDeviceId()));
        }
    }
}
