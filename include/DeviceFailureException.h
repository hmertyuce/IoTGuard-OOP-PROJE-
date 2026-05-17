#pragma once
#include <exception>
#include <string>

class DeviceFailureException : public std::exception {
private:
    std::string fullMsg;
    std::string deviceId;

public:
    DeviceFailureException(const std::string& msg, const std::string& devId) : deviceId(devId) {
        fullMsg = "DONANIM HATASI: " + msg + " (Cihaz: " + deviceId + ")";
    }

    const char* what() const noexcept override {
        return fullMsg.c_str();
    }

    std::string getDeviceId() const { return deviceId; }
};
