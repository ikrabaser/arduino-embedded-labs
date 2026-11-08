// a) Interrupt kullanmadan cozum
// Cikis (led) belirlenen periyotla yanip soner.
// 1. buton periyodu 10 ms artirir, 2. buton 10 ms azaltir.
// Periyot en az 10 ms, en fazla 1000 ms olabilir. Baslangic: 500 ms.
//
// INPUT_PULLUP kullanildigi icin:
//   buton basili degilken -> HIGH (1)
//   buton basiliyken      -> LOW  (0)

const int buton1 = 2;               // artiran buton
const int buton2 = 3;               // azaltan buton
const int cikisPin = 13;            // cikis (varsayim)

int periyot = 500;                  // cikis periyodu (ms)
unsigned long sonDegisim = 0;       // cikisin en son degistigi an
int cikisDurumu = LOW;              // cikisin su anki hali
int eskiB1 = HIGH;                  // 1. butonun bir onceki okunan hali
int eskiB2 = HIGH;                  // 2. butonun bir onceki okunan hali

void setup() {
  pinMode(buton1, INPUT_PULLUP);
  pinMode(buton2, INPUT_PULLUP);
  pinMode(cikisPin, OUTPUT);
}

void loop() {
  int b1 = digitalRead(buton1);
  int b2 = digitalRead(buton2);

  if (b1 == LOW && eskiB1 == HIGH) {            // 1. butona yeni basildi
    if (periyot < 1000) periyot += 10;
  }
  if (b2 == LOW && eskiB2 == HIGH) {            // 2. butona yeni basildi
    if (periyot > 10) periyot -= 10;
  }
  eskiB1 = b1;                                  // bu turdaki hali sakla
  eskiB2 = b2;

  if (millis() - sonDegisim >= periyot / 2) {   // yarim periyot doldu mu?
    sonDegisim = millis();
    cikisDurumu = !cikisDurumu;                 // cikisi tersine cevir
    digitalWrite(cikisPin, cikisDurumu);
  }
}
