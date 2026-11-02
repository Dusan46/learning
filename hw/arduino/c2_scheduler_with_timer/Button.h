#pragma once
#include <Arduino.h>

// Button class
class Button {
public:                                      // anyone can call these
  Button(uint8_t pin, uint32_t debounce_ms)  // constructor: runs when the object is created
    : pin_(pin), debounce_ms_(debounce_ms)   // initializer list: sets the members
  {}

  void begin();  // declared here, defined below
  void update(uint32_t now_ms);
  // getter: reports the values if needed
  bool fell() const {
    return fell_;
  }
  bool rose() const {
    return rose_;
  }
  bool isPressed() const {
    return stable_pressed_;
  }

private:  // only the class's own functions can touch these
  uint8_t pin_;
  uint32_t debounce_ms_;
  bool stable_pressed_{ false };  // what we've decided
  bool last_raw_{ false };        // what the pin said on the previous update()
  uint32_t last_change_ms_{ 0 };  // when the raw reading last changed
  bool fell_{ false };            // edge flags
  bool rose_{ false };
};