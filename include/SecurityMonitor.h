#pragma once
#include "Device.h"
#include "SecurityEvent.h"
#include "LogManager.h"
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

class SecurityMonitor {
private:
    std::vector<Device*> registeredDevices;
    std::vector<SecurityEvent> eventLog;
    std::mutex monitorMutex;

public:
    SecurityMonitor() {
        LogManager::writeSystemHeader("SYSTEM BOOT - YENI OTURUM");
    }

    void registerDevice(Device* device) {
        registeredDevices.push_back(device);
        std::cout << "Cihaz sisteme eklendi: " << device->getDeviceId() << "\n";
    }

    void receiveEvent(const SecurityEvent& event) {
        std::lock_guard<std::mutex> lock(monitorMutex);
        eventLog.push_back(event);
        LogManager::saveLogToFile(event);
        
        if (event.getSeverity() == EventSeverity::CRITICAL) {
            std::cerr << ">>> ACIL DURUM: " << event.toString() << "\n";
        } else {
            std::cout << "Log Kaydedildi: " << event.toString() << "\n";
        }
    }

    void runFullSystemScan() {
        std::cout << "\n--- Sistem Guvenlik Taramasi Baslatiliyor (Multi-Threaded) ---\n";
        LogManager::writeSystemHeader("SISTEM TARAMASI BASLATILDI");
        
        std::vector<std::thread> threads;

        for (Device* device : registeredDevices) {
            threads.emplace_back([this, device]() {
                try {
                    device->performSelfDiagnostic();
                } catch (const DeviceFailureException& e) {
                    std::lock_guard<std::mutex> lock(monitorMutex);
                    std::cerr << " DONANIM HATASI YAKALANDI (Exception Handling): " << e.what() << "\n";
                }
                device->scanForThreats();
            });
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        
        std::cout << "--- Sistem Taramasi Tamamlandi ---\n\n";
    }

    void displayAllEvents() {
        std::cout << "\n--- Merkezi Olay Gunlugu (" << eventLog.size() << " kayit) ---\n";
        std::cout << "Not: Tum loglar ayni zamanda 'security_logs.txt' dosyasina yazilmistir.\n";
    }
};
