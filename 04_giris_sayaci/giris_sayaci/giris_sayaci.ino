// Alisveris merkezi giris sayaci (tek sensor, sadece giren kisiyi sayar)
// Sensor kisi onundeyken surekli 1 verir; kenar algilama (0 -> 1) ile
// ayni kisi birden fazla sayilmaz.

const int SENSOR_PIN = 2;

int sayac = 0;
int oncekiDurum;                    // Sensorun bir onceki okunan degeri

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  oncekiDurum = digitalRead(SENSOR_PIN);  // Baslangic degerini gercek sensorden al
  Serial.begin(9600);
  Serial.println("Giris sayaci basladi.");
}

void loop() {
  int simdikiDurum = digitalRead(SENSOR_PIN);   // 1. Simdiye bak

  if (simdikiDurum == HIGH && oncekiDurum == LOW) {   // 2. Az onceyle karsilastir
    sayac++;
    Serial.print("Giris sayisi: ");
    Serial.println(sayac);
  }

  oncekiDurum = simdikiDurum;                   // 3. Simdiyi hafizaya yaz
  delay(20);
}
