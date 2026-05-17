package iotguard.devices;

import iotguard.core.Monitorable;
import iotguard.events.SecurityEvent;
import iotguard.core.SecurityMonitor;

public abstract class Device implements Monitorable {
    private String deviceId;
    private String ipAddress;
    private String status;
    private SecurityMonitor monitor;

    public Device(String deviceId, String ipAddress, SecurityMonitor monitor) {
        this.deviceId = deviceId;
        this.ipAddress = ipAddress;
        this.status = "ONLINE";
        this.monitor = monitor;
    }

    public String getDeviceId() {
        return deviceId;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    // Abstract method to be implemented by all specific devices, throw exception if hardware fails
    public abstract void performSelfDiagnostic() throws iotguard.events.DeviceFailureException;

    @Override
    public void reportEvent(SecurityEvent event) {
        if (monitor != null) {
            monitor.receiveEvent(event);
        }
    }
    
    @Override
    public String toString() {
        return "Device ID: " + deviceId + " | IP: " + ipAddress + " | Status: " + status;
    }
}
