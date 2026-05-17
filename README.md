# IoTGuard - Gömülü Cihaz Güvenlik Monitörü

Bu projeyi **Nesne Yönelimli Programlama (OOP)** dersi ödevim için geliştirdim. Projem, bir akıllı ev veya iş yeri ağındaki farklı IoT (Nesnelerin İnterneti) cihazlarını takip eden, bu cihazlara güvenlik taramaları yaptıran ve olası siber tehditleri tespit ederek raporlayan Java tabanlı bir simülasyondur.

## Projede Kullandığım OOP Prensipleri

Projeyi kodlarken Nesne Yönelimli Programlama'nın 4 temel prensibini de aktif olarak kullandım:

1. **Kalıtım (Inheritance):** Sistemdeki cihaz tiplerini (`Camera`, `Router`, `SmartLock`) kodlarken kod tekrarından kaçınmak için ortak özellikleri barındıran temel bir `Device` (Cihaz) üst sınıfı oluşturdum ve diğer cihazları bu sınıftan türettim.
2. **Çok Biçimlilik (Polymorphism):** Merkezi kontrol sistemimde (`SecurityMonitor.java`), farklı türdeki cihazları tek tek ayırmak yerine hepsini `Device` referansıyla bir listede tuttum. Sistemi tarattığımda her cihazın kendi üzerine yazdığı (override ettiği) `scanForThreats()` metodu dinamik olarak çalışıyor.
3. **Soyutlama (Abstraction):** `Device` sınıfını `abstract` olarak tasarladım ve `performSelfDiagnostic()` metodunu soyut bıraktım. Böylece projeye yeni bir cihaz eklemek istediğimde, o cihazın kendi donanım testini mecburen yazmasını sağladım.
4. **Arayüzler (Interfaces):** Cihazların merkeze olay bildirebilmesi için `Monitorable` isimli bir arayüz tanımladım ve cihazların bu sözleşmeye uymasını zorunlu kıldım.

## Proje Yapısı

```
src/
└── iotguard/
    ├── core/
    │   ├── Monitorable.java     # Cihazların uyması gereken arayüz
    │   └── SecurityMonitor.java # Olayları toplayan ve yöneten merkezi sistemim
    ├── devices/
    │   ├── Device.java          # Temel soyut cihaz sınıfım
    │   ├── Camera.java          # Kamera simülasyonu
    │   ├── Router.java          # Yönlendirici (Ağ) simülasyonu
    │   └── SmartLock.java       # Akıllı Kilit simülasyonu
    ├── events/
    │   ├── EventSeverity.java   # Hata/Uyarı düzeyleri (Enum)
    │   └── SecurityEvent.java   # Sisteme düşen logları tuttuğum nesne
    └── Main.java                # Simülasyonu başlattığım ana sınıf
```

## Nasıl Çalıştırılır?

Projemi herhangi bir Java IDE'si ile (IntelliJ IDEA, VS Code vb.) açabilirsiniz. Çalıştırmak için `src/iotguard/Main.java` dosyasını derleyip koşturmanız yeterlidir. Konsolda cihazların teşhisleri, ağ trafiği uyarıları ve "Brute Force" gibi siber saldırı simülasyonlarının canlı loglarını görebilirsiniz.
