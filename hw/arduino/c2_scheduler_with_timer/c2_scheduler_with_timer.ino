#include "Button.h"
#include "Blinker.h"
#include <util/atomic.h>

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

  // Timer1 setup
  noInterrupts();            // don't let anything fire while we reconfigure
  // WGM13 = WGM11 = WGM10 = 0, WGM12 = 1 -> CTC (clear timer on compare mode)
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  // CS = 011 -> 1/64 (scaler), 16MHz/64 = 250'000 for 1 second 
  TCCR1B |= (1 << CS11) | (1 << CS10);      
  OCR1A  = 249;                // 250'000/1000 = 250 - 1 (ceiling), for 1kHz i.e. 1ms precisely
  TIMSK1 = (1 << OCIE1A);     // call the ISR on matching counter to OCR1A
  TCNT1 = 0; // start the counter from 0
  interrupts();

  Serial.begin(115200);
}

volatile uint32_t g_ticks{0};
volatile uint8_t g_step_due {10};
volatile bool flag;

// interrupt routine
ISR(TIMER1_COMPA_vect) {       // runs every 1ms, whatever loop() is doing
  static uint8_t sub = 0;   // initialised once, keeps its value between calls
  g_ticks++;
  sub++;
  if (sub == g_step_due){
    flag = true;
    sub = 0;
  }
  
}

uint32_t prev = 0;

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

  // Timer1 interrupts
  if (flag){
    flag = false;
    uint32_t ticks;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { ticks = g_ticks; } // disable interrupt while reading the value
    Serial.println(ticks - prev);
    prev = ticks;
  }
}