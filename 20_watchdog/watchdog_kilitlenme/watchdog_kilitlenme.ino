// Watchdog zamanlayici (WDT) ile kilitlenmeden kurtarma - avr/wdt.h
//
// Watchdog, kendi osilatoruyla calisan bir geri sayim sayacidir. Program belirli surede
// wdt_reset() ile sayaci sifirlamazsa watchdog islemciyi RESETLER. Kod bir dongude
// takilirsa (kilitlenirse) wdt_reset() cagrilmaz ve kart kendini yeniden baslatir.
//
// Baglanti: D13 -> dahili led, D2 -> buton (diger ucu GND, INPUT_PULLUP)
//
// Deneme: led yanip soner. Butona basinca kod sonsuz donguye girer (kilitlenme benzetimi),
// 2 sn sonra watchdog karti resetler: seri monitorde "Sistem basladi" tekrar gorunur,
// led once 3 kez hizli yanip soner, sonra normal yanip sonmeye doner.
//
// NOT: Arduino Uno (Optiboot) ile sorunsuz calisir. Eski bootloader'li Nano'larda
// watchdog resetinden sonra kart surekli resetlenmeye girebilir (bootloader WDT'yi kapatmaz).

#include <avr/wdt.h>

const int LED = 13;
const int BUTON = 2;

unsigned long sonDegisim = 0;
int ledDurumu = LOW;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTON, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Sistem basladi");

  for (int i = 0; i < 3; i++) {     // yeniden baslama gorunsun diye 3 hizli yanip sonme
    digitalWrite(LED, HIGH);
    delay(80);
    digitalWrite(LED, LOW);
    delay(80);
  }

  wdt_enable(WDTO_2S);              // 2 sn icinde wdt_reset() gelmezse reset
}

void loop() {
  if (digitalRead(BUTON) == LOW) {  // butona basildi: kilitlenmeyi benzet
    Serial.println("Kilitlenme benzetimi: sonsuz donguye giriliyor...");
    Serial.flush();                 // yazinin gitmesini bekle
    while (true) {}                 // wdt_reset() hic cagrilmaz -> 2 sn sonra reset
  }

  if (millis() - sonDegisim >= 500) {   // normal is: led 500 ms'de bir degisir
    sonDegisim = millis();
    ledDurumu = !ledDurumu;
    digitalWrite(LED, ledDurumu);
  }

  wdt_reset();                      // "sistem saglikli" - sayaci sifirla
}
