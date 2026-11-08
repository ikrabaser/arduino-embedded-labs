// Tahtadaki kod 1: DC motor surme (surucu uzerinden)
// D4 -> IN1, D5 -> IN2, D6 -> PWM, D7 -> STBY/Enable (TB6612FNG benzeri surucu)
// IN1/IN2 yonu belirler (0-1 bir yon, 1-0 ters yon), PWM hizi belirler.
// Geri besleme yok: 1 sn bir yone, 1 sn ters yone doner.

void setup() {
  pinMode(4, OUTPUT);   // IN1
  pinMode(5, OUTPUT);   // IN2
  pinMode(6, OUTPUT);   // PWM
  pinMode(7, OUTPUT);   // STBY / Enable (surucuyu acar)
  digitalWrite(7, 1);
}

void loop() {
  digitalWrite(4, 0);   // IN1 = 0
  digitalWrite(5, 1);   // IN2 = 1  -> bir yone don
  analogWrite(6, 127);  // PWM, yaklasik %50 hiz
  delay(1000);
  digitalWrite(4, 1);   // IN1 = 1
  digitalWrite(5, 0);   // IN2 = 0  -> ters yone don
  delay(1000);
}
