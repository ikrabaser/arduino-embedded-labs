// Su deposu seviye kontrolu (HC-SR04). Depo yuksekligi 200 cm.
// Su seviyesi = 200 - mesafe (sensor deponun tepesinde, suya bakiyor)
// Seviye 200 cm'ye cikinca dolum kapanir, bosaltim acilir;
// seviye 10 cm'ye inince bosaltim kapanir, dolum acilir.
//
// Pinler: D9 Trig, D10 Echo, D7 dolum vanasi, D8 bosaltim vanasi
// Not: bircok role modulu ters calisir (LOW = acik), o zaman HIGH/LOW yer degistirilir.

const int TRIG = 9, ECHO = 10;
const int DOLUM = 7, BOSALTIM = 8;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(DOLUM, OUTPUT);
  pinMode(BOSALTIM, OUTPUT);
  digitalWrite(DOLUM, HIGH);          // Dolumla basla (depo bos kabul edildi)
}

void loop() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  float seviye = 200 - pulseIn(ECHO, HIGH) * 0.017;   // us -> cm

  if (seviye >= 200) {                // Depo doldu
    digitalWrite(DOLUM, LOW);
    digitalWrite(BOSALTIM, HIGH);
  }
  else if (seviye <= 10) {            // Depo bosaldi
    digitalWrite(BOSALTIM, LOW);
    digitalWrite(DOLUM, HIGH);
  }
  delay(500);
}
