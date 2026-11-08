// I2C ile MPU6050 (ivmeolcer + jiroskop) okuma - Wire.h
//
// Baglanti (GY-521 modulu):  VCC -> 5V (modulde regulator var), GND -> GND,
//                            SDA -> A4, SCL -> A5, AD0 -> GND (adres 0x68)
//
// I2C'de master (Arduino) slave'in adresini yazar, sonra register adresi gonderir:
//   yazma: [adres+W] [register] [deger]
//   okuma: [adres+W] [register]  sonra  [adres+R] [bayt] [bayt] ...
//
// Kullanilan register'lar:
//   0x6B PWR_MGMT_1 : 0 yazilirsa sensor uyanir (acilista uyku modunda)
//   0x75 WHO_AM_I   : 0x68 doner (kimlik kontrolu)
//   0x3B ...        : ACCEL_X, ACCEL_Y, ACCEL_Z, TEMP, GYRO_X, GYRO_Y, GYRO_Z
//                     (her biri 2 bayt, once yuksek bayt) = toplam 14 bayt

#include <Wire.h>

const int MPU = 0x68;               // I2C adresi (AD0 = GND)

void yaz(byte reg, byte deger) {
  Wire.beginTransmission(MPU);
  Wire.write(reg);
  Wire.write(deger);
  Wire.endTransmission();
}

byte oku1(byte reg) {
  Wire.beginTransmission(MPU);
  Wire.write(reg);
  Wire.endTransmission(false);      // false: hatti birakma (repeated start)
  Wire.requestFrom(MPU, 1);
  return Wire.read();
}

int16_t oku16() {                   // 2 bayt oku: yuksek bayt once
  int16_t yuksek = Wire.read();
  int16_t dusuk = Wire.read();
  return (yuksek << 8) | dusuk;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  yaz(0x6B, 0);                     // uyku modundan cik

  Serial.print("WHO_AM_I: 0x");
  Serial.println(oku1(0x75), HEX);  // 0x68 beklenir (bazi klonlar farkli deger verebilir)
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);                 // okumaya ACCEL_XOUT_H'den basla
  if (Wire.endTransmission(false) != 0) {
    Serial.println("MPU6050 bulunamadi, baglantiyi kontrol et.");
    delay(1000);
    return;
  }
  Wire.requestFrom(MPU, 14);        // 14 bayt art arda oku

  int16_t ax = oku16();
  int16_t ay = oku16();
  int16_t az = oku16();
  int16_t sicaklik = oku16();
  int16_t gx = oku16();
  int16_t gy = oku16();
  int16_t gz = oku16();

  // varsayilan olcek: ivme +-2 g -> 16384 LSB/g, jiroskop +-250 derece/sn -> 131 LSB/(derece/sn)
  Serial.print("Ivme (g): ");
  Serial.print(ax / 16384.0); Serial.print("  ");
  Serial.print(ay / 16384.0); Serial.print("  ");
  Serial.print(az / 16384.0);
  Serial.print(" | Jiro (derece/sn): ");
  Serial.print(gx / 131.0); Serial.print("  ");
  Serial.print(gy / 131.0); Serial.print("  ");
  Serial.print(gz / 131.0);
  Serial.print(" | Sicaklik (C): ");
  Serial.println(sicaklik / 340.0 + 36.53);   // veri sayfasindaki formul

  delay(500);
}
