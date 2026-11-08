// 3 cesit urunu ayri ayri sayan kod (sade surum)
// Bant hizi 1 m/s. Sensor bandin 100 cm ustunde varsayildi.
//
//   Urun   Yukseklik  Genislik   Sensor mesafesi   Gecis suresi
//   1.     20 cm      20 cm      100 - 20 = 80     ~200 ms
//   2.     20 cm      30 cm      100 - 20 = 80     ~300 ms
//   3.     30 cm      30 cm      100 - 30 = 70     ~300 ms
//
// Yukseklik mesafeden, genislik gecis suresinden anlasilir.
//   Mesafe esigi: 75 (70 ile 80'in ortasi)
//   Sure esigi: 250 ms (200 ile 300'un ortasi)

const int trigPin = 9;
const int echoPin = 10;

int urun1 = 0;                  // 20 x 20 urun sayisi
int urun2 = 0;                  // 20 x 30 urun sayisi
int urun3 = 0;                  // 30 x 30 urun sayisi
unsigned long baslangic;        // kronometrenin basladigi an
unsigned long gecenSure;        // urunun sensor altinda kaldigi sure (ms)

int mesafe() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000);
  if (sure == 0) return 999;
  return sure * 0.034 / 2;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  while (mesafe() >= 90) {}             // urun gelene kadar bekle
  baslangic = millis();                 // kronometreyi baslat

  if (mesafe() < 75) {                  // uzun boylu urun: 3. urun
    while (mesafe() < 90) {}            // urunun gitmesini bekle
    urun3++;
  }
  else {                                // kisa boylu urun: 1. ya da 2. urun
    while (mesafe() < 90) {}            // urunun gitmesini bekle
    gecenSure = millis() - baslangic;   // kronometreyi durdur
    if (gecenSure < 250)
      urun1++;                          // dar urun: 1. urun
    else
      urun2++;                          // genis urun: 2. urun
  }

  Serial.print(urun1);
  Serial.print(" ");
  Serial.print(urun2);
  Serial.print(" ");
  Serial.println(urun3);
}
