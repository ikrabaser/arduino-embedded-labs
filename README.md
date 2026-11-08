# Arduino Ödevleri ve Sınav Çalışmaları

Gömülü Sistemler dersi için Arduino ile çözülen ödev ve vize hazırlık soruları.
Her sketch kendi klasöründe durur (klasör adı = `.ino` adı), bu yüzden Arduino IDE'de
doğrudan açılır: **File → Open → ilgili `.ino` dosyası**.

Kartlar: Arduino Uno (Nano için de geçerli). Uno'da interrupt pinleri **2 ve 3**'tür.

## İçindekiler

| Klasör | Sketch | Konu |
|---|---|---|
| `01_buton_ve_zaman` | `buton_sayma_10sn` | İlk basıştan sonra 10 sn içindeki basış sayısı (`millis()`, kenar algılama) |
| | `led_cift_basis` | İki basış arası süre < 100 ms ise led yanık kalır, değilse yanıp söner |
| | `periyot_kesmesiz` | İki butonla led periyodu ayarı, **interrupt'sız** (`millis()`) |
| | `periyot_kesmeli` | Aynı soru, **interrupt'lı** (`attachInterrupt`, `volatile`) |
| `02_mesafe_sensoru_urun_sayma` | `mesafe_urun_say` | HC-SR04 ile bantta ürün sayma (histerezis: 85 / 75 cm) |
| | `urun_ayri_say` | 20 cm ve 30 cm'lik ürünleri geçiş süresiyle ayırma |
| | `urun3_say` | 3 çeşit ürün: yükseklik (mesafe) + genişlik (süre) |
| `03_su_deposu` | `su_seviye` | 2 m'lik depo, dolum/boşaltım vanaları (200 cm / 10 cm eşikleri) |
| `04_giris_sayaci` | `giris_sayaci` | AVM giriş sayacı, kenar algılama (önceki/şimdiki durum) |
| `05_kosu_zamanlayici` | `kosu_zamanlayici` | 100 m koşu süresi ve hızı, iki interrupt pini |
| `06_encoder_adim_motoru` | `encoder_adim_polling` | Encoder ile adım motoru, interrupt'sız |
| | `encoder_adim_kesme` | Aynı soru, interrupt'lı |
| `07_dc_motor_ve_enkoder` | `dc_motor_yon` | Sürücü üzerinden DC motor yön/hız (IN1, IN2, PWM, STBY) |
| | `enkoder_hiz_kontrol` | Enkoder geri beslemeli aç-kapa hız kontrolü |
| `08_onewire` | `onewire_read_rom` | 1-Wire reset/presence + Read ROM (0x33), hocanın fonksiyonlarıyla |
| | `onewire_read_rom_kisa` | Aynısının kısaltılmış hali |
| | `crc8_1wire` | CRC-8 (Dallas/Maxim) ödevi, 3 testle (DD, C9, 3A) |
| | `ds18b20` | Kütüphanesiz DS18B20 sıcaklık okuma |

## Pin özetleri

| Sketch | Bağlantılar |
|---|---|
| butonlu sketch'ler | Buton → pin 2 (ve 3) ile GND arası, `INPUT_PULLUP` (basılı değilken HIGH, basınca LOW) |
| HC-SR04 sketch'leri | Trig → D9, Echo → D10, VCC → 5V, GND → GND |
| `su_seviye` | Trig D9, Echo D10, dolum vanası D7, boşaltım vanası D8 |
| `kosu_zamanlayici` | Start → D2, bitiş sensörü → D3 (10 kΩ pull-down) |
| encoder sketch'leri | Encoder A → D2, B → D3, sürücü puls → D4, direction → D5 |
| `dc_motor_yon`, `enkoder_hiz_kontrol` | IN1 D4, IN2 D5, PWM D6, STBY D7, enkoder A D2 / B D3 |
| 1-Wire sketch'leri | Veri hattı → D2, D2 ile 5V arasında 4.7 kΩ pull-up |

## Notlar

- `crc8_1wire` çıktısı her test için `CRC: DD Beklenen: DD`, `Gecerli`, `Hata bulundu` şeklindedir.
  Beklenen değerler (DD, C9, 3A) algoritmanın bağımsız bir uygulamasıyla da doğrulandı.
- `giris_sayaci` tam dosya olarak sohbette yoktu; `loop()` ve `setup()` parçalarından
  birleştirildi. Seri çıktı metni yeni yazıldı.
- `enkoder_hiz_kontrol`: tahta fotoğrafında `Hiz` formülü okunamadı, darbe/sn olarak
  tamamlandı (dosyada belirtildi).
- `onewire_read_rom` ve `ds18b20` donanım gerektirir; diğerleri mantık olarak
  Tinkercad'de de simüle edilebilir.
- Tüm sketch'ler derleme öncesi sözdizimi kontrolünden geçirildi. Gerçek kart
  üzerinde yüklenip denenmedi.
- RFID banka/çok imzalı kasa projesi ayrı bir repoda: `rfid-banka-kasa`.
