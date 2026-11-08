// b) Interrupt kullanarak cozum
// Cikis (led) belirlenen periyotla yanip soner.
// 1. buton periyodu 10 ms artirir, 2. buton 10 ms azaltir.
// Periyot en az 10 ms, en fazla 1000 ms olabilir. Baslangic: 500 ms.
//
// Arduino Uno'da interrupt sadece 2 ve 3 numarali pinlerde calisir.
// INPUT_PULLUP kullanildigi icin basinca pin HIGH'dan LOW'a duser (FALLING).

const int buton1 = 2;               // artiran buton
const int buton2 = 3;               // azaltan buton
const int cikisPin = 13;            // cikis (varsayim)

volatile int periyot = 500;         // cikis periyodu (ms), interrupt icinde degistigi icin volatile

void artir() {                      // 1. butona basilinca otomatik calisir
  if (periyot < 1000) periyot += 10;
}

void azalt() {                      // 2. butona basilinca otomatik calisir
  if (periyot > 10) periyot -= 10;
}

void setup() {
  pinMode(buton1, INPUT_PULLUP);
  pinMode(buton2, INPUT_PULLUP);
  pinMode(cikisPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buton1), artir, FALLING);
  attachInterrupt(digitalPinToInterrupt(buton2), azalt, FALLING);
}

void loop() {
  digitalWrite(cikisPin, HIGH);
  delay(periyot / 2);               // yarim periyot yanik
  digitalWrite(cikisPin, LOW);
  delay(periyot / 2);               // yarim periyot sonuk
}
