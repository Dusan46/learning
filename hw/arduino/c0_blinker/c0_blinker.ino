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

void Blinker::begin() {  // "Blinker::" = this is Blinker's begin()
  pinMode(pin_, OUTPUT);
}

void Blinker::update(uint32_t now_ms) {
  // if half_period_ms_ has passed since the last toggle,
  // flip on_, write it to the pin, and remember when you did it.
  if (now_ms - last_toggle_ms_ >= half_period_ms_) {
    on_ = !on_;
    last_toggle_ms_ = now_ms;
    digitalWrite(pin_, on_);
    // Serial.println(last_toggle_ms_);     // temporarily
  }
}

Blinker slow(LED_BUILTIN, 500);  // two objects, same blueprint, independent state
Blinker fast(8, 100);            // LED + 220 Ohm resistor on pin 8

void setup() {
  slow.begin();
  fast.begin();
  // Serial.begin(9600);
}

void loop() {
  uint32_t now = millis();
  slow.update(now);
  fast.update(now);
}