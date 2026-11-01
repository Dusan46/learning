
volatile bool flag;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  noInterrupts();            // don't let anything fire while we reconfigure
  // WGM13 = WGM11 = WGM10 = 0, WGM12 = 1 -> CTC (clear timer on compare mode)
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  // CS = 100 -> 1/256 (scaler), timer starts
  TCCR1B |= (1 << CS12);      
  OCR1A  = 62499;                // 16MHz/256 = 62500, compare for exact 1 second
  TIMSK1 = (1 << OCIE1A);     // call the ISR on matching counter to OCR1A
  TCNT1 = 0; // start the counter from 0
  interrupts();


  Serial.begin(115200);
}

ISR(TIMER1_COMPA_vect) {       // runs every 1s, whatever loop() is doing
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  flag = true;
}

void loop() {
  uint32_t now = millis();
  if (flag){
    Serial.println(now);
    flag = false;
  }
}