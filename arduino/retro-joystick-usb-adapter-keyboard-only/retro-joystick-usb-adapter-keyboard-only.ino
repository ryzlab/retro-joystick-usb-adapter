// https://github.com/MHeironimus/ArduinoJoystickLibrary

#include <Keyboard.h>
#include <Joystick.h>
#include <Bounce2.h>

// Joystick to pin mapping
#define PIN_UP     2
#define PIN_DOWN   3
#define PIN_LEFT   4
#define PIN_RIGHT  5
#define PIN_FIRE   6
#define PIN_SELECT 7
#define PIN_START  8

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
  fireBounce.interval   (BOU śNCE_INTERVAL);
  selectBounce.interval (BOUNCE_INTERVAL);
  startBounce.interval  (BOUNCE_INTERVAL);

  selectBounce.update();
  startBounce.update();
}

void handleInput(Bounce xBounce, int keyboardCharacter) {
  // fell = switch closed
  if (xBounce.fell()) {
      Keyboard.press(keyboardCharacter);
  } else if (xBounce.rose()) {
      Keyboard.release(keyboardCharacter);
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

  handleInput(upBounce,    KEY_UP_ARROW);
  handleInput(downBounce,  KEY_DOWN_ARROW);
  handleInput(leftBounce,  KEY_LEFT_ARROW);
  handleInput(rightBounce, KEY_RIGHT_ARROW);
  handleInput(fireBounce,  ' ');
  handleInput(fireBounce,  's');
}
