# Hardware dev log

Phase 1 hardware hour, weeks 2–5: one challenge (C1–C12) at a time from
`Phase1_Hardware_Workbook_Weeks2-5.pdf`, written from datasheets instead of
following the ELEGOO lessons (did lessons 1–24 first; too passive).

Entry format: Spec said - Broke - Verified - Learned - Next.
"Broke" is symptom -> cause. "Verified" always has a number.

---

## 2026-10-31 - Exercise 0 - Blinker class
- **Spec:** two LEDs at independent rates, `millis()` only, no `delay()`.
- **Broke:**
  - Both LEDs stuck on -> `on_ = ~on_`: bitwise NOT on a bool gives -1 / -2,
    both convert to `true`. Fix: `!on_`.
  - Both LEDs "on" (actually toggling every loop, dimmer) ->
    `last_toggle_ms_` stored the period instead of `now_ms`, so
    `now - last` stayed >= period forever. Found by tracing values by hand.
- **Verified:** 500 ms and 100 ms half periods visibly independent.
- **Learned:** class/object, public/private, constructor + initializer list,
  `begin()` vs constructor, `const` getters. Rollover-safe timing:
  `now - last >= interval`. `= now` (drifts) vs `+= period` (keeps schedule).

## 2026-10-31 - C1 - Debounced Button class
- **Spec:** Ganssle part 1: bounce lasts ~ms. Internal pull-up: released = HIGH,
    pressed = LOW. Wait-for-stable debouncer, not lockout (lockout accepts noise 
    spikes).
- **Broke:**
  - First `update()` wrote the raw pin into `stable_pressed_` and never
    updated `last_raw_` -> no debouncing. Fix: raw in a local variable,
    one job per member.
  - `pinMode(OUTPUT)` / `INPUT` instead of `INPUT_PULLUP` -> floating input
    (OUTPUT could short the pin if driven HIGH).
  - `counter` declared inside `setup()` -> out of scope in `loop()`.
- **Verified:** 50 presses -> 50 counts at 20 ms; 54 counts at 0 ms.
- **Learned:** edge = change of the *decided* state; edge flags cleared at
  the start of `update()` so every reader in one loop pass sees them;
  counting/printing belongs in the application, not in `Button`.
- **Next:** C2a scheduler.

## 2026-11-01 - C2a - Task scheduler

**Spec:** Run 3 LEDs (2, 5, 11.1 Hz), a 1 s heartbeat and the C1 button
without delay(). One Task struct {last, period, action}, one table, one
loop over the table. Button polled on every loop() pass.

**Broke:**
- Timing block copied into every class (Blinker, SerialP) -> same logic
  three times. Fix: data per task in a struct, logic once in loop().
- SerialP::begin() declared but not defined -> linker: "undefined reference".
  Member typo last_period_ms_ -> compiler: "not declared". Linker errors
  name a function, not a line.
- Blinker.isOn(): called on the class, not an object.
- One switchLight() for three rows: an Action has no parameters, so it
  can't know which row called it -> one function per LED.
- Writing !slow.isOn() without updating on_ -> LED stuck on.
  Fix: Blinker::toggle() keeps its own state.
- `void` in the action slot: a type, not a function.
- tasks[0].update(): a struct has no functions; the loop does the work.
- last = now -> drift: lateness accumulates. Fix: last += period.
- Serial.begin twice: Serial is shared, so it belongs in setup().

**Verified:**
- 50 presses -> 50 counts while all LEDs blink.
- LED rates and heartbeat: by eye only.

**Learned:**
- struct = data, class = data + behaviour.
- A function pointer stores *which* function to call, as data.
- for (Task& t : tasks) visits each row; & = the real row, not a copy.
- .h holds the class, .cpp the member functions; only the .ino gets
  Arduino.h automatically.

## 2026-11-01 - C2b - Timer1 compare-match interrupt (1 s blink)

**Spec:** Blink LED_BUILTIN from a Timer1 interrupt with an empty loop():
first Normal mode (overflow), then exactly 1 s in CTC mode. Registers by
hand from the ATmega2560 datasheet, chapter "16-bit Timer/Counter": Waveform
Generation Mode table, Clock Select table, TCCR1A/TCCR1B/TIMSK1 register
descriptions, vector table in the Interrupts chapter. No TimerOne library.
- Why not millis(): Timer0 overflows every 1.024 ms; millis() adds 1 per
  overflow and jumps by 2 about every 42 overflows to catch up. A
  millis()-scheduled "10 ms" task really runs every 10 or 11 ms.
