// 2 potansiyometre ile 2 adim motoru kontrolu
//
// Pot 1 (A0) -> Motor 1 (D2..D5)      Pot 2 (A1) -> Motor 2 (D6..D9)
// Motorlar surucu karti (ornegin ULN2003) uzerinden baglanir.
//
// Calisma: pot okumasi -64..+64 araligina cevrilir (konum). Her turda yeni konum
// eskisiyle karsilastirilir, fark kadar adim atilir:
//   pot artmis  (fark < 0) -> yon 1
//   pot azalmis (fark > 0) -> yon 0
// Yani motor potun mutlak konumuna degil, DEGISIMINE gore doner.
//
// Adim dizisi: tek bobin acik (wave drive). Bir "adim" = 4 bobinin sirayla bir turu.

const int MOTOR1_ILK_PIN = 2;      // motor 1: D2, D3, D4, D5
const int MOTOR2_ILK_PIN = 6;      // motor 2: D6, D7, D8, D9
const int BEKLEME = 5;             // bobinler arasi bekleme (ms)

int konum1;                        // pot 1'in en son konumu (-64..64)
int konum2;                        // pot 2'nin en son konumu (-64..64)

int potKonum(int pin) {
  return map(analogRead(pin), 0, 1023, -64, 64);
}

// motor: 1 veya 2, yon: 1 -> ilk pinden son pine, 0 -> son pinden ilk pine
void dondur(int adimSayisi, int motor, int yon) {
  int ilkPin = (motor == 1) ? MOTOR1_ILK_PIN : MOTOR2_ILK_PIN;

  for (int i = 0; i < adimSayisi; i++) {
    for (int k = 0; k < 4; k++) {
      int pin = (yon == 1) ? ilkPin + 3 - k : ilkPin + k;
      digitalWrite(pin, HIGH);
      delay(BEKLEME);
      digitalWrite(pin, LOW);
    }
  }
}

void setup() {
  for (int pin = 2; pin <= 9; pin++) {
    pinMode(pin, OUTPUT);
  }
  konum1 = potKonum(A0);           // baslangic konumu: acilista motor oynamasin
  konum2 = potKonum(A1);
}

void loop() {
  int yeni1 = potKonum(A0);
  int yeni2 = potKonum(A1);

  int fark1 = konum1 - yeni1;
  int fark2 = konum2 - yeni2;

  dondur(abs(fark1), 1, fark1 < 0 ? 1 : 0);
  dondur(abs(fark2), 2, fark2 < 0 ? 1 : 0);

  konum1 = yeni1;
  konum2 = yeni2;
}
