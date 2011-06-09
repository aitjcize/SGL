#include "logging.h"

#include <stdio.h>
#include <stdarg.h>

void _sgl_debug(struct sgl_context* ctx, const char* fmtstr, ...)
{
  fprintf(stderr, "**error** ");
  va_list args;
  va_start(args, fmtstr);
  vfprintf(stderr, fmtstr, args);
  va_end(args);
}

void _sgl_error(struct sgl_context* ctx, GLenum error, const char* fmtstr, ...)
{
  fprintf(stderr, "**error** ");
  va_list args;
  va_start(args, fmtstr);
  vfprintf(stderr, fmtstr, args);
  va_end(args);
}
