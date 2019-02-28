// https://github.com/MHeironimus/ArduinoJoystickLibrary

#include <Keyboard.h>
#include <Joystick.h>
#include <Bounce2.h>

#define JOYSTICK 1
#define KEYBOARD 2

#define JOYSTICK_BUTTON_FIRE    0
#define JOYSTICK_BUTTON_SELECT  1
#define JOYSTICK_BUTTON_START   2

Joystick_ joystick(
  0x03,   // uint8_t hidReportId  - Default: 0x03 - Indicates the joystick's HID report ID.
          // This value must be unique if you are creating multiple instances of Joystick.
          // Do not use 0x01 or 0x02 as they are used by the built-in Arduino Keyboard and Mouse libraries. 
  JOYSTICK_TYPE_GAMEPAD,  // uint8_t joystickType - Default: JOYSTICK_TYPE_JOYSTICK or 0x04 - Indicates the HID input device type.
                          // Supported values:
                          // JOYSTICK_TYPE_JOYSTICK or 0x04 - Joystick
                          // JOYSTICK_TYPE_GAMEPAD or 0x05 - Gamepad
                          // JOYSTICK_TYPE_MULTI_AXIS or 0x08 - Multi-axis Controller
  3,      // uint8_t  buttonCount -         Default: 32 - Indicates how many buttons will be available on the joystick. 
  0,      // uint8_t  hatSwitchCount -      Default: 2 - Indicates how many hat switches will be available on the joystick. Range: 0 - 2
  true,   // bool     includeXAxis -        Default: true - Indicates if the X Axis is available on the joystick.
  true,   // bool     includeYAxis -        Default: true - Indicates if the Y Axis is available on the joystick.
  false,  // bool     includeZAxis -        Default: true - Indicates if the Z Axis (in some situations this is the right X Axis) is available on the joystick.
  false,  // bool     includeRxAxis -       Default: true - Indicates if the X Axis Rotation (in some situations this is the right Y Axis) is available on the joystick.
  false,  // bool     includeRyAxis -       Default: true - Indicates if the Y Axis Rotation is available on the joystick.
  false,  // bool     includeRzAxis -       Default: true - Indicates if the Z Axis Rotation is available on the joystick.
  false,  // bool     includeRudder -       Default: true - Indicates if the Rudder is available on the joystick.
  false,  // bool     includeThrottle -     Default: true - Indicates if the Throttle is available on the joystick.
  false,  // bool     includeAccelerator -  Default: true - Indicates if the Accelerator is available on the joystick.
  false,  // bool     includeBrake -        Default: true - Indicates if the Brake is available on the joystick.
  false   // bool     includeSteering -     Default: true - Indicates if the Steering is available on the joystick.
);

// Joystick to pin mapping
#define PIN_UP     2
#define PIN_DOWN   3
#define PIN_LEFT   4
#define PIN_RIGHT  5
#define PIN_FIRE   6
#define PIN_SELECT 7
#define PIN_START  8

int inputMode ;

// Bounce in milliseconds
#define BOUNCE_INTERVAL    5

// Bounce for pins
Bounce upBounce =     Bounce();
Bounce downBounce =   Bounce();
Bounce leftBounce =   Bounce();
Bounce rightBounce =  Bounce();
Bounce fireBounce =   Bounce();
Bounce selectBounce = Bounce();
Bounce startBounce =  Bounce();

void setup() {
  Serial.begin(9600);

  pinMode (PIN_UP,     INPUT_PULLUP);
  pinMode (PIN_DOWN,   INPUT_PULLUP);
  pinMode (PIN_LEFT,   INPUT_PULLUP);
  pinMode (PIN_RIGHT,  INPUT_PULLUP);
  pinMode (PIN_FIRE,   INPUT_PULLUP);
  pinMode (PIN_SELECT, INPUT_PULLUP);
  pinMode (PIN_START,  INPUT_PULLUP);

  upBounce.attach     (PIN_UP);
  downBounce.attach   (PIN_DOWN);
  leftBounce.attach   (PIN_LEFT);
  rightBounce.attach  (PIN_RIGHT);
  fireBounce.attach   (PIN_FIRE);
  selectBounce.attach (PIN_SELECT);
  startBounce.attach  (PIN_START);

  upBounce.interval     (BOUNCE_INTERVAL);
  downBounce.interval   (BOUNCE_INTERVAL);
  leftBounce.interval   (BOUNCE_INTERVAL);
  rightBounce.interval  (BOUNCE_INTERVAL);
  fireBounce.interval   (BOUNCE_INTERVAL);
  selectBounce.interval (BOUNCE_INTERVAL);
  startBounce.interval  (BOUNCE_INTERVAL);

  selectBounce.update();
  startBounce.update();
  if (selectBounce.read() == LOW || startBounce.read() == LOW) {
    inputMode = KEYBOARD;
    Keyboard.begin();
  } else {
    inputMode = JOYSTICK;
    joystick.setXAxisRange(-1, 1);
    joystick.setYAxisRange(-1, 1);
    joystick.begin(true);
  }
}

void handleYAxis(Bounce yBounce, int yAxis, int keyboardCaracter) {
  // fell = switch closed
  if (yBounce.fell()) {
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(yAxis); 
    } else {
      Keyboard.press(keyboardCharacter);
    }
  } else if (yBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(0); 
    } else {
      Keyboard.release(keyboardCaracter);
    }
  }
}
void handleXAxis(Bounce xBounce, int xAxis, int keyboardCaracter) {
  // fell = switch closed
  if (xBounce.fell()) {
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(xAxis); 
    } else {
      Keyboard.press(keyboardCharacter);
    }
  } else if (xBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(0); 
    } else {
      Keyboard.release(keyboardCaracter);
    }
  }
}

void handleButton(Bounce buttonBounce, int joystickButtonIndex, int keyboardCharacter) {
  // fell = switch closed
  if (buttonBounce.fell()) { 
    if (inputMode == JOYSTICK) { 
      joystick.pressButton(joystickButtonIndex); 
    } else {
      Keyboard.press(keyboardCharacter);
    }
  } else if (buttonBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.releaseButton(joystickButtonIndex); 
    } else {
      Keyboard.release(keyboardCharacter);
    }
  }
}

void loop() {
  upBounce.update();
  downBounce.update();
  leftBounce.update();
  rightBounce.update();
  fireBounce.update();
  selectBounce.update();
  startBounce.update();

  handleYAxis(upBounce,   -1, KEY_UP_ARROW);
  handleYAxis(downBounce,  1, KEY_DOWN_ARROW);
  handleXAxis(leftBounce, -1, KEY_LEFT_ARROW);
  handleXAxis(rightBounce, 1, KEY_RIGHT_ARROW);

  handleButton(fireBounce, JOYSTICK_BUTTON_FIRE, 's');
  handleButton(selectBounce, JOYSTICK_BUTTON_SELECT, 's');
  handleButton(startBounce, JOYSTICK_BUTTON_START, 's');
}
