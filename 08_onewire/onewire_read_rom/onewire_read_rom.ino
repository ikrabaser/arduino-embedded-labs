// 1-Wire: Reset/Presence + Read ROM (0x33)  -  hocanin fonksiyonlari aynen
// Veri hatti: D2 (+ 4.7 kohm pull-up onerilir). Tek cihaz bagliyken calisir.

// HOCANIN TAHTADAKI "0" GONDERME FONKSIYONU
void write_0() {
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  delayMicroseconds(100);
  pinMode(2, INPUT_PULLUP);
  delayMicroseconds(1);
}

// HOCANIN TAHTADAKI "1" GONDERME FONKSIYONU
void write_1() {
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  delayMicroseconds(1);
  pinMode(2, INPUT_PULLUP);
  delayMicroseconds(60);
}

// HOCANIN TAHTADAKI 1 BAYT (8 BIT) OKUMA FONKSIYONU
int read_byte() {
  int a = 0;
  for (int i = 0; i < 8; i = i + 1) {
    pinMode(2, OUTPUT);
    digitalWrite(2, 0);
    delayMicroseconds(1);

    pinMode(2, INPUT_PULLUP);
    delayMicroseconds(15);

    if (digitalRead(2)) {
      a = a | (1 << i);
    }

    delayMicroseconds(45);
  }
  return a;
}

// 1 BAYT GONDERME (LSB ONCE)
void write_byte(byte v) {
  for (int i = 0; i < 8; i++) {
    if (v & 1) write_1(); else write_0();
    v >>= 1;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // RESET VE PRESENCE
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  delayMicroseconds(480);

  pinMode(2, INPUT_PULLUP);
  delayMicroseconds(70);
  int durum = digitalRead(2);
  delayMicroseconds(410);

  if (durum == 0) {
    // READ ROM KOMUTU (0x33)
    write_byte(0x33);

    // 8 BAYT ROM: [0]=Family, [1..6]=Seri No, [7]=CRC
    byte rom[8];
    for (int i = 0; i < 8; i++) rom[i] = read_byte();

    Serial.print("Family Code: ");
    Serial.println(rom[0], HEX);

    Serial.print("Serial Number: ");
    for (int i = 1; i <= 6; i++) {
      Serial.print(rom[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("CRC: ");
    Serial.println(rom[7], HEX);
  }

  delay(10000);
}
