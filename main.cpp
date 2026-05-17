/**
 * IoTGuard - Gomulu Cihaz Guvenlik Monitoru (C++ Versiyonu)
 * Yazar: Hayati Mert
 * 
 * Bu proje, Nesne Yonelimli Programlama (OOP) dersi icin ozel olarak hazirlanmistir.
 * Karmasikligi onlemek amaciyla tum proje "Tek Dosya (Single-File)" mimarisinde birlestirilmistir.
 * Kodlar yukaridan asagiya; Enumlar -> Arayuzler -> Siniflar -> Ana Program seklinde siralanmistir.
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
#include <conio.h>
#include <windows.h>

// ANSI Renk Kodları (Hacker Teması)
const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";

// Windows'ta ANSI renklerini aktif etme fonksiyonu
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Film Tarzı Yazı Yazma Efekti (Cinematic Typing)
void typeText(const std::string& text, int delay_ms = 20) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

// Yükleme Çubuğu Animasyonu (Progress Bar)
void showProgressBar(int duration_ms, const std::string& taskName) {
    std::cout << YELLOW << " " << taskName << "\n [";
    for (int i = 0; i < 40; ++i) {
        std::cout << " ";
    }
    std::cout << "] 0%\r [";
    
    int step = duration_ms / 40;
    for (int i = 0; i < 40; ++i) {
        std::cout << "=" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(step));
    }
    std::cout << "] 100%\n\n" << RESET;
}

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

class LogManager {
private:
    inline static std::mutex logMutex;
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

class Monitorable {
public:
    virtual ~Monitorable() = default;
    virtual void scanForThreats() = 0;
    virtual void reportEvent(const SecurityEvent& event) = 0;
};

class SecurityMonitor;

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

    virtual void performSelfDiagnostic() = 0;
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
    std::mutex printMutex; // Konsol ciktilarini duzenlemek icin

public:
    SecurityMonitor() {
        LogManager::writeSystemHeader("SYSTEM BOOT - YENI OTURUM");
    }

    void registerDevice(Device* device) {
        registeredDevices.push_back(device);
        std::cout << " Cihaz sisteme eklendi: " << device->getDeviceId() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Cihaz ekleme efekti
    }

    void receiveEvent(const SecurityEvent& event) {
        std::lock_guard<std::mutex> lock(monitorMutex);
        eventLog.push_back(event);
        LogManager::saveLogToFile(event);
        
        if (event.getSeverity() == EventSeverity::CRITICAL) {
            std::cerr << RED << " >>> ACIL DURUM: " << event.toString() << RESET << "\n";
        } else if (event.getSeverity() == EventSeverity::WARNING) {
            std::cout << YELLOW << " Log Kaydedildi: " << event.toString() << RESET << "\n";
        } else {
            std::cout << " Log Kaydedildi: " << event.toString() << "\n";
        }
    }

    void runFullSystemScan() {
        showProgressBar(2000, "Derin Sistem Analizi Baslatiliyor...");
        
        std::vector<std::thread> threads;

        for (Device* device : registeredDevices) {
            threads.emplace_back([this, device]() {
                try {
                    device->performSelfDiagnostic();
                } catch (const DeviceFailureException& e) {
                    std::lock_guard<std::mutex> lock(monitorMutex);
                    std::cerr << RED << " " << e.what() << RESET << "\n";
                }
                device->scanForThreats();
            });
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }

    void displayAllEvents() {
        std::cout << CYAN << "\n --- Merkezi Olay Gunlugu (" << eventLog.size() << " kayit) ---\n\n" << RESET;
        if(eventLog.empty()) {
            std::cout << " Henuz kaydedilmis bir olay bulunmamaktadir.\n";
        } else {
            for(const auto& ev : eventLog) {
                if (ev.getSeverity() == EventSeverity::CRITICAL) {
                    std::cout << RED << " " << ev.toString() << RESET << "\n";
                } else if (ev.getSeverity() == EventSeverity::WARNING) {
                    std::cout << YELLOW << " " << ev.toString() << RESET << "\n";
                } else {
                    std::cout << GREEN << " " << ev.toString() << RESET << "\n";
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Log okuma efekti
            }
        }
        std::cout << "\n Not: Tum loglar 'security_logs.txt' dosyasina da yazilmistir.\n";
    }
};

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
        std::string msg = " [DIAGNOSTIC] Camera " + getDeviceId() + " checking lens...\n";
        std::cout << msg;
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
        std::string msg = " [DIAGNOSTIC] Router " + getDeviceId() + " checking firmware...\n";
        std::cout << msg;
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
        std::string msg = " [DIAGNOSTIC] SmartLock " + getDeviceId() + " checking mechanical integrity...\n";
        std::cout << msg;
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
// 6. ANA PROGRAM (INTERAKTIF KONSOL ARAYÜZÜ - TUI)
// =========================================================================

void printASCII() {
    std::cout << CYAN;
    std::cout << "  ____   _______ _____                     _ \n";
    std::cout << " |_   _|/ |__   |  __ \\                   | |\n";
    std::cout << "   | | ___   | || |  \\/ _   _  __ _ _ __  | |_\n";
    std::cout << "   | |/ _ \\  | || | __ | | | |/ _` | '__| |  _|\n";
    std::cout << "   | | (_) | | || |_\\ \\| |_| | (_| | |    | |_\n";
    std::cout << "   \\_/\\___/  \\_/ \\____/ \\__,_|\\__,_|_|     \\__|\n";
    std::cout << RESET;
    std::cout << "====================================================\n";
    std::cout << "   Gomulu Cihaz Guvenlik Monitoru [C++ Editon]\n";
    std::cout << "====================================================\n\n";
}

bool adminLogin() {
    std::string user;
    std::string pass;
    
    while(true) {
        system("cls");
        std::cout << CYAN << "====================================================\n";
        std::cout << "          IOTGUARD YONETICI GIRIS PANELI\n";
        std::cout << "====================================================\n" << RESET;
        std::cout << "\n Lutfen Sisteme Giris Yapin:\n";
        std::cout << " (Not: Varsayilan admin / 1234)\n\n";
        
        typeText(" Kullanici Adi: ", 10);
        std::cin >> user;
        
        typeText(" Sifre: ", 10);
        pass = "";
        char ch;
        while((ch = _getch()) != '\r') { // Enter'a basana kadar oku
            if (ch == '\b') { // Backspace
                if(pass.length() > 0) {
                    std::cout << "\b \b";
                    pass.pop_back();
                }
            } else {
                pass.push_back(ch);
                std::cout << "*";
            }
        }
        std::cout << "\n";

        if (user == "admin" && pass == "1234") {
            std::cout << GREEN;
            typeText("\n [BASARILI] Kimlik dogrulandi. Sisteme baglaniliyor...\n", 30);
            std::cout << RESET;
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            return true;
        } else {
            std::cout << RED;
            typeText("\n [HATA] Kullanici adi veya sifre hatali!\n", 30);
            std::cout << RESET;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    }
    return false;
}

int main() {
    enableANSI();
    
    // Giris paneli
    if (!adminLogin()) {
        return 0;
    }

    SecurityMonitor centralMonitor;

    Camera frontCamera("CAM-FRONT-01", "192.168.1.10", "1080p", &centralMonitor);
    Router mainRouter("RTR-MAIN-01", "192.168.1.1", "v1.0.0", &centralMonitor);
    SmartLock frontDoorLock("LCK-DOOR-01", "192.168.1.20", &centralMonitor);

    system("cls");
    printASCII();
    
    showProgressBar(1500, "Cihaz Modulleri Sisteme Entegre Ediliyor");
    
    centralMonitor.registerDevice(&frontCamera);
    centralMonitor.registerDevice(&mainRouter);
    centralMonitor.registerDevice(&frontDoorLock);
    
    std::cout << "\n Yukleme tamamlandi. Devam etmek icin bir tusa basin...";
    _getch();

    bool isRunning = true;

    while (isRunning) {
        system("cls");
        printASCII();
        std::cout << GREEN << " [1] " << RESET << "Sistemi Tara (Multi-Threaded)\n";
        std::cout << GREEN << " [2] " << RESET << "Cihaz Loglarini Goster\n";
        std::cout << GREEN << " [3] " << RESET << "Gorsel Simulasyon (Matrix Modu)\n";
        std::cout << RED   << " [4] " << RESET << "Sistemi Kapat (Cikis)\n";
        std::cout << "\n Lutfen bir islem secin (1-4): ";

        char choice = _getch();

        switch (choice) {
            case '1': {
                system("cls");
                printASCII();
                std::cout << YELLOW << "\n [UYARI] Tum cihazlar eszamanli (Multi-Thread) taranmaya basliyor...\n\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                
                centralMonitor.runFullSystemScan();
                
                std::cout << CYAN << "\n Islem tamamlandi. Ana menuye donmek icin herhangi bir tusa basin..." << RESET;
                _getch();
                break;
            }
            case '2': {
                system("cls");
                printASCII();
                centralMonitor.displayAllEvents();
                std::cout << CYAN << "\n Ana menuye donmek icin herhangi bir tusa basin..." << RESET;
                _getch();
                break;
            }
            case '3': {
                system("cls");
                std::cout << GREEN;
                std::string chars = "01001100101010100101010010101011110010";
                std::random_device rd; std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(0, chars.length() - 1);
                
                for(int i=0; i<400; i++) {
                    for(int j=0; j<8; j++) {
                        std::cout << chars[distrib(gen)] << " ";
                    }
                    std::cout << "   ";
                    if (i % 3 == 0) std::cout << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(8));
                }
                std::cout << RESET;
                std::cout << "\n\n Simulasyon tamamlandi. Geri donmek icin tusa basin...";
                _getch();
                break;
            }
            case '4': {
                isRunning = false;
                std::cout << RED;
                typeText("\n\n Sistem Kapatiliyor... Gorusmek Uzere!\n", 40);
                std::cout << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(600));
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
