#include "cli_parser.h"

Command parse(const char* line)
{
  char copy[32];
  strncpy(copy, line, sizeof copy);   // line is const
  copy[sizeof copy - 1] = '\0';       // so work on a copy

  Command cmd;

  char* word = strtok(copy, " ");
  if (word == nullptr) {
    cmd.type = CmdType::Error;
    cmd.error = "empty command";
    return cmd;
  }

  if (strcmp(word, "led") == 0) {
    cmd.type = CmdType::Led;
  } else if (strcmp(word, "rate") == 0) {
    cmd.type = CmdType::Rate;
  } else if (strcmp(word, "status") == 0) {
    cmd.type = CmdType::Status;
  } else if (strcmp(word, "help") == 0) {
    cmd.type = CmdType::Help;
  } else {
    cmd.type = CmdType::Error;
    cmd.error = "unknown command";
    return cmd;
  }

  char* arg = strtok(nullptr, " "); // continue where you stopped
  if (arg == nullptr) {
    cmd.type = CmdType::Error;
    cmd.error = "missing argument";
    return cmd;
  }

  char* end;
  long led_number = strtol(arg, &end, 10);
  if (*end != '\0') {
    cmd.type = CmdType::Error;
    cmd.error = "invalid argument";
    return cmd;
  } else if (led_number < 0 || led_number > 3) {
    cmd.type = CmdType::Error;
    cmd.error = "argument out of range";
    return cmd;
  } else {
    cmd.index = static_cast<uint8_t>(led_number);
  }

  if (cmd.type == CmdType::Help || cmd.type == CmdType::Status) {
    // no more arguments expected
    if (strtok(nullptr, " ") != nullptr) {
      cmd.type = CmdType::Error;
      cmd.error = "too many arguments";
      return cmd;
    }
  } else if (cmd.type == CmdType::Led) {
    arg = strtok(nullptr, " ");
    if (arg == nullptr) {
      cmd.type = CmdType::Error;
      cmd.error = "missing argument";
      return cmd;
    }
    if (strcmp(arg, "on") == 0) {
      cmd.on = true;
    } else if (strcmp(arg, "off") == 0) {
      cmd.on = false;
    } else {
      cmd.type = CmdType::Error;
      cmd.error = "invalid argument";
      return cmd;
    }
  } else if (cmd.type == CmdType::Rate) {
    arg = strtok(nullptr, " ");
    if (arg == nullptr) {
      cmd.type = CmdType::Error;
      cmd.error = "missing argument";
      return cmd;
    }
    long ms = strtol(arg, &end, 10);
    if (*end != '\0') {
      cmd.type = CmdType::Error;
      cmd.error = "invalid argument";
      return cmd;
    } else if (ms < 10 || ms > 100'000) {
      cmd.type = CmdType::Error;
      cmd.error = "argument out of range";
      return cmd;
    } else {
      cmd.ms = static_cast<uint16_t>(ms);
    }
  }

  return cmd;
}
