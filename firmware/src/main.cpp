#include <Arduino.h>
#include <FastAccelStepper.h>

constexpr int PIN_EN = 14;
constexpr int PIN_SPREAD = 15;
constexpr int PIN_UART = 18;
constexpr int PIN_STEP = 20;
constexpr int PIN_DIR = 21;

constexpr int SPEED = 500;  // step per sec
constexpr int ACCEL = 1000; // step per sec^2

constexpr int TOP_POSITION = 0;       // step per sec^2
constexpr int BOTTOM_POSITION = 1000; // step per sec^2

enum user_input
{
  PAUSE,
  TOP,
  BOTTOM,
  OPTIONS_COUNT
};

user_input command = PAUSE;

int read_int()
{
  int command = -1;

  if (Serial.available() > 0)
  {
    command = Serial.parseInt();

    Serial.printf("Command recieved: %d\n", command);
  }

  return command;
};

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = nullptr;

void setup()
{
  pinMode(PIN_STEP, OUTPUT);
  digitalWrite(PIN_SPREAD, LOW);

  Serial.begin(115200);
  delay(1000);

  engine.init();

  stepper = engine.stepperConnectToPin(PIN_STEP);

  if (stepper)
  {
    Serial.println("SUCESS: Stepper connected!");

    stepper->setDirectionPin(PIN_DIR);
    stepper->setEnablePin(PIN_EN);
    stepper->setAutoEnable(true);
    stepper->setSpeedInHz(SPEED);
    stepper->setAcceleration(ACCEL);
    stepper->setCurrentPosition(0);
  }
  else
  {
    Serial.println("ERROR: Stepper failed to connect.");
  }
}

void loop()
{

  command = static_cast<user_input>(read_int());

  if (command != -1)
  {
    Serial.printf("Current Position: %d Enter an option:\n\t0.Pause\n\t1. Move Top\n\t2. Move Bottom\n", stepper->getCurrentPosition());

    switch (command)
    {
    case PAUSE:
      Serial.println("Action: STOP");
      stepper->stopMove();
      break;

    case TOP:
      Serial.println("Action: TOP");
      stepper->moveTo(TOP_POSITION, false);
      break;

    case BOTTOM:
      Serial.println("Action: BOTTOM");
      stepper->moveTo(BOTTOM_POSITION, false);
      break;

    default:
      Serial.printf("Invalid command! Enter a valid integer between 1 and %d.\n", OPTIONS_COUNT);
      break;
    }
  }
}