// SORU 2 a) Adim motorunu encoder ile kontrol et - INTERRUPT OLMADAN
// Encoder 2 cikisli (A, B), 1 tur 200 pulse. 1 encoder pulse'i = 1 motor adimi.
// Pin 2: encoder A, Pin 3: encoder B, Pin 4: surucu puls, Pin 5: surucu direction
//
// A kanalinin yukselen kenarini loop icinde kendimiz yakaliyoruz.
// Yukselen kenarda B dusukse saga, yuksekse sola donmus demektir.
// loop icinde uzun delay olmamali, yoksa encoder pulse'lari kacar.

const int ENC_A = 2;
const int ENC_B = 3;
const int PULS  = 4;   // surucu - puls
const int DIR   = 5;   // surucu - direction

int sonA;

void adimAt(bool yon) {
  digitalWrite(DIR, yon);
  delayMicroseconds(5);      // yon oturma suresi
  digitalWrite(PULS, HIGH);
  delayMicroseconds(5);      // surucunun istedigi min. puls genisligi
  digitalWrite(PULS, LOW);
  delayMicroseconds(5);
}

void setup() {
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(PULS, OUTPUT);
  pinMode(DIR, OUTPUT);
  sonA = digitalRead(ENC_A);
}

void loop() {
  int a = digitalRead(ENC_A);
  if (a == HIGH && sonA == LOW) {          // A'da yukselen kenar
    if (!digitalRead(ENC_B)) adimAt(HIGH); // saga
    else                     adimAt(LOW);  // sola
  }
  sonA = a;
}
