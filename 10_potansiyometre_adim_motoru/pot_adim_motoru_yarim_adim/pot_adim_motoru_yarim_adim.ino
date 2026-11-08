// 2 potansiyometre ile 2 adim motoru kontrolu - v2 (yarim adim + olu bolge)
//
// v1'den (pot_adim_motoru) farklari:
//  1. Yarim adim (half-step) dizisi: tek bobin / iki bobin dönüşümlü, 8 durum.
//     Hareket daha yumusak ve guclu, bekleme 1 ms (v1'de 5 ms) oldugu icin daha hizli.
//  2. Olu bolge (ESIK): pot konumu en az ESIK birim degismedikce motor oynamaz.
//     Referans konum sadece motor dondugunde guncellenir. Pot gurultusu motoru titretmez.
//  3. Seri monitore pot 2'nin ham degeri yazilir (esigi ayarlamak icin).
//
// Pot 1 (A0) -> Motor 1 (D2..D5)      Pot 2 (A1) -> Motor 2 (D6..D9)
// Motorlar surucu karti (ornegin ULN2003) uzerinden baglanir.
//
// Pot okumasi -64..+64 araligina cevrilir (konum). Fark kadar "adim" (4 bobinlik tur) atilir:
//   pot artmis  (fark < -ESIK) -> yon 1
//   pot azalmis (fark >  ESIK) -> yon 0

const int MOTOR1_ILK_PIN = 2;      // motor 1: D2, D3, D4, D5
const int MOTOR2_ILK_PIN = 6;      // motor 2: D6, D7, D8, D9
const int BEKLEME = 1;             // durumlar arasi bekleme (ms)
const int ESIK = 10;               // olu bolge (konum biriminde)

// Yarim adim durumlari. Her bit bir bobin: bit 0 = ilk pin, bit 3 = son pin.
// 8 durum artan sirada: {0}, {0,1}, {1}, {1,2}, {2}, {2,3}, {3}, {3,0}
const byte DURUM[8] = {
  0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001
};
const int BASLANGIC = 6;           // her tur son bobinden ({3}) baslar

int konum1;                        // pot 1'in referans konumu (-64..64)
int konum2;                        // pot 2'nin referans konumu (-64..64)

int potKonum(int pin) {
  return map(analogRead(pin), 0, 1023, -64, 64);
}

void durumYaz(int ilkPin, byte durum) {
  for (int k = 0; k < 4; k++) {
    digitalWrite(ilkPin + k, (durum >> k) & 1);
  }
}

// motor: 1 veya 2, yon: 1 -> son pinden ilk pine, 0 -> ilk pinden son pine
void dondur(int adimSayisi, int motor, int yon) {
  int ilkPin = (motor == 1) ? MOTOR1_ILK_PIN : MOTOR2_ILK_PIN;

  for (int i = 0; i < adimSayisi; i++) {
    for (int n = 0; n < 8; n++) {
      int idx = (yon == 1) ? BASLANGIC - n : BASLANGIC + n;
      durumYaz(ilkPin, DURUM[(idx + 8) % 8]);
      delay(BEKLEME);
    }
    durumYaz(ilkPin, DURUM[BASLANGIC]);   // tur sonunda sadece son bobin acik kalir
  }
}

void setup() {
  for (int pin = 2; pin <= 9; pin++) {
    pinMode(pin, OUTPUT);
  }
  konum1 = potKonum(A0);           // baslangic konumu: acilista motor oynamasin
  konum2 = potKonum(A1);
  Serial.begin(9600);
}

void loop() {
  int yeni1 = potKonum(A0);
  int yeni2 = potKonum(A1);
  Serial.println(analogRead(A1));  // ayar icin, istersen sil

  int fark1 = konum1 - yeni1;
  int fark2 = konum2 - yeni2;

  if (fark1 < -ESIK) { dondur(abs(fark1), 1, 1); konum1 = yeni1; }
  if (fark1 >  ESIK) { dondur(abs(fark1), 1, 0); konum1 = yeni1; }

  if (fark2 < -ESIK) { dondur(abs(fark2), 2, 1); konum2 = yeni2; }
  if (fark2 >  ESIK) { dondur(abs(fark2), 2, 0); konum2 = yeni2; }
}