- rate = 16 MHz / prescaler / (ceiling + 1)
- Normal, /256: 62 500 counts/s, overflow every 65 536 -> 1.049 s. No
  prescaler gives exactly 1 s: the ceiling is fixed at 65535.
- CTC, /256, OCR1A = 62499: counts 0..62499 = 62 500 counts = 1.000 s.
  Ceiling is N - 1 because 0 is counted too.
- Final: TCCR1A = 0; TCCR1B = WGM12 | CS12; OCR1A = 62499;
  TIMSK1 = OCIE1A; ISR(TIMER1_COMPA_vect).

**Broke:**
- Lost in the datasheet -> read it register by register with no idea what
  mattered. Fix: start from what I want (mode, speed, event) and look up
  only those fields. 7 bits out of ~30 mattered.
- Thought COM1A1 (TCCR1A bit 7) enables the comparison -> it only chooses
  what pin OC1A (D11) does on a match. The compare always runs; the
  interrupt is enabled in TIMSK1.
- `TCCR1B = (1 << WGM12);` then `TCCR1B = (1 << CS12);` -> second `=`
  replaced the whole byte, WGM12 gone, back in Normal mode. Fix: `|=`.
- 62499 written into TCNT1 -> that's the counter, so it only set the
  starting point. The ceiling lives in OCR1A.
- Kept TOIE1 / TIMER1_OVF_vect in CTC -> counter resets at OCR1A, never
  reaches 65535, overflow never fires. Fix: OCIE1A + TIMER1_COMPA_vect.
- The broken version would still have blinked at ~1.049 s (first overflow
  after 3037 counts, then every 65 536). 5 % is invisible by eye -> measure.

**Verified:**
- ISR sets a volatile flag; loop() prints millis() when it sees it.
- 25 readings 999 -> 24999: 24 intervals, 24 000 ms = 1000 ms average.
- Single intervals 999-1001 ms (14999 -> 16000 -> 16999): millis()
  granularity (lags 0 to ~1 ms, jumps by 2 to catch up), not Timer1.
  The wobble is in the ruler, not in what's measured.
- Both timers run from the same 16 MHz clock: this checks the
  configuration, not the crystal.

**Learned:**
```
16 MHz -> prescaler (CS) -> TCNT1 counts 0, 1, 2 ... ceiling -> back to 0
                                                        |
                                    flag -> if enabled in TIMSK1 -> ISR
```
- Three decisions: how fast (CS bits; CS = 000 = stopped, setting CS
  starts it), where the ceiling is (WGM: Normal = 65535, CTC = OCR1A),
  which event calls my code (TIMSK1 bit + matching vector name).
- Configure once in setup(); the ISR runs whatever loop() does.
- ISR can fire between any two instructions: keep it tiny, no Serial.
- volatile: shared with an ISR; the compiler must not keep a stale copy.
- TCCR1A / TCCR1B = control byte 1 and 2; the A/B is NOT channel A.
  TCCR1A: COM (pins) + WGM11..10. TCCR1B: input capture + WGM13..12 + CS.
  WGM is split: take 4 bits from the mode table, put each where it lives.
- Bit names are positions: CS12 = 2, (1 << CS12) = 0b00000100.
  `|` combines bits, `=` replaces the register, `|=` adds to it.
- First write with `=`: the Arduino core set Timer1 up for PWM before
  setup(). Start the clock (CS) last.
- avr-libc only gives names (TCCR1B = 0x81, CS12 = 2, ISR() macro).

**Next:** C2 = C2a scheduler + C2b timer.

## 2026-11-01 - C2 - Scheduler + Timer1 tick (C2a + C2b combined)

**Spec:** Keep the C2a task table (LEDs, heartbeat, button on millis()) and
add a second lane driven by Timer1: CTC, /64, OCR1A = 249 -> 1 kHz tick.
ISR: g_ticks++ (never reset, a precise ms counter) and a sub-counter that
sets a flag every 10th tick. loop() clears the flag and runs the 100 Hz
step. Workbook done-when: 10.00 ms period, jitter, step execution time.
- 16 MHz / 64 = 250 000 counts/s; 250 counts per tick -> ceiling 249.
- Two lanes in loop(): soft timing (millis) for LEDs/heartbeat/button,
  hard timing (Timer1) for the future PI loop.

**Broke:**
- OCR1A = 25000 -> 10 Hz, not 1 kHz, and ignored ceiling = N - 1.
- Reset g_ticks every 10 ticks -> it stopped being a ms count. Fix: two
  counters, g_ticks (never reset) + static uint8_t sub (0..9).
