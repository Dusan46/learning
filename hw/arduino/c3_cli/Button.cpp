#include "Button.h"

void Button::begin() {  // "Button::" = this is Button's begin()
  pinMode(pin_, INPUT_PULLUP);
  last_raw_ = (digitalRead(pin_) == LOW);  // LOW means pressed
  stable_pressed_ = last_raw_;             // start from the real state: no fake edge at power-up
  last_change_ms_ = millis();
}

void Button::update(uint32_t now_ms) {
  // 1. clear both edge flags
  fell_ = false;
  rose_ = false;

  // 2. read the pin into a local variable
  bool raw = (digitalRead(pin_) == LOW);

  if (raw != last_raw_) {
    // 3. the pin moved: remember the new raw value and the time
    last_raw_ = raw;
    last_change_ms_ = now_ms;
  } else if (now_ms - last_change_ms_ >= debounce_ms_ && raw != stable_pressed_) {
    // 4. accept: update stable_pressed_, then set fell_ OR rose_ depending on direction
    stable_pressed_ = raw;
    if (stable_pressed_) {
      fell_ = true;
    } else {
      rose_ = true;
    }
  }
}