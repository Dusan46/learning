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

## 2026-11-1 - C2a - Task scheduler

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
