#include "logger.h"
//TODO: rewrite to some fancy streams later
#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#include <Windows.h>
#endif
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
#ifdef _WIN32
  char dest[2048];
  vsprintf(dest, format, argp);
  std::string st = dest;
  st += "\n";
  OutputDebugStringA(st.c_str());
#else
  vfprintf(stderr, format, argp);
#endif
  va_end(argp);
  fprintf(stderr, "\n");
}

}
