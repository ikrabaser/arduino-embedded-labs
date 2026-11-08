// SPI ile 74HC595 kaydirmali yazmac: 8 ledi 3 pinle surme - SPI.h
//
// Baglanti (Uno):  D11 (MOSI) -> 74HC595 pin 14 (DS)
//                  D13 (SCK)  -> 74HC595 pin 11 (SH_CP)
//                  D10 (SS)   -> 74HC595 pin 12 (ST_CP, latch)
//                  74HC595: pin 16 (VCC) -> 5V, pin 8 (GND) -> GND,
//                           pin 10 (MR) -> 5V, pin 13 (OE) -> GND
//                  Q0..Q7 (pin 15, 1..7) -> 220 ohm direnc -> led -> GND
//
// SPI'da master saat (SCK) uretir, bitler MOSI uzerinden MSB once kaydirilir.
// Latch (ST_CP) yukselince kaydirma yazmacindaki 8 bit cikislara aktarilir;
// boylece cikislar kaydirma sirasinda titremez.
//
// Ilk kisim: 0..255 ikili sayac. Ikinci kisim: yanip kayan tek led.

#include <SPI.h>

const int LATCH = 10;               // ST_CP (SS pini)

void yaz595(byte veri) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(LATCH, LOW);
  SPI.transfer(veri);               // 8 bit kaydir
  digitalWrite(LATCH, HIGH);        // cikislara aktar
  SPI.endTransaction();
}

void setup() {
  pinMode(LATCH, OUTPUT);
  SPI.begin();
}

void loop() {
  for (int i = 0; i < 256; i++) {   // ikili sayac
    yaz595(i);
    delay(50);
  }
  for (int i = 0; i < 8; i++) {     // kayan led
    yaz595(1 << i);
    delay(100);
  }
}
