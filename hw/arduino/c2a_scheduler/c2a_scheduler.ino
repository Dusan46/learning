#include "Button.h"
#include "Blinker.h"

Button button(2, 20);  // pin 2, delay of 20ms
uint16_t counter{ 0 };
bool led_on{ false };

Blinker slow(6);    // LED + 220 Ohm resistor on pin 6, 2Hz -> half period of 250ms
Blinker medium(7);  // LED + 220 Ohm resistor on pin 7, 5Hz -> half period of 100ms
Blinker fast(8);     // LED + 220 Ohm resistor on pin 8, ~11Hz -> half period of ~45ms

// Making Task struct: blinking and printing
void toggle2Hz() {
  slow.toggle();
}
void toggle5Hz() {
  medium.toggle();
}
void toggle11Hz() {
  fast.toggle();
}
void printOneSec() {
  Serial.println("A second has passed");
}
using Action = void (*)();
// Sturctor as the printint and blinking (actions) happen by same procedure, 
// compare current time - previous time to the half period time 
struct Task {
  uint32_t last_time;
  uint32_t half_period_time;
  Action action;
};
Task tasks[] = {
  { 0, 250, toggle2Hz },     // 2Hz
  { 0, 100, toggle5Hz },     // 5Hz
  { 0, 45, toggle11Hz },     // ~11Hz
  { 0, 1000, printOneSec },  // print each second
};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // to track when the buttom is pushed
  button.begin();

  slow.begin();
  medium.begin();
  fast.begin();

  Serial.begin(115200);
}



void loop() {
  uint32_t now = millis();
  // LED blink & print
  for (Task& t : tasks) {
    if (now - t.last_time >= t.half_period_time) {
      t.last_time = t.last_time + t.half_period_time;
      t.action();
    }
  }


  // Button pressed
  button.update(now);
  if (button.fell()) {
    led_on = !led_on;
    digitalWrite(LED_BUILTIN, led_on);
    counter = counter + 1;
    Serial.println(counter);
  }
}