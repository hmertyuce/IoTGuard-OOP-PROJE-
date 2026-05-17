#define LOG_MANAGER_IMPL
#include "../include/SecurityMonitor.h"
#include "../include/Camera.h"
#include "../include/Router.h"
#include "../include/SmartLock.h"
#include <iostream>

int main() {
    std::cout << "=========================================\n";
    std::cout << "     IoTGuard - Security Monitor        \n";
    std::cout << "=========================================\n\n";

    SecurityMonitor centralMonitor;

    Camera frontCamera("CAM-FRONT-01", "192.168.1.10", "1080p", &centralMonitor);
    Router mainRouter("RTR-MAIN-01", "192.168.1.1", "v1.0.0", &centralMonitor);
    SmartLock frontDoorLock("LCK-DOOR-01", "192.168.1.20", &centralMonitor);

    centralMonitor.registerDevice(&frontCamera);
    centralMonitor.registerDevice(&mainRouter);
    centralMonitor.registerDevice(&frontDoorLock);

    std::cout << "\n[SIMULATION] Starting normal daily scan...\n";
    centralMonitor.runFullSystemScan();
    
    std::cout << "\n[SIMULATION] Simulating later in the day...\n";
    centralMonitor.runFullSystemScan();

    centralMonitor.displayAllEvents();
    
    std::cout << "\n=========================================\n";
    std::cout << "     System Shutdown Successfully        \n";
    std::cout << "=========================================\n";

    return 0;
}
