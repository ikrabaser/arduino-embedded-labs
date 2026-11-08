// Tahtadaki kod 2+3: Enkoder ile geri beslemeli hiz kontrolu (ac-kapa / on-off)
// Enkoder: D2 -> A (kesme pini), D3 -> B (yon)
// Surucu : D4 -> IN1, D5 -> IN2, D6 -> PWM, D7 -> STBY
//
// NOT: Tahta fotografinda Hiz formulu okunamiyordu; asagidaki hesap
// (darbe/sn = 1000 / iki darbe arasi ms) tamamlanmis halidir.
// Esik (100) ve PWM degerleri tahtadaki gibidir.

volatile long sayac = 0;            // enkoder sayaci (yon: B kanalina gore)
volatile unsigned long sonT = 0;    // onceki darbenin zamani
volatile float Hiz = 0;             // darbe/sn

void encoder() {                    // A kanali yukselen kenarda cagrilir (ISR)
  unsigned long T = millis();       // zaman damgasi
  if (digitalRead(3))               // B kanalinin durumuna bak
    sayac++;
  else
    sayac--;
  if (T > sonT) Hiz = 1000.0 / (T - sonT);   // iki darbe arasi sureden hiz
  sonT = T;
}

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7, 1);               // surucuyu ac
  attachInterrupt(digitalPinToInterrupt(2), encoder, RISING);
}

void loop() {
  digitalWrite(4, 0);
  digitalWrite(5, 1);

  noInterrupts();
  float h = Hiz;                    // paylasilan degiskenin guvenli kopyasi
  interrupts();

  if (h < 100)
    analogWrite(6, 127);            // hiz dusukse motora PWM ver
  else
    analogWrite(6, 0);              // hiz yuksekse kes
}
