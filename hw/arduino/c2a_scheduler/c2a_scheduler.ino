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

// Blink a LED
class Blinker {
public:                                           // anyone can call these
  Blinker(uint8_t pin, uint32_t half_period_ms)   // constructor: runs when the object is created
    : pin_(pin), half_period_ms_(half_period_ms)  // initializer list: sets the members
  {}

  void begin();  // declared here, defined below
  void update(uint32_t now_ms);
  bool isOn() const {
    return on_;
  }  // 'const' = promises not to change the object

private:         // only the class's own functions can touch these
  uint8_t pin_;  // trailing _ marks a member (just a convention)
  uint32_t half_period_ms_;
  uint32_t last_toggle_ms_ = 0;  // default value
  bool on_ = false;
};

void Blinker::begin() {  
  pinMode(pin_, OUTPUT);
}

void Blinker::update(uint32_t now_ms) {
  // if half_period_ms_ has passed since the last toggle,
  // flip on_, write it to the pin, and remember when you did it.
  if (now_ms - last_toggle_ms_ >= half_period_ms_) {
    on_ = !on_;
    last_toggle_ms_ = now_ms;
    digitalWrite(pin_, on_);
  }
}

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