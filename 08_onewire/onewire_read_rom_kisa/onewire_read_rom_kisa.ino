// 1-Wire: Reset/Presence + Read ROM (0x33)  -  kisaltilmis surum (~45 satir)
// write_0 ve write_1 tek bit_gonder() fonksiyonunda birlestirildi.
// Cikti tek satir: ROM: 28 FF 4A ... (Family, 6 bayt seri no, CRC)

void bit_gonder(bool b) {            // write_0 ve write_1 birlesti
  pinMode(2, OUTPUT); digitalWrite(2, 0);
  delayMicroseconds(b ? 1 : 100);
  pinMode(2, INPUT_PULLUP);
  delayMicroseconds(b ? 60 : 1);
}

byte bayt_oku() {
  byte a = 0;
  for (int i = 0; i < 8; i++) {
    pinMode(2, OUTPUT); digitalWrite(2, 0);
    delayMicroseconds(1);
    pinMode(2, INPUT_PULLUP);
    delayMicroseconds(15);
    if (digitalRead(2)) a |= 1 << i;
    delayMicroseconds(45);
  }
  return a;
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // RESET VE PRESENCE
  pinMode(2, OUTPUT); digitalWrite(2, 0);
  delayMicroseconds(480);
  pinMode(2, INPUT_PULLUP);
  delayMicroseconds(70);
  bool var = !digitalRead(2);        // slave hatti LOW cektiyse cihaz var
  delayMicroseconds(410);

  if (var) {
    for (int i = 0; i < 8; i++) bit_gonder((0x33 >> i) & 1);  // Read ROM, LSB once

    Serial.print("ROM: ");           // Family, 6 bayt Seri No, CRC
    for (int i = 0; i < 8; i++) { Serial.print(bayt_oku(), HEX); Serial.print(" "); }
    Serial.println();
  }
  delay(10000);
}
