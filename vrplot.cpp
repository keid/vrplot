#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <unistd.h>

#if defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

#include "trackball.h"

#include "config.hpp"

#include "VolumeRenderer.hpp"
#include "Controller.hpp"
#include "FileLoader.hpp"

#include "Components.hpp"

/////////////

vrplot::Components* components = NULL;

static int window_w = 256;
static int window_h = 256;

/////////////

static void init();
static void display();
static void resize( int w, int h );
static void idle(int value);
static void mouse( int button, int state, int x, int y );
static void motion( int x, int y );
static void keyboard( unsigned char key, int x, int y );
static void cleanup();

int main(int argc, char *argv[])
{

  glutInit(&argc, argv);
  glutInitWindowSize( window_w, window_h );
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow( vrplot::PROG_NAME.c_str() );
  
  init();
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutTimerFunc( 100, idle, 1);

  glutMainLoop();
  
  return 0;
}

static void cleanup() {
  delete components;
}

static void init() {

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  atexit( cleanup );
  
  components = new vrplot::Components();
  components->getController()->invoke();
}

static void display(void)
{

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glTranslated(-0.5, -0.5, -3.0);

  glMultMatrixd(trackballRotation());
  
  components->getRenderer()->drawVolume( window_w, window_h );
  
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  window_w = w;
  window_h = h;

  trackballRegion(w, h);
  
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  
  glLoadIdentity();
  gluPerspective(45.0, (double)w / (double)h, 1.00, 100.0);
}

static void idle( int value ) {
  if ( components == NULL ) {
    usleep( 100000 );
    return;
  } else if ( components->getController() == NULL ) {
    exit( EXIT_FAILURE );
  } else if ( components->getController()->isFinished() ) {
    exit( EXIT_SUCCESS );
  } else if ( components->getRenderer() != NULL ) {
    if ( value == 1 ) {
      glutPostRedisplay();
    } else if ( components->getRenderer()->isUpdated() ) {
      glutPostRedisplay();
    }
  }
  glutTimerFunc( 100, idle, 0 );
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      trackballStart(x, y);
      break;
    case GLUT_UP:
      trackballStop(x, y);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

static void motion(int x, int y)
{
  trackballMotion(x, y);
  glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit( EXIT_SUCCESS );
  default:
    break;
  }
}
