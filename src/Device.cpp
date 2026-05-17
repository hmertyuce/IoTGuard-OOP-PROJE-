#include "../include/Device.h"
#include "../include/SecurityMonitor.h"

void Device::reportEvent(const SecurityEvent& event) {
    if (monitor != nullptr) {
        monitor->receiveEvent(event);
    }
}
