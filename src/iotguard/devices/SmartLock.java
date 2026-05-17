package iotguard.devices;

import iotguard.core.SecurityMonitor;
import iotguard.events.EventSeverity;
import iotguard.events.SecurityEvent;

import java.util.Random;

public class SmartLock extends Device {
    private boolean isLocked;
    private int failedAttempts;

    public SmartLock(String deviceId, String ipAddress, SecurityMonitor monitor) {
        super(deviceId, ipAddress, monitor);
        this.isLocked = true;
        this.failedAttempts = 0;
    }

    @Override
    public void performSelfDiagnostic() {
        System.out.println("[DIAGNOSTIC] SmartLock " + getDeviceId() + " checking mechanical integrity...");
        if (!isLocked) {
            reportEvent(new SecurityEvent("EVT-LCK-01", "Lock is in open state during security scan", EventSeverity.WARNING, getDeviceId()));
        }
    }

    @Override
    public void scanForThreats() {
        Random rand = new Random();
        int attempts = rand.nextInt(5);
        this.failedAttempts += attempts;

        if (this.failedAttempts > 3) {
            reportEvent(new SecurityEvent("EVT-LCK-02", "Multiple failed unlock attempts detected! Brute force attack suspected.", EventSeverity.CRITICAL, getDeviceId()));
        } else {
             reportEvent(new SecurityEvent("EVT-LCK-03", "Lock secure", EventSeverity.INFO, getDeviceId()));
        }
    }
}
