// 20 cm ve 30 cm'lik urunleri ayri ayri sayan kod (anlasilir isimlerle)
// Bant hizi 1 m/s oldugu icin:
//   20 cm'lik urun sensorun altinda yaklasik 200 ms kalir
//   30 cm'lik urun sensorun altinda yaklasik 300 ms kalir
// 250 ms, ikisinin ortasi: ondan kisaysa 20 cm'lik, uzunsa 30 cm'lik

const int trigPin = 9;
const int echoPin = 10;

int urun20 = 0;                 // 20 cm'lik urun sayisi
int urun30 = 0;                 // 30 cm'lik urun sayisi
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
  while (mesafe() > 85) {}              // urun gelene kadar bekle
  baslangic = millis();                 // kronometreyi baslat

  while (mesafe() < 75) {}              // urun gidene kadar bekle
  gecenSure = millis() - baslangic;     // kronometreyi durdur

  if (gecenSure < 250)
    urun20++;                           // kisa sure: 20 cm'lik urun
  else
    urun30++;                           // uzun sure: 30 cm'lik urun

  Serial.print(urun20);
  Serial.print(" ");
  Serial.println(urun30);
}
