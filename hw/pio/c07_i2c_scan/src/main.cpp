#include <Arduino.h> // the Arduino IDE added this line automatically,
                     // but PlatformIO does not!
#include <Wire.h>

// The IDE generated forward declarations for .ino files automatically,
// but PlatformIO does not, so we need to forwarde declare it ourselves.
void updateLed(uint32_t now_ms);
void scanBus();
void printHex2(uint8_t v);
uint8_t probe(uint8_t addr);
const __FlashStringHelper *wireErrorText(uint8_t code);

constexpr uint32_t kHalfPeriodMs = 500;
constexpr uint8_t kUnknown = 6;  // index for codes > 5

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // must match monitor_speed in platformio.ini
  Serial.println(F("c07_i2c_scan: PlatformIO build OK")); // check

  // I2C
  Wire.begin();
  Wire.setWireTimeout(3000, true);
  scanBus();
}

void loop() { updateLed(millis()); }

void updateLed(uint32_t now_ms) {
  static uint32_t last_toggle_ms = 0; // static: keeps its value between calls
  static bool led_on = false;
  if (now_ms - last_toggle_ms >= kHalfPeriodMs) { // rollover-safe, as in C1
    last_toggle_ms = now_ms;
    led_on = !led_on;
    digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);
  }
}

void scanBus() {
  uint16_t counts[kUnknown + 1]{};

  for (uint8_t addr = 0x08; addr <= 0x77; ++addr) {
    uint8_t code = probe(addr);
    uint8_t idx = (code <= 5) ? code : kUnknown;   // bounds guard
    ++counts[idx];

    if (code == 0) {                 // a device answered
      Serial.print(F("  "));
      printHex2(addr);
      Serial.println(F("  ACK"));
    }
  }

  // summary: one line per code that occurred
  for (uint8_t c = 0; c <= kUnknown; ++c) {
    if (counts[c] == 0) { continue; }
    Serial.print(wireErrorText(c));
    Serial.print(F(": "));
    Serial.println(counts[c]);
  }
}

void printHex2(uint8_t v) {
  Serial.print(F("0x"));
  if (v < 16) { Serial.print(F("0")); }
  Serial.print(v, HEX);
}

uint8_t probe(uint8_t addr) {
  Wire.beginTransmission(addr);
  uint8_t code = Wire.endTransmission();
  return code;
}

const __FlashStringHelper *wireErrorText(uint8_t code) {
  switch (code) {
  case 0:
    return F("OK");
  case 1:
    return F("Data too long for buffer");
  case 2:
    return F("Received NACK on transmit of address");
  case 3:
    return F("Received NACK on transmit of data");
  case 4:
    return F("Other error");
  case 5:
    return F("Timeout");
  default:
    return F("Unknown error code");
  }
}