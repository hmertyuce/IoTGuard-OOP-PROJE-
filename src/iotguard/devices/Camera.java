package iotguard.devices;

import iotguard.core.SecurityMonitor;
import iotguard.events.EventSeverity;
import iotguard.events.SecurityEvent;

import java.util.Random;

public class Camera extends Device {
    private String resolution;
    private boolean isRecording;

    public Camera(String deviceId, String ipAddress, String resolution, SecurityMonitor monitor) {
        super(deviceId, ipAddress, monitor);
        this.resolution = resolution;
        this.isRecording = true;
    }

    @Override
    public void performSelfDiagnostic() throws iotguard.events.DeviceFailureException {
        System.out.println("[DIAGNOSTIC] Camera " + getDeviceId() + " checking lens and recording status...");
        Random rand = new Random();
        if (rand.nextInt(100) < 5) { // %5 ihtimalle donanım hatası
            throw new iotguard.events.DeviceFailureException("Kamera lensi yanit vermiyor!", getDeviceId());
        }
        if (!isRecording) {
            reportEvent(new SecurityEvent("EVT-CAM-01", "Camera stopped recording unexpectedly", EventSeverity.HIGH, getDeviceId()));
        }
    }

    @Override
    public void scanForThreats() {
        // Simulate scanning for threats like motion in restricted areas or unauthorized access
        Random rand = new Random();
        if (rand.nextInt(10) > 7) {
            reportEvent(new SecurityEvent("EVT-CAM-02", "Unauthorized motion detected in restricted zone", EventSeverity.CRITICAL, getDeviceId()));
        } else {
            reportEvent(new SecurityEvent("EVT-CAM-03", "Routine camera check passed", EventSeverity.INFO, getDeviceId()));
        }
    }
}
