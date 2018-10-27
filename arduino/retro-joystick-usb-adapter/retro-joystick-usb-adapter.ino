#include <Keyboard.h>

#include <Joystick.h>

#include <Bounce2.h>

// Joystick to pin mapping
#define TAC2_SELECT_PIN 7
#define TAC2_START_PIN  8
#define TAC2_FIRE_PIN   6
#define TAC2_UP_PIN     2
#define TAC2_DOWN_PIN   3
#define TAC2_LEFT_PIN   4
#define TAC2_RIGHT_PIN  5

// Joystick config
#define MIN_JOYSTICK_VALUE -1
#define MAX_JOYSTICK_VALUE 1

// Bounce in milliseconds
#define BOUNCE_INTERVAL                 5
// Longer interval for start and select to prevent accidental exit of emulator
#define BOUNCE_INTERVAL_CONTROL_BUTTONS 700

// Convenience when looping over controls
#define AXIS_COUNT 4
#define BUTTON_COUNT 3

// Index in axisControlBounce array
#define CONTROL_UP_INDEX    0
#define CONTROL_DOWN_INDEX  1
#define CONTROL_LEFT_INDEX  2
#define CONTROL_RIGHT_INDEX 3

// Array used for setting up axis input pins and attach bounce
int axisControlPins[4] = {TAC2_UP_PIN, TAC2_DOWN_PIN, TAC2_LEFT_PIN, TAC2_RIGHT_PIN};
// Bounce for axisControlPins
Bounce axisControlBounce[4] = {Bounce(), Bounce(), Bounce(), Bounce()};

// Array used for setting up button input pins and attach bounce
int buttonPins[3] = {TAC2_FIRE_PIN, TAC2_SELECT_PIN, TAC2_START_PIN};
// Bounce for buttonPins
Bounce buttonBounce[3] = {Bounce(), Bounce(), Bounce()};

// Bounce is a bit different for start and select to prevent accidental exit from emulator
// (start+select exits emulator so we want longer interval)
int axisBounceIntervals[4] = {BOUNCE_INTERVAL, BOUNCE_INTERVAL, BOUNCE_INTERVAL, BOUNCE_INTERVAL};
int buttonBounceIntervals[3] = {BOUNCE_INTERVAL, BOUNCE_INTERVAL_CONTROL_BUTTONS, BOUNCE_INTERVAL_CONTROL_BUTTONS};

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


void setup() {
  Serial.begin(9600);

  // Initialize pins and bounce for axis controls
  for (int cnt=0; cnt < AXIS_COUNT; cnt++) {
    pinMode(axisControlPins[cnt], INPUT_PULLUP);
    axisControlBounce[cnt].attach(axisControlPins[cnt]);
    axisControlBounce[cnt].interval(axisBounceIntervals[cnt]);
  }
  // Initialize pins and bounce for button controls
  for (int cnt=0; cnt < BUTTON_COUNT; cnt++) {
    pinMode(buttonPins[cnt], INPUT_PULLUP);
    buttonBounce[cnt].attach(buttonPins[cnt]);
    buttonBounce[cnt].interval(buttonBounceIntervals[cnt]);
  }
  joystick.setXAxisRange(MIN_JOYSTICK_VALUE, MAX_JOYSTICK_VALUE);
  joystick.setYAxisRange(MIN_JOYSTICK_VALUE, MAX_JOYSTICK_VALUE);
  joystick.begin(true);
}
void loop() {
  for (int cnt=0; cnt < AXIS_COUNT; cnt++) {
    axisControlBounce[cnt].update();
  }
  for (int cnt=0; cnt < BUTTON_COUNT; cnt++) {
    buttonBounce[cnt].update();
    if (buttonBounce[cnt].fell()) {
      //Keyboard.press(' ');
      joystick.pressButton(cnt);
    } else if (buttonBounce[cnt].rose()) {
      //Keyboard.release(' ');
      joystick.releaseButton(cnt);
    }

  }

  if (axisControlBounce[CONTROL_UP_INDEX].fell()) {
    joystick.setYAxis(MIN_JOYSTICK_VALUE);
    //Keyboard.press(KEY_UP_ARROW);
  } else if (axisControlBounce[CONTROL_UP_INDEX].rose()) {
    //Keyboard.release(KEY_UP_ARROW);
    joystick.setYAxis(0);
  }
  if (axisControlBounce[CONTROL_DOWN_INDEX].fell()) {
    //Keyboard.press(KEY_DOWN_ARROW);
    joystick.setYAxis(MAX_JOYSTICK_VALUE);
  } else if (axisControlBounce[CONTROL_DOWN_INDEX].rose()) {
    //Keyboard.release(KEY_DOWN_ARROW);
    joystick.setYAxis(0);
  }
  if (axisControlBounce[CONTROL_LEFT_INDEX].fell()) {
    //Keyboard.press(KEY_LEFT_ARROW);
    joystick.setXAxis(MIN_JOYSTICK_VALUE);
  } else if (axisControlBounce[CONTROL_LEFT_INDEX].rose()) {
    // Keyboard.release(KEY_LEFT_ARROW);
    joystick.setXAxis(0);
  }
  if (axisControlBounce[CONTROL_RIGHT_INDEX].fell()) {
    //Keyboard.press(KEY_LEFT_ARROW);
    joystick.setXAxis(MAX_JOYSTICK_VALUE);
  } else if (axisControlBounce[CONTROL_RIGHT_INDEX].rose()) {
    // Keyboard.release(KEY_LEFT_ARROW);
    joystick.setXAxis(0);
  }
}
