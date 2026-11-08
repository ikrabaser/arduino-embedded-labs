// DS18B20 sicaklik okuma - kutuphanesiz 1-Wire
// Arduino Nano, veri hatti D2, D2 ile 5V arasinda 4.7k direnc, VDD 5V'a bagli
// 0xCC (Skip ROM) sadece hatta tek cihaz varsa calisir.

void write_bit(int b) {
  pinMode(2, OUTPUT); digitalWrite(2, 0);
  delayMicroseconds(b ? 1 : 100);
  pinMode(2, INPUT);
  delayMicroseconds(b ? 60 : 1);
}

int read_bit() {
  pinMode(2, OUTPUT); digitalWrite(2, 0);
  delayMicroseconds(1);
  pinMode(2, INPUT);
  delayMicroseconds(2);
  int b = digitalRead(2);
  delayMicroseconds(55);
  return b;
}

int reset() {
  pinMode(2, OUTPUT); digitalWrite(2, 0);
  delayMicroseconds(480);
  pinMode(2, INPUT);
  delayMicroseconds(70);
  int p = !digitalRead(2);   // 0 ise cihaz var
  delayMicroseconds(410);
  return p;
}

void write_byte(byte x) {
  for (int i = 0; i < 8; i++, x >>= 1) write_bit(x & 1);   // once LSB
}

byte read_byte() {
  byte a = 0;
  for (int i = 0; i < 8; i++) { a >>= 1; if (read_bit()) a |= 0x80; }
  return a;
}

float sicaklik() {
  reset(); write_byte(0xCC); write_byte(0x44);   // Skip ROM, Convert T
  delay(750);
  reset(); write_byte(0xCC); write_byte(0xBE);   // Skip ROM, Read Scratchpad
  byte lsb = read_byte(), msb = read_byte();
  return (int16_t)((msb << 8) | lsb) / 16.0;
}

void setup() { Serial.begin(9600); }

void loop() {
  if (reset()) { Serial.print(sicaklik()); Serial.println(" C"); }
  else Serial.println("Cihaz YOK");
  delay(1000);
}
