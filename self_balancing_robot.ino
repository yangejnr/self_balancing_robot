// ===== MOTOR 1 PINS (existing) =====
const int L1_EN_PIN   = 23;
const int R1_EN_PIN   = 2;
const int L1_PWM_PIN  = 21;
const int R1_PWM_PIN  = 22;
const int HALL1_A     = 5;
const int HALL1_B     = 4;

// ===== MOTOR 2 PINS (new) =====
const int L2_EN_PIN   = 15;
const int R2_EN_PIN   = 16;
const int L2_PWM_PIN  = 19;   // PWM-capable
const int R2_PWM_PIN  = 25;   // PWM-capable
const int HALL2_A     = 17;
const int HALL2_B     = 18;

// Encoder positions
volatile long encoderPos1 = 0;
volatile long encoderPos2 = 0;

// Motion parameters
long targetDistance = 500;  // counts per move (calibrate)
int phase = 0;              // 0=forward, 1=backward

// ===== ENCODER ISRs =====
void IRAM_ATTR encoder1ISR() {
  static uint8_t lastAB1 = 0b00;
  uint8_t currAB1 = (digitalRead(HALL1_A) << 1) | digitalRead(HALL1_B);

  switch (lastAB1) {
    case 0b00: if (currAB1 == 0b01 || currAB1 == 0b10) encoderPos1++; break;
    case 0b01: if (currAB1 == 0b00 || currAB1 == 0b11) encoderPos1++; break;
    case 0b10: if (currAB1 == 0b00 || currAB1 == 0b11) encoderPos1--; break;
    case 0b11: if (currAB1 == 0b01 || currAB1 == 0b10) encoderPos1--; break;
  }
  lastAB1 = currAB1;
}

void IRAM_ATTR encoder2ISR() {
  static uint8_t lastAB2 = 0b00;
  uint8_t currAB2 = (digitalRead(HALL2_A) << 1) | digitalRead(HALL2_B);

  switch (lastAB2) {
    case 0b00: if (currAB2 == 0b01 || currAB2 == 0b10) encoderPos2++; break;
    case 0b01: if (currAB2 == 0b00 || currAB2 == 0b11) encoderPos2++; break;
    case 0b10: if (currAB2 == 0b00 || currAB2 == 0b11) encoderPos2--; break;
    case 0b11: if (currAB2 == 0b01 || currAB2 == 0b10) encoderPos2--; break;
  }
  lastAB2 = currAB2;
}

// ===== MOTOR 1 CONTROL =====
void motor1Forward(int speed) {
  analogWrite(L1_PWM_PIN, speed);
  analogWrite(R1_PWM_PIN, 0);
}
void motor1Backward(int speed) {
  analogWrite(L1_PWM_PIN, 0);
  analogWrite(R1_PWM_PIN, speed);
}
void motor1Stop() {
  analogWrite(L1_PWM_PIN, 0);
  analogWrite(R1_PWM_PIN, 0);
}

// ===== MOTOR 2 CONTROL =====
void motor2Forward(int speed) {
  analogWrite(L2_PWM_PIN, speed);
  analogWrite(R2_PWM_PIN, 0);
}
void motor2Backward(int speed) {
  analogWrite(L2_PWM_PIN, 0);
  analogWrite(R2_PWM_PIN, speed);
}
void motor2Stop() {
  analogWrite(L2_PWM_PIN, 0);
  analogWrite(R2_PWM_PIN, 0);
}

void setup() {
  Serial.begin(115200);

  // === MOTOR 1 PINS ===
  pinMode(L1_EN_PIN, OUTPUT);
  pinMode(R1_EN_PIN, OUTPUT);
  pinMode(L1_PWM_PIN, OUTPUT);
  pinMode(R1_PWM_PIN, OUTPUT);
  pinMode(HALL1_A, INPUT_PULLUP);
  pinMode(HALL1_B, INPUT_PULLUP);

  // === MOTOR 2 PINS ===
  pinMode(L2_EN_PIN, OUTPUT);
  pinMode(R2_EN_PIN, OUTPUT);
  pinMode(L2_PWM_PIN, OUTPUT);
  pinMode(R2_PWM_PIN, OUTPUT);
  pinMode(HALL2_A, INPUT_PULLUP);
  pinMode(HALL2_B, INPUT_PULLUP);

  // Enable both drivers
  digitalWrite(L1_EN_PIN, HIGH);
  digitalWrite(R1_EN_PIN, HIGH);
  digitalWrite(L2_EN_PIN, HIGH);
  digitalWrite(R2_EN_PIN, HIGH);

  // Attach encoder interrupts
  attachInterrupt(digitalPinToInterrupt(HALL1_A), encoder1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HALL2_A), encoder2ISR, CHANGE);

  encoderPos1 = 0;
  encoderPos2 = 0;

  Serial.println("=== DUAL MOTOR HIGH-SPEED TEST ===");
  Serial.println("Phase | Target | M1 Pos | M2 Pos");
}

void loop() {
  long target = (phase == 0) ? targetDistance : -targetDistance;

  Serial.print("Phase "); Serial.print(phase);
  Serial.print(" | Tgt: "); Serial.print(target);
  Serial.print(" | M1: "); Serial.print(encoderPos1);
  Serial.print(" | M2: "); Serial.println(encoderPos2);

  bool m1Reached = (target > 0) ? (encoderPos1 >= target) : (encoderPos1 <= target);
  bool m2Reached = (target > 0) ? (encoderPos2 >= target) : (encoderPos2 <= target);

  // MOTOR 1 CONTROL - MAX SPEED 255
  if (!m1Reached) {
    if (phase == 0) motor1Forward(255);      // FULL SPEED FORWARD
    else motor1Backward(255);                // FULL SPEED BACKWARD
  } else {
    motor1Stop();
  }

  // MOTOR 2 CONTROL - MAX SPEED 255
  if (!m2Reached) {
    if (phase == 0) motor2Forward(255);      // FULL SPEED FORWARD
    else motor2Backward(255);                // FULL SPEED BACKWARD
  } else {
    motor2Stop();
  }

  // Advance phase when BOTH motors reach target
  if (m1Reached && m2Reached) {
    motor1Stop();
    motor2Stop();
    Serial.println(">> BOTH MOTORS REACHED TARGET. SWITCHING PHASE...");
    phase = (phase + 1) % 2;  // Toggle forward/backward
    encoderPos1 = 0;
    encoderPos2 = 0;
    delay(1500);  // Pause between phases
  }

  delay(50);
}
