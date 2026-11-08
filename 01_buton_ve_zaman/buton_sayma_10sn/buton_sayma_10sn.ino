// Butona ilk basildiktan sonra 10 sn icinde butona kac kez basildigini sayar.
// Buton: pin 2 ile GND arasi, INPUT_PULLUP
//   buton basili degilken -> HIGH (1)
//   buton basiliyken      -> LOW  (0)

int say;
unsigned long t;

void setup() {
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  say = 0;

  while (digitalRead(2)) {}          // 1 oldugu surece (basilmadikca) bekle
  t = millis();                      // ilk basis: zamani al
  say = 1;                           // ilk basis sayilir (sayilmasin istenirse say = 0)
  while (!digitalRead(2)) {}         // buton birakilana kadar bekle
  delay(30);                         // debounce

  bool onceki = HIGH;
  while (millis() - t < 10000) {     // 10 sn boyunca
    bool simdi = digitalRead(2);
    if (onceki == HIGH && simdi == LOW) {   // 1 -> 0 gecisi = yeni basis
      say++;
      delay(30);                     // debounce
    }
    onceki = simdi;
  }

  Serial.println(say);               // 10 sn bitince sonucu yaz
}
