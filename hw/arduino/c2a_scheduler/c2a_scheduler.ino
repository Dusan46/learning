#include "Button.h"
#include "Blinker.h"

// Serial print
class SerialP {
public:                                           // anyone can call these
  SerialP(uint32_t period)   // constructor: runs when the object is created
    : period_(period)  // initializer list: sets the members
  {}

  void begin();  // declared here, defined below
  void update(uint32_t now_ms);

private:         // only the class's own functions can touch these
  uint32_t period_;
  uint32_t last_sent_ms_ = 0;  // default value
};

void SerialP::begin() {  
  Serial.begin(115200);
}

void SerialP::update(uint32_t now_ms) {
  // if period has passed, print out the message
  if (now_ms - last_sent_ms_ >= period_) {
    last_sent_ms_ = now_ms;
    Serial.println("A second has passed");
  }
}

Button button(2, 20);            // pin 2, delay of 20ms
uint16_t counter{ 0 };
bool led_on{ false };

Blinker slow(6, 250);            // LED + 220 Ohm resistor on pin 6, 2Hz -> half period of 250ms
Blinker medium(7, 100);          // LED + 220 Ohm resistor on pin 7, 5Hz -> half period of 100ms
Blinker fast(8, 45);             // LED + 220 Ohm resistor on pin 8, ~11Hz -> half period of ~45ms

SerialP print_sec(1000);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // to track when the buttom is pushed
  Serial.begin(115200);
  button.begin();
  slow.begin();
  medium.begin();
  fast.begin();
  print_sec.begin();
}

void loop() {
  uint32_t now = millis();
  // LED blink
  slow.update(now);
  medium.update(now);
  fast.update(now);

  // Button pressed
  button.update(now);
  if (button.fell()) {
    led_on = !led_on;
    digitalWrite(LED_BUILTIN, led_on);
    counter = counter + 1;
    Serial.println(counter);
  }

  // Serial message
  print_sec.update(now);

}