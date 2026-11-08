// Uyku modu + watchdog kesmesi ile periyodik uyanma (8 sn) - avr/sleep.h, avr/wdt.h
//
// Pille calisan sensor dugumlerinin klasik yapisi: cogu zaman power-down uykusunda,
// 8 sn'de bir uyan, kisa bir olcum/is yap, tekrar uyu.
//
// Watchdog burada RESET degil KESME modunda kullanilir (WDIE=1, WDE=0). Watchdog kesmesi
// uykudaki islemciyi uyandirir; ISR icinde bir sey yapmak gerekmez.
//
// Baglanti: sadece dahili led (D13). Gercek bir sensor ekleyeceksen "is()" fonksiyonuna yaz.
//
// Tuketim hesabi icin (ornek): uyanik 10 ms, uykuda 8 sn ise cihaz zamanin %99.9'unda uykudadir;
// ortalama akim ~ (uyanik akim * 0.125%) + (uyku akimi * 99.875%).

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

const int LED = 13;

ISR(WDT_vect) {
  // Bos: watchdog kesmesi uykudan uyandirmak icin yeterli
}

void wdt8snKesmeModu() {
  noInterrupts();
  wdt_reset();
  MCUSR &= ~(1 << WDRF);                      // watchdog reset bayragini temizle
  WDTCSR |= (1 << WDCE) | (1 << WDE);         // ayar degistirme izni (4 saat cevrimi icinde yaz)
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);   // kesme modu, 8 sn periyot
  interrupts();
}

void uyu() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  noInterrupts();
  sleep_enable();
  sleep_bod_disable();
  interrupts();
  sleep_cpu();                      // WDT kesmesi gelene kadar uyur
  sleep_disable();
}

void is() {                         // uyaninca yapilacak kisa is (burada led yakip sondurur)
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
}

void setup() {
  pinMode(LED, OUTPUT);
  power_adc_disable();              // ADC kullanilmiyor
  power_spi_disable();
  power_twi_disable();
  wdt8snKesmeModu();
}

void loop() {
  is();
  uyu();                            // 8 sn sonra WDT kesmesiyle uyanip loop basina doner
}