- Flag never cleared -> after the first 10 ms the lane-2 block ran on every
  loop() pass, printing a flood of 0s and 1s. Fix: clear it first thing.
- Read g_ticks twice (print, then prev = g_ticks) -> two different values
  possible. Fix: one atomic read into a local, use only the local.

**Verified:**
- Lane 2 prints ticks per step: 10 on every line.
- Period, jitter, step time: NOT MEASURED YET (see Next).

**Learned:**
- Torn read: g_ticks is 4 bytes and the AVR loads one byte at a time, so
  the ISR can fire between loads. At 255 -> 256 (0x000000FF -> 0x00000100)
  loop() can assemble 0x000001FF = 511: half old, half new value.
- It's rare (needs the ISR in that window and a carry into the next byte),
  so testing almost never shows it. That's what makes it dangerous.
- ATOMIC_BLOCK(ATOMIC_RESTORESTATE) = interrupts off, copy, restore. No tick
  is lost: a pending interrupt runs right after.
- Only multi-byte variables shared with an ISR need this. A bool is one
  byte and can't tear. Variables shared only between loop() functions need
  neither volatile nor atomic.
- static local: initialised once, keeps its value between calls; a global
  only that function can see.
- A 1 ms tick is a general timebase: other rates can be derived from it.

**Next:**
- C2 measurement when the logic analyzer arrives: capture pin 9, period,
  jitter (min/max), high time. If it hasn't arrived by C10, use the
  micros() version instead.

  ## 2026-11-02 - C3a - line reader

**Spec:** Read serial input without blocking. LineReader collects chars
into a 32-byte buffer; '\r' or '\n' ends a line; too long -> ERR overflow;
empty lines ignored. Decisions: led n off/on holds until rate; rate in
half periods, 10..10000 ms; execute() checks n; lower case only, extra
spaces OK; empty lines ignored.

**Broke:**
- Lane 2 printed 100 lines/s -> moved tick count into the heartbeat,
  lane 2 now only runs controlStep().
- Both NL & CR gave an extra empty line -> ignore empty lines.

**Verified:**
- Heartbeat: 1000
- Echo correct with Newline, CR, Both; nothing with No line ending
  until a line end arrives.
- 40-char line -> ERR overflow, next line correct.
- Button: 10 presses -> 10 counts during typing.

**Learned:**
- NL = '\n' (10), CR = '\r' (13); terminals disagree on which they send.
- A C string ends with '\0'; 32-byte buffer = 31 chars + terminator.
- Reset at the start of the next feed(), so the line stays readable.

**Next:** parser (parse(const char*) -> Command), then execute().

## 2026-11-03 - C3b - parser, first draft

**Spec:** parse(const char*) -> Command with no Arduino calls, so it
can be unit-tested on the Mac. Tokens with strtok, commands with
strcmp, numbers with strtol + end pointer check.

**Done:**
- parse() stub wired into lane 3: ERR <reason> or OK.
- First full parse(): led, rate, status, help, with error reasons.

**Broke (known, not fixed yet):**
- help/status read an LED index -> "help" gives ERR missing argument.
- "too many arguments" only checked for help/status -> "led 1 on x" accepted.
- cli_parser.cpp needs its own <string.h> and <stdlib.h>.

**Learned:**
- enum class: names stay scoped (CmdType::Led), no silent int conversion.
- A function returning a struct: store it, then read its fields.
- strtok writes into the string, so parse a copy; strcmp == 0 means equal.

**Next:** restructure (help/status first, extra-args check at the end),
makeError() helper, Command cmd{}; then the test table.

## 2026-11-06 - C3c - parser fixed, Mac tests

**Spec:** parse() unit-tested on the Mac with GoogleTest; same .cpp
compiled for the Mega and for the Mac.

**Broke:**
- help/status read an LED index -> moved them before the index read.
- Extra-args check only in led/rate -> "status 3" accepted. One check
  at the end now.
- "rate 1": strtol(nullptr) -> would segfault on the Mac.
- ms max 100000 in a uint16_t (max 65535) -> 70000 wrapped to 4464.

**Verified:** ctest 11/11 passed.

**Learned:**
- Unit test: fixed input -> parse() -> check fields; GoogleTest gives
  TEST, EXPECT_EQ, EXPECT_STREQ and main().
- EXPECT_STREQ for C strings: EXPECT_EQ would compare pointers.

**Next:** PlatformIO + C7.