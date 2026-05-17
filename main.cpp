/**
 * IoTGuard - Gömülü Cihaz Güvenlik Monitörü (C++ Versiyonu)
 * Yazar: Hayati Mert
 * 
 * Bu proje, Nesne Yönelimli Programlama (OOP) dersi için özel olarak hazırlanmıştır.
 * Karmaşıklığı önlemek amacıyla tüm proje "Tek Dosya (Single-File)" mimarisinde birleştirilmiştir.
 * Kodlar yukarıdan aşağıya; Enumlar -> Arayüzler -> Sınıflar -> Ana Program şeklinde sıralanmıştır.
 */

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <thread>
#include <mutex>
#include <random>
#include <exception>

// =========================================================================
// 1. ÇEKİRDEK YAPILAR (Enums & Exceptions)
// =========================================================================

enum class EventSeverity {
    INFO,
    WARNING,
    HIGH,
    CRITICAL
};

// Özel Hata Sınıfı (Custom Exception Handling)
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

// =========================================================================
// 2. GÜVENLİK OLAYI VE LOG YÖNETİMİ
// =========================================================================

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

// Dosyaya yazdırma işlemlerini yöneten sınıf (File I/O)
class LogManager {
private:
    inline static std::mutex logMutex; // Multi-thread çakışmasını engellemek için Mutex
    inline static const std::string LOG_FILE = "security_logs.txt";
public:
    static void saveLogToFile(const SecurityEvent& event) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) file << event.toString() << "\n";
    }
    static void writeSystemHeader(const std::string& header) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) {
            file << "\n=========================================\n" << header << "\n=========================================\n";
        }
    }
};

// =========================================================================
// 3. OOP ARAYÜZLERİ VE TEMEL SINIFLAR (Interfaces & Abstract Classes)
// =========================================================================

// C++'ta Interface (Saf Sanal Sınıf)
class Monitorable {
public:
    virtual ~Monitorable() = default;
    virtual void scanForThreats() = 0;
    virtual void reportEvent(const SecurityEvent& event) = 0;
};

// İleri bildirim
class SecurityMonitor;

// C++'ta Abstract Class (Soyut Sınıf)
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

    virtual void performSelfDiagnostic() = 0; // Pure Virtual Method
    void reportEvent(const SecurityEvent& event) override; 
};

// =========================================================================
// 4. MERKEZİ KONTROL SİSTEMİ (Polymorphism & Multi-Threading)
// =========================================================================

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
            // Lambda ile her cihaz için yeni bir iş parçacığı oluştur
            threads.emplace_back([this, device]() {
                try {
                    device->performSelfDiagnostic();
                } catch (const DeviceFailureException& e) {
                    std::lock_guard<std::mutex> lock(monitorMutex);
                    std::cerr << " " << e.what() << "\n";
                }
                device->scanForThreats();
            });
        }

        // Tüm iş parçacıklarının bitmesini bekle
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
        
        std::cout << "--- Sistem Taramasi Tamamlandi ---\n\n";
    }

    void displayAllEvents() {
        std::cout << "\n--- Merkezi Olay Gunlugu (" << eventLog.size() << " kayit) ---\n";
        std::cout << "Not: Tum loglar ayni zamanda 'security_logs.txt' dosyasina yazilmistir.\n";
    }
};

// Arayüz metodu tanımlaması (Circular Dependency çözümü)
inline void Device::reportEvent(const SecurityEvent& event) {
    if (monitor != nullptr) {
        monitor->receiveEvent(event);
    }
}

// =========================================================================
// 5. CİHAZ SINIFLARI (Inheritance / Kalıtım)
// =========================================================================

class Camera : public Device {
private:
    std::string resolution;
    bool isRecording;

    int getRandom(int max) {
        std::random_device rd; std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, max);
        return distrib(gen);
    }
public:
    Camera(std::string id, std::string ip, std::string res, SecurityMonitor* m)
        : Device(id, ip, m), resolution(res), isRecording(true) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] Camera " << getDeviceId() << " checking lens and recording status...\n";
        if (getRandom(100) < 5) throw DeviceFailureException("Kamera lensi yanit vermiyor!", getDeviceId());
        if (!isRecording) reportEvent(SecurityEvent("EVT-CAM-01", "Camera stopped recording", EventSeverity::HIGH, getDeviceId()));
    }

    void scanForThreats() override {
        if (getRandom(10) > 7) reportEvent(SecurityEvent("EVT-CAM-02", "Unauthorized motion detected", EventSeverity::CRITICAL, getDeviceId()));
        else reportEvent(SecurityEvent("EVT-CAM-03", "Routine camera check passed", EventSeverity::INFO, getDeviceId()));
    }
};

class Router : public Device {
private:
    std::string firmwareVersion;
public:
    Router(std::string id, std::string ip, std::string fw, SecurityMonitor* m)
        : Device(id, ip, m), firmwareVersion(fw) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] Router " << getDeviceId() << " checking firmware and connections...\n";
        std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distrib(0, 100);
        if (distrib(gen) < 5) throw DeviceFailureException("Router ag karti arizasi!", getDeviceId());
        
        if (firmwareVersion == "v1.0.0") reportEvent(SecurityEvent("EVT-RTR-01", "Outdated firmware detected.", EventSeverity::WARNING, getDeviceId()));
    }

    void scanForThreats() override {
        reportEvent(SecurityEvent("EVT-RTR-03", "Network traffic normal", EventSeverity::INFO, getDeviceId()));
    }
};

class SmartLock : public Device {
private:
    bool isLocked;
public:
    SmartLock(std::string id, std::string ip, SecurityMonitor* m)
        : Device(id, ip, m), isLocked(true) {}

    void performSelfDiagnostic() override {
        std::cout << "[DIAGNOSTIC] SmartLock " << getDeviceId() << " checking mechanical integrity...\n";
        std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distrib(0, 100);
        if (distrib(gen) < 5) throw DeviceFailureException("Akill kilit motoru sikisti!", getDeviceId());
    }

    void scanForThreats() override {
        std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distrib(0, 5);
        if (distrib(gen) > 3) reportEvent(SecurityEvent("EVT-LCK-02", "Multiple failed unlock attempts! Brute force.", EventSeverity::CRITICAL, getDeviceId()));
        else reportEvent(SecurityEvent("EVT-LCK-03", "Lock secure", EventSeverity::INFO, getDeviceId()));
    }
};

// =========================================================================
// 6. ANA PROGRAM (Main Entry)
// =========================================================================

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

    // Konsolun aniden kapanmasını engellemek için eklendi
    system("pause");

    return 0;
}
