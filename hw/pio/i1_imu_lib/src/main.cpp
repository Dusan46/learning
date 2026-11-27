// I1 - read the MPU-6500 by its registers and print physical units.
// Adafruit's MPU6050 library rejects this chip (WHO_AM_I is 0x70, not 0x68),
// so this does the same register reads by hand.
#include <Arduino.h>
#include <Wire.h>

constexpr uint8_t kAddr = 0x68;  // I2C address; 0x69 if AD0 is high

bool writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(kAddr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;  // 0 = success
}

// Write the register address, then read n bytes starting there.
bool readRegs(uint8_t reg, uint8_t* buf, uint8_t n) {
  Wire.beginTransmission(kAddr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) {  // false = repeated start
    return false;
  }
  if (Wire.requestFrom(kAddr, n) != n) {
    return false;
  }
  for (uint8_t i = 0; i < n; ++i) {
    buf[i] = Wire.read();
  }
  return true;
}

// The data registers are big-endian: high byte first.
int16_t be16(const uint8_t* p) {
  return static_cast<int16_t>(static_cast<uint16_t>(p[0]) << 8 | p[1]);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("I1: MPU-6500 by registers"));
  Wire.begin();
  Wire.setClock(400000);
  Wire.setWireTimeout(3000, true);  // a stuck bus becomes an error, not a hang

  uint8_t id = 0;
  if (!readRegs(0x75, &id, 1)) {  // WHO_AM_I
    Serial.println(F("No answer at 0x68: check wiring and address"));
    while (true) {
    }
  }
  Serial.print(F("WHO_AM_I 0x"));
  Serial.println(id, HEX);  // 0x70 = MPU-6500

  writeReg(0x6B, 0x01);  // PWR_MGMT_1: awake, best clock (PLL)
  delay(100);            // let the gyro clock settle
  writeReg(0x1A, 0x03);  // CONFIG: gyro low-pass ~41 Hz
  writeReg(0x1D, 0x03);  // ACCEL_CONFIG2: accel low-pass ~45 Hz
  writeReg(0x1B, 0x08);  // GYRO_CONFIG: +/-500 deg/s
  writeReg(0x1C, 0x08);  // ACCEL_CONFIG: +/-4 g
}

void loop() {
  static uint32_t last = 0;
  const uint32_t now = millis();
  if (now - last < 200) {  // print 5 times a second, without delay()
    return;
  }
  last = now;

  uint8_t b[14];
  const uint32_t t0 = micros();
  const bool ok = readRegs(0x3B, b, 14);  // ACCEL_XOUT_H: 14 bytes
  const uint32_t readUs = micros() - t0;
  if (!ok) {
    Serial.println(F("read error"));
    return;
  }

  float a[3];
  float w[3];
  for (uint8_t i = 0; i < 3; ++i) {
    a[i] = be16(&b[2 * i]) / 8192.0f;    // +/-4 g: 8192 LSB/g
    w[i] = be16(&b[8 + 2 * i]) / 65.5f;  // +/-500 deg/s: 65.5 LSB/(deg/s)
  }
  const float norm = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
  const float tempC = be16(&b[6]) / 333.87f + 21.0f;  // MPU-6500 formula

  Serial.print(F("a[g] "));
  for (float v : a) {
    Serial.print(v, 3);
    Serial.print(' ');
  }
  Serial.print(F(" |a| "));
  Serial.print(norm, 3);
  Serial.print(F("  w[deg/s] "));
  for (float v : w) {
    Serial.print(v, 2);
    Serial.print(' ');
  }
  Serial.print(F(" T[C] "));
  Serial.print(tempC, 1);
  Serial.print(F("  read[us] "));
  Serial.println(readUs);
}