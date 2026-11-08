// Uyku modu (Power-down) + butonla uyanma - avr/sleep.h
//
// Uyku modlari (azalan tuketim): IDLE, ADC, PWR_SAVE, STANDBY, PWR_DOWN.
// Power-down en dusuk tuketimli moddur: ana osilator durur, CPU ve cogu birim kapanir.
// Bu modda sadece harici seviye kesmesi (INT0/INT1 LOW), pin degisimi kesmesi ve watchdog
// uyandirabilir. Bu yuzden butonu kesme pini D2'ye bagliyoruz ve LOW seviyesiyle uyandiriyoruz
// (power-down'da FALLING/RISING kenar kesmesi calismaz).
//
// Baglanti: D2 -> buton (diger ucu GND, INPUT_PULLUP), D13 -> dahili led
//
// Akis: uyu -> butona basinca uyan -> led 2 sn yanar -> tekrar uyu.
//
// NOT: Uno kartinda regulator, USB cipi ve led de akim cektigi icin kart toplam tuketimi
// uykuda pek dusmez. Gercek kazanc cipin tek basina (cihaza gomulu) kullanildigi devrelerde olur.

#include <avr/sleep.h>
#include <avr/power.h>

const int LED = 13;
const int BUTON = 2;

void uyanma() {                     // ISR: sadece uyandirir
  detachInterrupt(digitalPinToInterrupt(BUTON));   // LOW seviye surekli tetiklemesin
}

void uyu() {
  Serial.println("Uykuya geciyor...");
  Serial.flush();                   // yazinin gitmesini bekle (yoksa yarim kalir)

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  noInterrupts();                   // kesme ile sleep arasinda yaris olmasin
  attachInterrupt(digitalPinToInterrupt(BUTON), uyanma, LOW);
  sleep_enable();
  sleep_bod_disable();              // uyurken brown-out dedektorunu kapat (daha az akim)
  interrupts();                     // sei'den sonraki ilk komut (sleep_cpu) kesilmeden calisir
  sleep_cpu();                      // --- burada uyur, butona basinca devam eder ---
  sleep_disable();
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTON, INPUT_PULLUP);
  Serial.begin(9600);
  power_adc_disable();              // kullanmadigimiz ADC'yi kapat
}

void loop() {
  uyu();

  Serial.println("Uyandi!");
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);

  while (digitalRead(BUTON) == LOW) {}   // buton birakilana kadar bekle
  delay(50);                             // kontak sekmesi (debounce)
}
