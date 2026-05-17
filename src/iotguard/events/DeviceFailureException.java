package iotguard.events;

public class DeviceFailureException extends Exception {
    private String deviceId;

    public DeviceFailureException(String message, String deviceId) {
        super(message);
        this.deviceId = deviceId;
    }

    public String getDeviceId() {
        return deviceId;
    }
}
