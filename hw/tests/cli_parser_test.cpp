#include <gtest/gtest.h>
#include "cli_parser.h"

TEST(Parse, LedOn) {
  Command c = parse("led 1 on");
  EXPECT_EQ(c.type, CmdType::Led);
  EXPECT_EQ(c.index, 1);
  EXPECT_TRUE(c.on);
}

TEST(Parse, LedExtraWord) {
  Command c = parse("led 1 on extra");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "too many arguments");
}

TEST(Parse, Rate) {
  Command c = parse("rate 1 250");
  EXPECT_EQ(c.type, CmdType::Rate);
  EXPECT_EQ(c.index, 1);
  EXPECT_EQ(c.ms, 250);
}

TEST(Parse, Status) {
  Command c = parse("status");
  EXPECT_EQ(c.type, CmdType::Status);
}

TEST(Parse, Help) {
  Command c = parse("help");
  EXPECT_EQ(c.type, CmdType::Help);
}

TEST(Parse, StatusExtraArg) {
  Command c = parse("status 3");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "too many arguments");
}

TEST(Parse, RateMissArg) {
  Command c = parse("rate 1");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "missing argument");
}

TEST(Parse, RateArgOutOfRange) {
  Command c = parse("rate 1 0");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "argument out of range");
}

TEST(Parse, RateArgInvalid) {
  Command c = parse("rate 1 12x");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "invalid argument");
}

TEST(Parse, Foo) {
  Command c = parse("foo");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "unknown command");
}

TEST(Parse, Empty) {
  Command c = parse("");
  EXPECT_EQ(c.type, CmdType::Error);
  EXPECT_STREQ(c.error, "empty command");
}