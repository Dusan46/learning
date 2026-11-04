#include "cli_parser.h"

Command parse(const char* line)
{
  Command cmd;
  cmd.type = CmdType::Error;
  cmd.error = "not implemented";

  return cmd;
}