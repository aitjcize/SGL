/**
 * @file   viewport.h
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

#ifndef __SGL_VIEWPORT_H__
#define __SGL_VIEWPORT_H__

#include "sglheader.h"
#include "context.h"

extern void _sgl_init_viewport(struct sgl_context *ctx);

extern void _sgl_free_viewport_data(struct sgl_context *ctx);

extern void _sgl_set_viewport(struct sgl_context *ctx, GLint x, GLint y,
                              GLsizei width, GLsizei height);

extern void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

extern void glDepthRange(GLclampd nearval, GLclampd farval);

extern void glDepthRangef(GLclampf nearval, GLclampf farval);

#endif /* __SGL_VIEWPORT_H__ */
