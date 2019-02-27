// https://github.com/MHeironimus/ArduinoJoystickLibrary

#include <Keyboard.h>

#include <Joystick.h>

#include <Bounce2.h>

#define JOYSTICK 1
#define KEYBOARD 2

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

void loop() {
  upBounce.update();
  downBounce.update();
  leftBounce.update();
  rightBounce.update();
  fireBounce.update();
  selectBounce.update();
  startBounce.update();

  // fell = switch closed
  if (upBounce.fell()) {
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(-1); 
    } else {
      Keyboard.press(KEY_UP_ARROW);
    }
  } else if (upBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(0); 
    } else {
      Keyboard.release(KEY_UP_ARROW);
    }
  }
  if (downBounce.fell()) { 
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(1); 
    } else {
      Keyboard.press(KEY_DOWN_ARROW);
    }
  } else if (downBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setYAxis(0); 
    } else {
      Keyboard.release(KEY_DOWN_ARROW);
    }
  }
  if (leftBounce.fell()) { 
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(-1); 
    } else {
      Keyboard.press(KEY_LEFT_ARROW);
    }
  } else if (leftBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(0); 
    } else {
      Keyboard.release(KEY_LEFT_ARROW);
    }
  }
  if (rightBounce.fell()) { 
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(1); 
    } else {
      Keyboard.press(KEY_RIGHT_ARROW);
    }
  } else if (rightBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.setXAxis(0); 
    } else {
      Keyboard.release(KEY_RIGHT_ARROW);
    }
  }

  // 0 = fire
  // 1 = select
  // 2 = start
  if (fireBounce.fell()) { 
    if (inputMode == JOYSTICK) { 
      joystick.pressButton(0); 
    } else {
      Keyboard.press('s');
    }
  } else if (fireBounce.rose()) {
    if (inputMode == JOYSTICK) { 
      joystick.releaseButton(0); 
    } else {
      Keyboard.release('s');
    }
  }
  if (selectBounce.fell()) { 
    joystick.pressButton(1); 
  } else if (selectBounce.rose()) {
    joystick.releaseButton(1); 
  }
  if (startBounce.fell()) { 
    joystick.pressButton(2); 
  } else if (startBounce.rose()) {
    joystick.releaseButton(2); 
  }
}
