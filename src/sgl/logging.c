/**
 * @file   logging.c
 *
 * Copyright (C) 2011 - SGL Authors <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
