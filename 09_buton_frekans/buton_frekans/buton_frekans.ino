// 2 buton, 1 led: frekans uretimi
//   1. buton basili -> led 20 Hz (periyot 50 ms: 25 ms yanik, 25 ms sonuk)
//   2. buton basili -> led 10 Hz (periyot 100 ms: 50 ms yanik, 50 ms sonuk)
//   iki buton ayni anda basili (veya hicbiri basili degil) -> sinyal yok, led sonuk
//
// INPUT_PULLUP kullanildigi icin: buton basili degilken HIGH, basiliyken LOW.
// delay() yerine millis() kullaniyoruz, boylece butonlar surekli okunur.

const int buton1 = 2;               // 20 Hz butonu
const int buton2 = 3;               // 10 Hz butonu
const int ledPin = 13;              // cikis (varsayim)

unsigned long sonDegisim = 0;       // ledin en son degistigi an
int ledDurumu = LOW;                // ledin su anki hali

void setup() {
  pinMode(buton1, INPUT_PULLUP);
  pinMode(buton2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  bool b1 = (digitalRead(buton1) == LOW);   // 1. buton basili mi?
  bool b2 = (digitalRead(buton2) == LOW);   // 2. buton basili mi?

  unsigned long yarim;                      // yarim periyot (ms)
  if (b1 && !b2) {
    yarim = 25;                             // 20 Hz -> periyot 50 ms -> yarisi 25 ms
  } else if (b2 && !b1) {
    yarim = 50;                             // 10 Hz -> periyot 100 ms -> yarisi 50 ms
  } else {
    // ikisi birden basili ya da hicbiri basili degil: sinyal uretme
    ledDurumu = LOW;
    digitalWrite(ledPin, LOW);
    sonDegisim = millis();                  // bir sonraki basista sifirdan basla
    return;
  }

  if (millis() - sonDegisim >= yarim) {     // yarim periyot doldu mu?
    sonDegisim = millis();
    ledDurumu = !ledDurumu;                 // ledi tersine cevir
    digitalWrite(ledPin, ledDurumu);
  }
}
