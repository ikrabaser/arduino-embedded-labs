// SORU 2 b) Adim motorunu encoder ile kontrol et - INTERRUPT ILE
// Pin 2: encoder A, Pin 3: encoder B, Pin 4: surucu puls, Pin 5: surucu direction
//
// Duzeltmeler:
//  1. sayac ISR ile loop arasinda paylasildigi icin volatile
//  2. int AVR'de 16 bit oldugu icin loop'ta okurken/degistirirken
//     interrupt kisa sure kapatilir (atomik erisim)
//  3. puls HIGH/LOW arasina kisa bekleme (surucu icin daha guvenli)
//
// Encoder pulse'i aninda sayac'a islenir, motor yavas kalirsa biriken
// adimlar sayac'ta bekler ve sirayla islenir ("cevirdigim an artacak").

const int ENC_A = 2;
const int ENC_B = 3;
const int PULS  = 4;
const int DIR   = 5;

volatile int sayac = 0;   // >0: saga cevrilmis, <0: sola cevrilmis

void encoder() {          // ISR: A'nin yukselen kenarinda calisir
  if (!digitalRead(ENC_B)) sayac++;
  else                     sayac--;
}

void adimAt(bool yon) {
  digitalWrite(DIR, yon);
  delayMicroseconds(5);
  digitalWrite(PULS, HIGH);
  delayMicroseconds(5);
  digitalWrite(PULS, LOW);
  delayMicroseconds(5);
}

void setup() {
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(PULS, OUTPUT);
  pinMode(DIR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder, RISING);
}

void loop() {
  noInterrupts();
  int s = sayac;          // sayac'i guvenli kopyala
  interrupts();

  if (s > 0) {
    adimAt(HIGH);
    noInterrupts(); sayac--; interrupts();
  }
  else if (s < 0) {
    adimAt(LOW);
    noInterrupts(); sayac++; interrupts();
  }
}
