package iotguard.events;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class SecurityEvent {
    private String eventId;
    private String description;
    private EventSeverity severity;
    private LocalDateTime timestamp;
    private String deviceId;

    public SecurityEvent(String eventId, String description, EventSeverity severity, String deviceId) {
        this.eventId = eventId;
        this.description = description;
        this.severity = severity;
        this.deviceId = deviceId;
        this.timestamp = LocalDateTime.now();
    }

    public EventSeverity getSeverity() {
        return severity;
    }

    @Override
    public String toString() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        return String.format("[%s] [%s] Device: %s - %s", timestamp.format(formatter), severity, deviceId, description);
    }
}
