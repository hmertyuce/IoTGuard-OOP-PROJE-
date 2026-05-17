# IoTGuard - Gömülü Cihaz Güvenlik Monitörü (C++ Versiyonu)

Bu projeyi **Nesne Yönelimli Programlama (OOP)** dersi ödevim için geliştirdim. Projem, bir akıllı ev veya iş yeri ağındaki farklı IoT cihazlarını takip eden, bu cihazlara güvenlik taramaları yaptıran ve olası siber tehditleri tespit ederek raporlayan **C++** tabanlı bir simülasyondur.

## İleri Düzey Mühendislik Özellikleri (A+)
Bu projeyi standart bir ödevden ayırmak için C++11/C++17 standartlarını entegre ettim:
1. **Çoklu İş Parçacığı (Multi-threading):** Ağ üzerindeki tüm cihazların taranması `<thread>` kütüphanesi ile *aynı anda* yapılmaktadır. `std::mutex` ve `std::lock_guard` kullanılarak veri yarışması (data race) önlenmiştir.
2. **Dosya Okuma/Yazma (File I/O):** Loglar `security_logs.txt` dosyasına `<fstream>` kütüphanesi kullanılarak kalıcı olarak kaydedilir.
3. **Özel Hata Yönetimi (Custom Exception Handling):** Donanımsal arızaları temsil etmek için `std::exception` sınıfından türeyen kendi `DeviceFailureException` sınıfımı yazdım.
4. **Header / Source Ayrımı:** Profesyonel C++ standartlarına uygun olarak sınıfların yapısı `.h` dosyalarında, uygulanması `.cpp` dosyalarında tutulmuştur.

## Projede Kullandığım Temel OOP Prensipleri
1. **Kalıtım (Inheritance):** `Camera`, `Router`, `SmartLock` sınıflarını `Device` (Cihaz) üst sınıfından türettim.
2. **Çok Biçimlilik (Polymorphism):** Merkezi kontrol sistemimde (`SecurityMonitor.h`), farklı türdeki cihazları `std::vector<Device*>` içinde tuttum. Sanal (virtual) metotlar ile dinamik dispatch sağladım.
3. **Soyutlama (Abstraction):** `Device` sınıfını içindeki `virtual void performSelfDiagnostic() = 0;` saf sanal fonksiyonu (pure virtual) ile soyut sınıfa (abstract class) dönüştürdüm.
4. **Arayüzler (Interfaces):** `Monitorable` sınıfı tamamen saf sanal fonksiyonlardan oluşur ve Java'daki Interface mantığını C++'a taşır.

## Nasıl Derlenir ve Çalıştırılır?
Windows üzerinde MingW (g++) yüklüyse, klasörün içindeki `build.bat` dosyasına çift tıklayarak otomatik derleyip çalıştırabilirsiniz.

Alternatif Komut:
```bash
g++ -std=c++17 src/main.cpp src/Device.cpp -o IoTGuard.exe
./IoTGuard.exe
```
