// Butona ust uste 2 kez basilinca, iki basis arasindaki sure
//   100 ms'den kisaysa -> led yanar ve yanik kalir
//   degilse            -> led yanip soner
//
// INPUT_PULLUP kullanildigi icin:
//   buton basili degilken -> HIGH (1)
//   buton basiliyken      -> LOW  (0)

const int butonPin = 2;
const int ledPin = 13;              // kart uzerindeki led (varsayim)

unsigned long ilkBasis;             // 1. basisin zamani
unsigned long fark;                 // iki basis arasindaki sure (ms)

void setup() {
  pinMode(butonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  while (digitalRead(butonPin) == HIGH) {}      // 1. basisi bekle
  ilkBasis = millis();                          // 1. basisin zamanini kaydet
  digitalWrite(ledPin, LOW);                    // onceki sonuctan kalan isigi sondur

  while (digitalRead(butonPin) == LOW) {}       // buton birakilana kadar bekle
  while (digitalRead(butonPin) == HIGH) {}      // 2. basisi bekle
  fark = millis() - ilkBasis;                   // simdiki zaman - 1. basisin zamani

  if (fark < 100) {
    digitalWrite(ledPin, HIGH);                 // hizli basis: led yanik kalir
  }
  else {
    digitalWrite(ledPin, HIGH);                 // yavas basis: led yanip soner
    delay(500);
    digitalWrite(ledPin, LOW);
  }

  while (digitalRead(butonPin) == LOW) {}       // 2. basis birakilsin, sonra basa don
}
