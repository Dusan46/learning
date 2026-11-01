# Learning: C++ and embedded hardware

My learning repo alongside my PhD, as preparation for robotics software work.
The main project (a ROS 2 ground robot) will live in its own repository.

## Structure

- `cpp/`: exercises from [learncpp.com](https://www.learncpp.com), one folder per chapter.
- `hw/`: embedded C++ on an Arduino Mega 2560, written from datasheets
  rather than copied from kit examples.
  - `hw/arduino/`: one sketch per challenge (`c0_blinker`, `c1_button_debouncer`, ...)
  - `hw/DEVLOG.md`: what I built each session, what broke, and how I verified it.

## Building

C++ exercises (CMake):

    cd cpp
    cmake -S . -B build -G Ninja
    cmake --build build

Arduino sketches: open the `.ino` file in the Arduino IDE, board
"Arduino Mega or Mega 2560". From challenge C7 on, sketches are PlatformIO
projects: `pio run -t upload` inside the challenge folder.