#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define RAD(x) (x * M_PI / 180.0)

#define DIV 20

static double g_theta = 0;
static double g_phi = 0;
static double g_radius = 5;

void init(void)
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  //glShadeModel (GL_FLAT);
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
  //glEnable(GL_DEPTH_TEST);
  glColor3f (1.0, 1.0, 1.0);

  glutSolidCube(1.5);
  //glBegin(GL_LINE_LOOP);
  //  glVertex3f(1, 1, 0);
  //  glVertex3f(1, -1, 0);
  //  glVertex3f(-1, -1, 0);
  //  glVertex3f(-1, 1, 0);
  //glEnd();

  glDisable(GL_DEPTH_TEST);
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
