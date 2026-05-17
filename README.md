# IoTGuard - Gömülü Cihaz Güvenlik Monitörü

Bu projeyi **Nesne Yönelimli Programlama (OOP)** dersi ödevim için geliştirdim. Projem, bir akıllı ev veya iş yeri ağındaki farklı IoT (Nesnelerin İnterneti) cihazlarını takip eden, bu cihazlara güvenlik taramaları yaptıran ve olası siber tehditleri tespit ederek raporlayan Java tabanlı bir simülasyondur.

## İleri Düzey Mühendislik Özellikleri (A+)
Bu projeyi standart bir ödevden ayırmak için ileri düzey Java özelliklerini entegre ettim:
1. **Çoklu İş Parçacığı (Multi-threading):** Ağ üzerindeki tüm cihazların taranması sırayla değil, modern sistemlerdeki gibi *aynı anda* (Thread'ler kullanarak asenkron) yapılmaktadır.
2. **Dosya Okuma/Yazma (File I/O):** Loglar sadece konsola yazılmaz. `LogManager` sınıfı üzerinden `security_logs.txt` dosyasına otomatik olarak kaydedilir.
3. **Özel Hata Yönetimi (Custom Exception Handling):** Donanımsal arızaları temsil etmek için kendi özel `DeviceFailureException` sınıfımı yazdım. Çalışma anında bir donanım çökerse, program `try-catch` blokları sayesinde hatayı yakalar ve sistem çökmeden çalışmaya devam eder.

## Projede Kullandığım Temel OOP Prensipleri
1. **Kalıtım (Inheritance):** Sistemdeki cihaz tiplerini (`Camera`, `Router`, `SmartLock`) kodlarken ortak özellikleri barındıran temel bir `Device` (Cihaz) üst sınıfı oluşturdum ve diğer cihazları bu sınıftan türettim.
2. **Çok Biçimlilik (Polymorphism):** Merkezi kontrol sistemimde (`SecurityMonitor.java`), farklı türdeki cihazları tek tek ayırmak yerine hepsini `Device` referansıyla bir listede tuttum.
3. **Soyutlama (Abstraction):** `Device` sınıfını `abstract` olarak tasarladım ve `performSelfDiagnostic()` metodunu soyut bıraktım. 
4. **Arayüzler (Interfaces):** Cihazların merkeze olay bildirebilmesi için `Monitorable` isimli bir arayüz tanımladım.

## Proje Yapısı
```
src/
└── iotguard/
    ├── core/
    │   ├── Monitorable.java     # Arayüz
    │   ├── SecurityMonitor.java # Olayları yöneten (Thread-safe) ana sistem
    │   └── LogManager.java      # Dosyaya log yazan (File I/O) yönetici
    ├── devices/
    │   ├── Device.java          # Soyut cihaz sınıfım
    │   ├── Camera.java          
    │   ├── Router.java          
    │   └── SmartLock.java       
    ├── events/
    │   ├── EventSeverity.java   
    │   ├── SecurityEvent.java   
    │   └── DeviceFailureException.java # Özel hata fırlatıcı sınıfım
    └── Main.java                
```
