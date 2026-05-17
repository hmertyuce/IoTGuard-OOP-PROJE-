#pragma once
#include "Monitorable.h"
#include "SecurityEvent.h"
#include "DeviceFailureException.h"
#include <string>

class SecurityMonitor; // forward declaration

class Device : public Monitorable {
private:
    std::string deviceId;
    std::string ipAddress;
    std::string status;
    SecurityMonitor* monitor;

public:
    Device(std::string id, std::string ip, SecurityMonitor* m)
        : deviceId(id), ipAddress(ip), status("ONLINE"), monitor(m) {}

    virtual ~Device() = default;

    std::string getDeviceId() const { return deviceId; }
    std::string getIpAddress() const { return ipAddress; }
    std::string getStatus() const { return status; }
    void setStatus(std::string s) { status = s; }

    virtual void performSelfDiagnostic() = 0;

    void reportEvent(const SecurityEvent& event) override; // Implemented in Device.cpp to avoid circular deps
};
