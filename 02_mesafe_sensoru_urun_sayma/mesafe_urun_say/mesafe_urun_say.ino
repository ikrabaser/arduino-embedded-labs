// Urun bandi: sensor bandin ustunde, mesafe degisimine bakarak urun sayar.
// HC-SR04: VCC -> 5V, GND -> GND, Trig -> 9, Echo -> 10
//
// Histerezis: iki esik (85 ve 75) kullanilir, tek esik olsa sensor
// titredikce ayni urun birkac kez sayilirdi.

const int trigPin = 9;
const int echoPin = 10;

int say = 0;                                // urun sayaci

int mesafe() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);              // 10 us'lik tetikleme darbesi
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000);  // en fazla 30 ms bekle
  if (sure == 0) return 999;                  // yansima gelmediyse "cok uzak" say
  return sure * 0.034 / 2;                    // cm cinsinden mesafe
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  while (mesafe() > 85) {}                  // urun gelene kadar bekle
  say++;
  while (mesafe() < 75) {}                  // urun gecene kadar bekle
  Serial.println(say);
}
