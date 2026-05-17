#pragma once
class SecurityEvent;

class Monitorable {
public:
    virtual ~Monitorable() = default;
    virtual void scanForThreats() = 0;
    virtual void reportEvent(const SecurityEvent& event) = 0;
};
