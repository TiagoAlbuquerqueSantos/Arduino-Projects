
#include <Servo.h>

// =====================================================
// CONFIGURAÇÕES
// =====================================================

#define DEADZONE 120
#define SERVO_DELAY 20
#define SERVO_STEP_MIN 1
#define SERVO_STEP_MAX 5

// =====================================================
// PINOS DOS JOYSTICKS
// =====================================================

#define JL_X A0
#define JL_Y A1
#define JL_SW 2

#define JR_X A3
#define JR_Y A4
#define JR_SW 3

// =====================================================
// PINOS DOS SERVOS
// =====================================================

#define SERVO_BASE_PIN   8
#define SERVO_REACH_PIN  9
#define SERVO_HEIGHT_PIN 10
#define SERVO_CLAW_PIN   11

// =====================================================
// SERVOS
// =====================================================

Servo servoBase;
Servo servoReach;
Servo servoHeight;
Servo servoClaw;

// =====================================================
// ÂNGULOS ATUAIS
// =====================================================

int angleBase   = 90;
int angleReach  = 90;
int angleHeight = 90;
int angleClaw   = 120;

// =====================================================
// LIMITES
// =====================================================

const int BASE_MIN   = 10;
const int BASE_MAX   = 170;

const int REACH_MIN  = 30;
const int REACH_MAX  = 170;

const int HEIGHT_MIN = 40;
const int HEIGHT_MAX = 170;

const int CLAW_MIN   = 90;
const int CLAW_MAX   = 170;

// =====================================================
// CENTRO DOS JOYSTICKS
// =====================================================

int centerJLX = 512;
int centerJLY = 512;
int centerJRX = 512;
int centerJRY = 512;

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  pinMode(JL_SW, INPUT_PULLUP);
  pinMode(JR_SW, INPUT_PULLUP);

  // Attach servos
  servoBase.attach(SERVO_BASE_PIN);
  servoReach.attach(SERVO_REACH_PIN);
  servoHeight.attach(SERVO_HEIGHT_PIN);
  servoClaw.attach(SERVO_CLAW_PIN);

  // Posição inicial
  servoBase.write(angleBase);
  servoReach.write(angleReach);
  servoHeight.write(angleHeight);
  servoClaw.write(angleClaw);

  // Calibração
  calibrateJoysticks();

  Serial.println("RoboARM Uno R4 iniciado");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  controlBase();
  controlHeight();
  controlReach();
  controlClaw();

  handleClawButton();

  delay(SERVO_DELAY);
}

// =====================================================
// CALIBRAÇÃO
// =====================================================

void calibrateJoysticks() {

  long sumLX = 0;
  long sumLY = 0;
  long sumRX = 0;
  long sumRY = 0;

  const int samples = 50;

  for (int i = 0; i < samples; i++) {

    sumLX += analogRead(JL_X);
    sumLY += analogRead(JL_Y);

    sumRX += analogRead(JR_X);
    sumRY += analogRead(JR_Y);

    delay(5);
  }

  centerJLX = sumLX / samples;
  centerJLY = sumLY / samples;

  centerJRX = sumRX / samples;
  centerJRY = sumRY / samples;

  Serial.println("Joysticks calibrados");
}

// =====================================================
// DEADZONE
// =====================================================

bool outsideDeadzone(int value) {
  return abs(value) > DEADZONE;
}

// =====================================================
// VELOCIDADE PROPORCIONAL
// =====================================================

int computeStep(int value) {

  int magnitude = abs(value);

  return map(
    magnitude,
    DEADZONE,
    512,
    SERVO_STEP_MIN,
    SERVO_STEP_MAX
  );
}

// =====================================================
// BASE
// =====================================================

void controlBase() {

  int value = analogRead(JL_Y) - centerJLY;

  if (!outsideDeadzone(value))
    return;

  int step = computeStep(value);

  if (value > 0)
    angleBase += step;
  else
    angleBase -= step;

  angleBase = constrain(angleBase, BASE_MIN, BASE_MAX);

  servoBase.write(angleBase);
}

// =====================================================
// HEIGHT
// =====================================================

void controlHeight() {

  int value = analogRead(JL_X) - centerJLX;

  if (!outsideDeadzone(value))
    return;

  int step = computeStep(value);

  if (value > 0)
    angleHeight -= step;
  else
    angleHeight += step;

  // Correção geométrica
  int minHeight = (-0.75 * angleReach) + 165;

  angleHeight = constrain(angleHeight, minHeight, HEIGHT_MAX);

  servoHeight.write(angleHeight);
}

// =====================================================
// REACH
// =====================================================

void controlReach() {

  int value = analogRead(JR_X) - centerJRX;

  if (!outsideDeadzone(value))
    return;

  int step = computeStep(value);

  if (value > 0)
    angleReach -= step;
  else
    angleReach += step;

  int minReach = (angleHeight - 165) / -0.75;

  angleReach = constrain(angleReach, minReach, REACH_MAX);

  servoReach.write(angleReach);
}

// =====================================================
// CLAW
// =====================================================

void controlClaw() {

  int value = analogRead(JR_Y) - centerJRY;

  if (!outsideDeadzone(value))
    return;

  int step = computeStep(value);

  if (value > 0)
    angleClaw += step;
  else
    angleClaw -= step;

  angleClaw = constrain(angleClaw, CLAW_MIN, CLAW_MAX);

  servoClaw.write(angleClaw);
}

// =====================================================
// BOTÃO DA GARRA
// =====================================================

void handleClawButton() {

  static bool lastState = HIGH;

  bool currentState = digitalRead(JR_SW);

  // Detecta clique
  if (lastState == HIGH && currentState == LOW) {

    if (angleClaw > 120)
      angleClaw = CLAW_MIN;
    else
      angleClaw = CLAW_MAX;

    servoClaw.write(angleClaw);

    delay(150);
  }

  lastState = currentState;
}

