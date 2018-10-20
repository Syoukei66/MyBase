#include "LoggerBuilder.h"
#include <iostream>
#include <fstream>

static const std::string LOGGER_PATH = "Engine/Core/NativeLogger.h";
static const std::string ASSERT_NAME = "Assert";

std::string LoggerBuilder::CreateEnums()
{
  std::string ret;
  for (const LevelData& level : this->levels_)
  {
    ret += "  " + level.name + ",\n";
  }
  ret += "  " + ASSERT_NAME + "\n";
  return ret;
}

std::string LoggerBuilder::CreateNames()
{
  std::string ret;
  for (const LevelData& level : this->levels_)
  {
    ret += "  \"" + level.name + "\",\n";
  }
  ret += "  \"" + ASSERT_NAME + "\"\n";
  return ret;
}

std::string LoggerBuilder::CreateLevelMethods(const std::string& name, DoLevel do_level, bool break_flag)
{
  std::string macro_head;
  std::string macro_foot;
  std::string logging_body;
  std::string format_logging_body;

  if (do_level != DoLevel::NOT_DO)
  {
    logging_body = "  Out(Level::" + name + ", message);\n";
    format_logging_body = "  Out(Level::" + name + ", message, args ...);\n";
  }
  if (break_flag)
  {
    logging_body += "  __debugbreak();\n";
  }
  if (do_level == DoLevel::ON_DEBUG)
  {
    macro_head = "#ifdef _DEBUG\n";
    macro_foot = "#endif\n";
  }

  return
R"(
FORCE_INLINE void )" + name + R"((const char* message)
{)"
+ "\n" + macro_head + logging_body + macro_foot +
R"(}
template <typename ... Args>
FORCE_INLINE void )" + name + R"((const char* message, const char* format, Args const & ... args)
{)"
+ "\n" + macro_head + logging_body + macro_foot +
R"(}
)";
}

std::string LoggerBuilder::CreateMethods()
{
  std::string ret;
  for (const LevelData& level : this->levels_)
  {
    ret += CreateLevelMethods(level.name, level.do_level, level.break_flag);
  }
  ret += CreateLevelMethods(ASSERT_NAME, DoLevel::ON_DEBUG, true);
  return ret;
}

void LoggerBuilder::CreateHeader()
{
  std::string header;

  header +=
R"(// auto generated by LoggerBuilder
#pragma once

#include <string.h>
#include <stdio.h>

#include "Macro.h"

namespace Log
{

enum class Level
{
)" 
+ CreateEnums() + 
R"(};

static const char* LEVEL_NAMES[] =
{
)" 
+ CreateNames() + 
R"(};

void Out(Level level, const char* message);

template <typename ... Args>
FORCE_INLINE void Out(Level level, const char* message, const char* format, Args const & ... args)
{
  char buf[1024];
  sprintf(buf, format, message, args ...);
  Out(level, buf);
}
)" + CreateMethods() + R"(
} // namespace Log
)";

  std::ofstream ofs(LOGGER_PATH);
  ofs << header;
  ofs.close();

  std::cout << "//==================================================" << std::endl;
  std::cout << "// " << ASSERT_NAME << std::endl;
  std::cout << "//==================================================" << std::endl;
  std::cout << header;
}
