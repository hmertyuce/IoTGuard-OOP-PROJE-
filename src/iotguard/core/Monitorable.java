package iotguard.core;

import iotguard.events.SecurityEvent;

public interface Monitorable {
    void scanForThreats();
    void reportEvent(SecurityEvent event);
}
