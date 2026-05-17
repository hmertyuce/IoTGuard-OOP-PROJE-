#pragma once
#include "Device.h"
#include <iostream>
#include <random>

class Camera : public Device {
private:
    std::string resolution;
    bool isRecording;

    int getRandom(int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, max);
        return distrib(gen);
    }

public:
    Camera(std::string id, std::string ip, std::string res, SecurityMonitor* m)
        : Device(id, ip, m), resolution(res), isRecording(true) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] Camera " << getDeviceId() << " checking lens and recording status...\n";
        if (getRandom(100) < 5) {
            throw DeviceFailureException("Kamera lensi yanit vermiyor!", getDeviceId());
        }
        if (!isRecording) {
            reportEvent(SecurityEvent("EVT-CAM-01", "Camera stopped recording unexpectedly", EventSeverity::HIGH, getDeviceId()));
        }
    }

    void scanForThreats() override {
        if (getRandom(10) > 7) {
            reportEvent(SecurityEvent("EVT-CAM-02", "Unauthorized motion detected in restricted zone", EventSeverity::CRITICAL, getDeviceId()));
        } else {
            reportEvent(SecurityEvent("EVT-CAM-03", "Routine camera check passed", EventSeverity::INFO, getDeviceId()));
        }
    }
};
