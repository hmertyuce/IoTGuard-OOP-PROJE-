#pragma once
#include "Device.h"
#include <iostream>
#include <random>

class Router : public Device {
private:
    int connectedDevices;
    std::string firmwareVersion;

    int getRandom(int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, max);
        return distrib(gen);
    }

public:
    Router(std::string id, std::string ip, std::string fw, SecurityMonitor* m)
        : Device(id, ip, m), firmwareVersion(fw), connectedDevices(5) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] Router " << getDeviceId() << " checking firmware and connections...\n";
        if (getRandom(100) < 5) {
            throw DeviceFailureException("Router ag karti arizasi!", getDeviceId());
        }
        if (firmwareVersion == "v1.0.0") {
            reportEvent(SecurityEvent("EVT-RTR-01", "Outdated firmware detected. Vulnerable to exploits.", EventSeverity::WARNING, getDeviceId()));
        }
    }

    void scanForThreats() override {
        int newConnections = getRandom(5);
        connectedDevices += newConnections;
        
        if (newConnections > 3) {
            reportEvent(SecurityEvent("EVT-RTR-02", "Unusual spike in network connections detected", EventSeverity::HIGH, getDeviceId()));
        } else {
            reportEvent(SecurityEvent("EVT-RTR-03", "Network traffic normal", EventSeverity::INFO, getDeviceId()));
        }
    }
};
