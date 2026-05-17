#pragma once
#include "Device.h"
#include <iostream>
#include <random>

class SmartLock : public Device {
private:
    bool isLocked;
    int failedAttempts;

    int getRandom(int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, max);
        return distrib(gen);
    }

public:
    SmartLock(std::string id, std::string ip, SecurityMonitor* m)
        : Device(id, ip, m), isLocked(true), failedAttempts(0) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] SmartLock " << getDeviceId() << " checking mechanical integrity...\n";
        if (getRandom(100) < 5) {
            throw DeviceFailureException("Akill kilit motoru sikisti!", getDeviceId());
        }
        if (!isLocked) {
            reportEvent(SecurityEvent("EVT-LCK-01", "Lock is in open state during security scan", EventSeverity::WARNING, getDeviceId()));
        }
    }

    void scanForThreats() override {
        int attempts = getRandom(5);
        failedAttempts += attempts;

        if (failedAttempts > 3) {
            reportEvent(SecurityEvent("EVT-LCK-02", "Multiple failed unlock attempts detected! Brute force attack suspected.", EventSeverity::CRITICAL, getDeviceId()));
        } else {
            reportEvent(SecurityEvent("EVT-LCK-03", "Lock secure", EventSeverity::INFO, getDeviceId()));
        }
    }
};
