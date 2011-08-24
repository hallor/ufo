#include "logger.h"
//TODO: rewrite to some fancy streams later
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

Logger log;

Logger::Logger()
{
}

void Logger::log(const char *prefix, const char * format,...)
{
  va_list argp;
  fprintf(stderr, "%s", prefix);
  va_start(argp, format);
  vfprintf(stderr, format, argp);
  va_end(argp);
  fprintf(stderr, "\n");
}

