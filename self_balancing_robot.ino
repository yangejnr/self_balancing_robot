// ===== MOTOR 1 PINS (Left Motor) =====
const int L1_EN_PIN   = 23;
const int R1_EN_PIN   = 2;
const int L1_PWM_PIN  = 21;
const int R1_PWM_PIN  = 22;

// ===== MOTOR 2 PINS (Right Motor) =====
const int L2_EN_PIN   = 15;
const int R2_EN_PIN   = 16;
const int L2_PWM_PIN  = 19;
const int R2_PWM_PIN  = 25;

void setup() {
  Serial.begin(115200);

  // All pins OUTPUT
  pinMode(L1_EN_PIN, OUTPUT); pinMode(R1_EN_PIN, OUTPUT);
  pinMode(L1_PWM_PIN, OUTPUT); pinMode(R1_PWM_PIN, OUTPUT);
  pinMode(L2_EN_PIN, OUTPUT); pinMode(R2_EN_PIN, OUTPUT);
  pinMode(L2_PWM_PIN, OUTPUT); pinMode(R2_PWM_PIN, OUTPUT);

  // Enable ALL drivers
  digitalWrite(L1_EN_PIN, HIGH); digitalWrite(R1_EN_PIN, HIGH);
  digitalWrite(L2_EN_PIN, HIGH); digitalWrite(R2_EN_PIN, HIGH);

  Serial.println("=== BTS7960 DIRECTION TEST ===");
  Serial.println("Commands: F1=B1=F2=B2=S (STOP)");
}

void loop() {
  Serial.println("\n--- SEND COMMAND ---");
  while (Serial.available() == 0) delay(100);
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  // MOTOR 1 TESTS
  if (cmd == "F1") {
    Serial.println("MOTOR 1 FORWARD: L_PWM=255, R_PWM=0");
    analogWrite(L1_PWM_PIN, 255); analogWrite(R1_PWM_PIN, 0);
  }
  else if (cmd == "B1") {
    Serial.println("MOTOR 1 BACKWARD: L_PWM=0, R_PWM=255");
    analogWrite(L1_PWM_PIN, 0); analogWrite(R1_PWM_PIN, 255);
  }

  // MOTOR 2 TESTS  
  else if (cmd == "F2") {
    Serial.println("MOTOR 2 FORWARD: L_PWM=255, R_PWM=0");
    analogWrite(L2_PWM_PIN, 255); analogWrite(R2_PWM_PIN, 0);
  }
  else if (cmd == "B2") {
    Serial.println("MOTOR 2 BACKWARD: L_PWM=0, R_PWM=255");
    analogWrite(L2_PWM_PIN, 0); analogWrite(R2_PWM_PIN, 255);
  }

  // STOP ALL
  else if (cmd == "S") {
    Serial.println("ALL MOTORS STOPPED");
    analogWrite(L1_PWM_PIN, 0); analogWrite(R1_PWM_PIN, 0);
    analogWrite(L2_PWM_PIN, 0); analogWrite(R2_PWM_PIN, 0);
  }
  
  delay(500);
}
