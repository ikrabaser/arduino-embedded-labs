// 100 m kosucu zamanlayici: sure ve ortalama hiz
// Pin 2: start (starter butonu), Pin 3: bitis (lazer/IR bariyer)
// Uno'da dis kesme (interrupt) pinleri sadece 2 ve 3'tur.
// Girislere 10 kohm pull-down: normalde LOW, olayda HIGH (RISING).

const int START_PIN = 2;
const int BITIS_PIN = 3;
const float MESAFE = 100.0;                   // metre

volatile unsigned long baslangicZamani = 0;   // start ani (ms)
volatile unsigned long kosuSuresi = 0;        // toplam sure (ms)
volatile bool kosuDevamEdiyor = false;
volatile bool sonucHazir = false;             // sure hesaplandi mi?

void startGeldi() {
  baslangicZamani = millis();
  kosuDevamEdiyor = true;
}

void bitisGeldi() {
  if (kosuDevamEdiyor) {                      // start gelmeden bitis sayilmasin
    kosuSuresi = millis() - baslangicZamani;
    kosuDevamEdiyor = false;
    sonucHazir = true;
  }
}

void setup() {
  pinMode(START_PIN, INPUT);
  pinMode(BITIS_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(START_PIN), startGeldi, RISING);
  attachInterrupt(digitalPinToInterrupt(BITIS_PIN), bitisGeldi, RISING);
  Serial.println("Hazir. Start bekleniyor...");
}

void loop() {
  if (sonucHazir) {                           // hesap loop'ta, kesme icinde degil
    float saniye = kosuSuresi / 1000.0;
    Serial.print("Sure: ");
    Serial.print(saniye);
    Serial.println(" s");
    Serial.print("Hiz: ");
    Serial.print(MESAFE / saniye);
    Serial.println(" m/s");
    sonucHazir = false;
  }
}
