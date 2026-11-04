#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum class CmdType : uint8_t { Led, Rate, Status, Help, Error };

struct Command {
  CmdType type;
  uint8_t index;       // led/rate: which LED
  bool on;             // led: on or off
  uint16_t ms;         // rate: the time value
  const char* error;   // Error: the reason, e.g. "unknown command"
};

Command parse(const char* line);