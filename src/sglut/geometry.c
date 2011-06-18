/**
 * @file   geometry.c
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

#include "gl.h"

#include <math.h>
#include <stdlib.h>

#define RAD(a) (a * M_PI / 180.0)

void _sglut_sphere(GLdouble radius, GLint stacks, GLint slices, GLenum type)
{
  int i = 0, j = 0;
  int num_vertices = (stacks + 1) * slices;
  int num_quads = stacks * slices;

  GLdouble* vert = (GLdouble*)malloc(num_vertices * 3 * sizeof(GLdouble));
  GLushort* idices = (GLushort*)malloc(num_quads * 4 * sizeof(GLushort));

  double theta_total = 180;
  double phi_total = 360;
  double theta_inc = theta_total / stacks;
  double phi_inc = phi_total / slices;

  double theta = 0;
  double phi = 0;
  int idx = 0, base = 0;

  for (i = 0; i <= stacks; ++i) {
    for (j = 0; j < slices; ++j) {
      vert[idx++] = sin(RAD(theta)) * cos(RAD(phi));
      vert[idx++] = cos(RAD(theta));
      vert[idx++] = sin(RAD(theta)) * sin(RAD(phi));
      phi += phi_inc;
    }
    theta += theta_inc;
  }

  idx = 0;
  for (i = 0; i < stacks; ++i) {
    for (j = 0; j < slices; ++j) {
      if (j == slices - 1) {
        idices[idx++] = base;
        idices[idx++] = base - slices + 1;
        idices[idx++] = base + 1;
        idices[idx++] = base + slices;
      } else {
        idices[idx++] = base;
        idices[idx++] = base + 1;
        idices[idx++] = base + slices + 1;
        idices[idx++] = base + slices;
      }
      ++base;
    }
  }

  if (type == GL_FILL) {
    glBegin(GL_QUADS);
    for (i = 0; i < stacks * slices * 4; ++i) {
      j = idices[i];
      glVertex3f(vert[j*3+0] *radius, vert[j*3+1] *radius, vert[j*3+2] *radius);
      glNormal3f(vert[j*3+0], vert[j*3+1], vert[j*3+2]);
    }
    glEnd();
  } else {
    for (i = 0; i < stacks * slices; ++i) {
      glBegin(GL_LINE_LOOP);
      j = idices[i * 4 + 0];
      glVertex3f(vert[j*3+0] *radius, vert[j*3+1] *radius, vert[j*3+2] *radius);
      glNormal3f(vert[j*3+0], vert[j*3+1], vert[j*3+2]);
      j = idices[i * 4 + 1];
      glVertex3f(vert[j*3+0] *radius, vert[j*3+1] *radius, vert[j*3+2] *radius);
      glNormal3f(vert[j*3+0], vert[j*3+1], vert[j*3+2]);
      j = idices[i * 4 + 2];
      glVertex3f(vert[j*3+0] *radius, vert[j*3+1] *radius, vert[j*3+2] *radius);
      glNormal3f(vert[j*3+0], vert[j*3+1], vert[j*3+2]);
      j = idices[i * 4 + 3];
      glVertex3f(vert[j*3+0] *radius, vert[j*3+1] *radius, vert[j*3+2] *radius);
      glNormal3f(vert[j*3+0], vert[j*3+1], vert[j*3+2]);
      glEnd();
    }
  }

  free(vert);
  free(idices);
}

void glutWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
  _sglut_sphere(radius, slices, stacks, GL_LINE);
}

void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
  _sglut_sphere(radius, slices, stacks, GL_FILL);
}

/*
 * Extract from freeglut.
 * Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
 */
void glutSolidCube(GLdouble dSize)
{
    double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

    /* PWO: Again, I dared to convert the code to use macros... */
    glBegin( GL_QUADS );
        N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
        N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
        N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
        N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
        N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
        N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
    glEnd();

#   undef V
#   undef N
}

/*
 * Extract from freeglut.
 * Draws a wireframed cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
 */
void glutWireCube(GLdouble dSize)
{
    double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

    /* PWO: I dared to convert the code to use macros... */
    glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
    glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();

#   undef V
#   undef N
}
