// PID ile DC motor hiz kontrolu (07'deki ac-kapa kontrolun gelismis hali)
//
// 07_dc_motor_ve_enkoder/enkoder_hiz_kontrol: hiz < 100 ise PWM 127, degilse 0 (ac-kapa).
// Burada ayni donanimla PID kullaniyoruz: PWM, hedef hiza gore surekli ayarlanir.
//
// Enkoder A: D2 (kesme)           Surucu: D4 -> IN1, D5 -> IN2, D6 -> PWM, D7 -> STBY
//
// Hiz = 200 ms'lik pencerede sayilan darbe / pencere suresi (darbe/sn).
// Seri Cizici (Serial Plotter) ile "hedef, hiz, pwm" egrilerini izleyebilirsin.
//
// PID:  cikis = Kp*hata + Ki*integral(hata) + Kd*turev
//   Kp: hata buyukse PWM'i hemen buyuk degistirir
//   Ki: kalici hatayi (hedefin biraz altinda kalmayi) giderir
//   Kd: hizdaki ani degisimi sonumler (asiri/salinimi azaltir)
// Kp, Ki, Kd degerleri ornektir; motoruna gore deneyerek ayarlanir.

const int ENC_A = 2;
const int IN1 = 4;
const int IN2 = 5;
const int PWM_PIN = 6;
const int STBY = 7;

const float HEDEF = 100.0;          // hedef hiz (darbe/sn)
const float Kp = 1.0;
const float Ki = 2.0;
const float Kd = 0.05;
const unsigned long ORNEK_MS = 200; // olcum / kontrol periyodu

volatile unsigned long darbe = 0;   // pencere icindeki darbe sayisi

float integral = 0;                 // hatanin birikimi
float oncekiHiz = 0;                // turev icin onceki olcum
unsigned long sonOrnek = 0;

void encoder() {                    // A kanali yukselen kenar (ISR)
  darbe++;
}

void setup() {
  pinMode(ENC_A, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);         // surucuyu ac
  digitalWrite(IN1, LOW);           // tek yone don
  digitalWrite(IN2, HIGH);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder, RISING);
  Serial.begin(9600);
  sonOrnek = millis();
}

void loop() {
  unsigned long simdi = millis();
  if (simdi - sonOrnek < ORNEK_MS) return;   // periyot dolmadiysa bekle

  float dt = (simdi - sonOrnek) / 1000.0;    // gercek gecen sure (sn)
  sonOrnek = simdi;

  noInterrupts();
  unsigned long d = darbe;                   // paylasilan sayaci guvenli oku
  darbe = 0;
  interrupts();

  float hiz = d / dt;                        // darbe/sn
  float hata = HEDEF - hiz;

  integral += hata * dt;
  // anti-windup: Ki*integral PWM araligi (0..255) disina cikmasin
  integral = constrain(integral, 0.0, 255.0 / Ki);

  // turevi hataya degil olcume uyguluyoruz (hedef degisince ani sicrama olmaz)
  float turev = -(hiz - oncekiHiz) / dt;
  oncekiHiz = hiz;

  float cikis = Kp * hata + Ki * integral + Kd * turev;
  int pwm = (int)constrain(cikis, 0.0, 255.0);
  analogWrite(PWM_PIN, pwm);

  Serial.print(HEDEF);
  Serial.print(',');
  Serial.print(hiz);
  Serial.print(',');
  Serial.println(pwm);
}
