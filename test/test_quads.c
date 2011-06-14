#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define RAD(x) (x * M_PI / 180.0)

#define DIV 30

static double g_theta = 0;
static double g_phi = 0;
static double g_radius = 5;

void init(void)
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
}

void create_sphere(GLfloat radius,
                   GLdouble **pp_vertices,
                   GLushort** pp_indices,
                   GLfloat* color,
                   GLint stacks,
                   GLint slices)
{
  int i = 0, j = 0;
  int num_vertices = (stacks + 1) * slices;
  int num_quads = stacks * slices;

  GLdouble* p_vertices = (GLdouble*)malloc(num_vertices * 3 * sizeof(GLdouble));
  GLushort* p_indices = (GLushort*)malloc(num_quads * 4 * sizeof(GLushort));

  *pp_vertices = p_vertices;
  *pp_indices = p_indices;

  double theta_total = 180;
  double phi_total = 360;
  double theta_inc = theta_total / stacks;
  double phi_inc = phi_total / slices;

  double theta = 0;
  double phi = 0;
  int idx = 0, base = 0;

  for (i = 0; i <= stacks; ++i) {
    for (j = 0; j < slices; ++j) {
      p_vertices[idx++] = radius * sin(RAD(theta)) * cos(RAD(phi));
      p_vertices[idx++] = radius * cos(RAD(theta));
      p_vertices[idx++] = radius * sin(RAD(theta)) * sin(RAD(phi));
      phi += phi_inc;
    }
    theta += theta_inc;
  }

  idx = 0;
  for (i = 0; i < stacks; ++i) {
    for (j = 0; j < slices; ++j) {
      if (j == slices - 1) {
        p_indices[idx++] = base;
        p_indices[idx++] = base - slices + 1;
        p_indices[idx++] = base + 1;
        p_indices[idx++] = base + slices;
      } else {
        p_indices[idx++] = base;
        p_indices[idx++] = base + 1;
        p_indices[idx++] = base + slices + 1;
        p_indices[idx++] = base + slices;
      }
      ++base;
    }
  }
}

void draw_sphere(GLdouble radius, GLint slices, GLint stacks)
{
  glEnableClientState(GL_VERTEX_ARRAY);

  GLdouble *p_vertices = NULL;
  GLushort *p_indices = NULL;

  create_sphere(radius, &p_vertices, &p_indices, NULL, slices, stacks);
  glVertexPointer(3, GL_DOUBLE, 0, p_vertices); 
  //glDrawElements(GL_QUADS, DIV * DIV * 4, GL_UNSIGNED_SHORT, p_indices);

  int i = 0, j = 0;
  glBegin(GL_QUADS);
  for (i = 0; i < DIV * DIV *4; ++i) {
    j = p_indices[i];
    glVertex3f(p_vertices[j*3+0], p_vertices[j*3+1], p_vertices[j*3+2]);
  }
  glEnd();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //glFrustum (-2.0, 2.0, -2.0, 2.0, 1.0, 20.0);
  glOrtho (-2.0, 2.0, -2.0, 2.0, 0.0, 20.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt (0.0, 0.0, g_radius, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glScalef (1.0, 1.0, 1.0);
  glRotatef(g_phi, 0.0, 1.0, 0.0);
  glRotatef(g_theta, cos(RAD(g_phi)), 0.0, -sin(RAD(g_phi)));

  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(1.0, 1.0, 1.0);
  draw_sphere(1.5, DIV, DIV);

  glutSwapBuffers();
}

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
}

void mouse(int button, int state, int x, int y)
{
  g_radius += (button == 4) * 0.5 - (button == 5) * 0.5;
  glutPostRedisplay();
}

void _motion(int x, int y, int state)
{
  static int prev_x = 0, prev_y = 0;

  if (state && prev_x && prev_y) {
    g_phi += (x - prev_x) * 0.5;
    g_theta += (y - prev_y) * 0.5;
  }

  prev_x = x;
  prev_y = y;

  if (state)
    glutPostRedisplay();
}

void motion(int x, int y)
{
  _motion(x, y, 1);
}

void passive(int x, int y)
{
  _motion(x, y, 0);
}

void keyboard(unsigned char key, int x, int y)
{
  double hor_inc = (key == 'w') * 0.5 - (key == 's') * 0.5;
  double ver_inc = (key == 'd') * 0.5 - (key == 'a') * 0.5;

  glMatrixMode (GL_PROJECTION);
  glTranslated(-ver_inc, 0, hor_inc);
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init();
  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(passive);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
