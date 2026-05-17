package iotguard;

import iotguard.core.SecurityMonitor;
import iotguard.devices.Camera;
import iotguard.devices.Device;
import iotguard.devices.Router;
import iotguard.devices.SmartLock;

public class Main {
    public static void main(String[] args) {
        System.out.println("=========================================");
        System.out.println("     IoTGuard - Security Monitor        ");
        System.out.println("=========================================\n");

        // 1. Initialize the central monitor
        SecurityMonitor centralMonitor = new SecurityMonitor();

        // 2. Create devices (Demonstrating Polymorphism and Inheritance)
        Device frontCamera = new Camera("CAM-FRONT-01", "192.168.1.10", "1080p", centralMonitor);
        Device mainRouter = new Router("RTR-MAIN-01", "192.168.1.1", "v1.0.0", centralMonitor);
        Device frontDoorLock = new SmartLock("LCK-DOOR-01", "192.168.1.20", centralMonitor);

        // 3. Register devices to the monitor
        centralMonitor.registerDevice(frontCamera);
        centralMonitor.registerDevice(mainRouter);
        centralMonitor.registerDevice(frontDoorLock);

        // 4. Run system scans
        System.out.println("\n[SIMULATION] Starting normal daily scan...");
        centralMonitor.runFullSystemScan();
        
        System.out.println("\n[SIMULATION] Simulating later in the day...");
        centralMonitor.runFullSystemScan();

        // 5. Display complete logs
        centralMonitor.displayAllEvents();
        
        System.out.println("\n=========================================");
        System.out.println("     System Shutdown Successfully        ");
        System.out.println("=========================================");
    }
}
