// 1-Wire CRC-8 (Dallas/Maxim) - hazir kutuphane YOK, bit bit elle
// Baslangic 0, polinom 0x31 (LSB-first icin aynalanmis hali 0x8C)
//
// Odev maddeleri:
//   A: 8 baytlik veriden CRC hesapla
//   B: CRC'yi sona ekle, paketi tekrar isle -> 0 ise gecerli
//   C: tek bit boz, hatanin yakalandigini goster
//   D: 3 test icin hesaplanan ve beklenen CRC'yi yazdir

byte crc8(byte *d, int n) {
  byte crc = 0;                       // 1. sifirdan basla
  for (int i = 0; i < n; i++) {       // 2. her bayt icin
    byte x = d[i];                    //    bayti al
    for (int j = 0; j < 8; j++) {     // 3. baytin 8 biti icin
      byte bit = (crc ^ x) & 1;       //    en dusuk bitleri XOR'la
      crc >>= 1;                      //    CRC'yi kaydir
      if (bit) crc ^= 0x8C;           //    bit 1 ise polinomla XOR'la
      x >>= 1;                        //    veriyi kaydir
    }
  }
  return crc;                         // 4. sonucu ver
}

/* Ezber icin cok kisa esdegeri:
uint8_t crc8(uint8_t *v, uint8_t n) {
  uint8_t c = 0;
  while (n--) {
    c ^= *v++;
    for (uint8_t i = 8; i--;)
      c = (c >> 1) ^ ((c & 1) * 0x8C);
  }
  return c;
}
*/

void test(byte *d, int n, byte beklenen) {
  byte sonuc = crc8(d, n);            // A: CRC hesapla
  d[n] = sonuc;                       // B: CRC'yi sona ekle

  Serial.print("CRC: ");              // D: hesaplanan ve beklenen
  Serial.print(sonuc, HEX);
  Serial.print(" Beklenen: ");
  Serial.println(beklenen, HEX);

  Serial.println(crc8(d, n + 1) == 0 ? "Gecerli" : "Gecersiz");           // B: tekrar isle

  d[0] ^= 1;                          // C: tek bit boz
  Serial.println(crc8(d, n + 1) == 0 ? "Hata yok" : "Hata bulundu");      // C: hata algilandi mi
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  byte a[9] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};   // beklenen DD
  byte b[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // beklenen C9
  byte c[8] = {0x28, 0xFF, 0x6C, 0x92, 0x91, 0x16, 0x04};         // ROM, beklenen 3A
  test(a, 8, 0xDD);
  test(b, 8, 0xC9);
  test(c, 7, 0x3A);
}

void loop() {}
