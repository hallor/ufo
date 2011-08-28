#include "logger.h"
//TODO: rewrite to some fancy streams later
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <string>
namespace Logger {
Logger __log;

Logger::Logger()
{
}

void Logger::log(const char *prefix, const char * format,...)
{
  va_list argp;
  fprintf(stderr, "%s", prefix);
  va_start(argp, format);
  char dest[2048];
  vsprintf(dest, format, argp);
  std::string st = dest;
  st += "\n";
  OutputDebugStringA(st.c_str());
  vfprintf(stderr, format, argp);
  va_end(argp);
  fprintf(stderr, "\n");
}

};
