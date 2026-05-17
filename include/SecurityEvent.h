#pragma once
#include <string>
#include <ctime>
#include "EventSeverity.h"

class SecurityEvent {
private:
    std::string eventId;
    std::string description;
    EventSeverity severity;
    std::string deviceId;
    std::string timestamp;

    std::string currentDateTime() const {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }

public:
    SecurityEvent(std::string id, std::string desc, EventSeverity sev, std::string devId)
        : eventId(id), description(desc), severity(sev), deviceId(devId) {
        timestamp = currentDateTime();
    }

    EventSeverity getSeverity() const { return severity; }

    std::string toString() const {
        std::string sevStr;
        switch(severity) {
            case EventSeverity::INFO: sevStr = "INFO"; break;
            case EventSeverity::WARNING: sevStr = "WARNING"; break;
            case EventSeverity::HIGH: sevStr = "HIGH"; break;
            case EventSeverity::CRITICAL: sevStr = "CRITICAL"; break;
        }
        return "[" + timestamp + "] [" + sevStr + "] Device: " + deviceId + " - " + description;
    }
};
