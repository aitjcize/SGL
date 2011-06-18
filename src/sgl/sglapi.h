/**
 * @file   sglapi.h
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

#ifndef __SGL_SGLAPI_H__
#define __SGL_SGLAPI_H__

extern struct sgl_context _g_sgl_context;

#define GET_CURRENT_CONTEXT(C) \
  struct sgl_context* C = (struct sgl_context*) &_g_sgl_context

extern void sglInit(int w, int h);

extern void sglPipelineIter(char* buffer);

#endif /* __SGL_SGLAPI_H__ */
