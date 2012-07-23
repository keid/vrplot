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

#include "config.hpp"

#include "VolumeRenderer.hpp"
#include "Controller.hpp"
#include "FileLoader.hpp"

#include "Components.hpp"

/////////////

vrplot::Components* components = NULL;

static int window_w = 256;
static int window_h = 256;

static GLdouble camera_yaw = 0;
static GLdouble camera_pitch = 0;
static GLdouble camera_angle = 45;

/////////////

static void init();
static void display();
static void resize( int w, int h );
static void idle(int value);
static void mouse( int button, int state, int x, int y );
static void motion( int x, int y );
static void keyboard( unsigned char key, int x, int y );
static void cleanup();
static void assignGlutFuncs();

int main(int argc, char *argv[])
{

  glutInit(&argc, argv);
  glutInitWindowSize( window_w, window_h );
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow( vrplot::PROG_NAME.c_str() );
  
  init();
  
  assignGlutFuncs();
  
  glutMainLoop();
  
  return 0;
}

static void assignGlutFuncs() {
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutTimerFunc( 100, idle, 1);
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
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  
  glLoadIdentity();
  gluPerspective(camera_angle, (double)window_w / (double)window_h, 1.00, 100.0);
    
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  
  glTranslated(-0.5, -0.5, -3.0);
  
  glTranslated(0.5, 0.5, 0.5);
  glRotated( camera_yaw, 0.0, 1.0, 0.0 );
  glRotated( camera_pitch, 1.0, 0.0, 0.0 );
  glTranslated(-0.5, -0.5, -0.5);
  
  components->getRenderer()->drawVolume( window_w, window_h );
  
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  window_w = w;
  window_h = h;

  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  
  glLoadIdentity();
  gluPerspective(45.0, (double)w / (double)h, 1.00, 100.0);
}

static void idle( int value ) {
  //camera_yaw += 0.1;
  //camera_pitch += 0.1;
  if ( components == NULL ) {
    usleep( 100000 );
    return;
  } else if ( components->getController() == NULL ) {
    exit( EXIT_FAILURE );
  } else if ( components->getController()->isFinished() ) {
    exit( EXIT_SUCCESS );
  } else {
    if ( ( value == 1 ) || ( components->getRenderer()->isUpdated() ) ) {
      glutPostRedisplay();
    }
  }
  glutTimerFunc( 100, idle, 0 );
}

static void mouse(int button, int state, int x, int y)
{

  if ( glutGetModifiers() & GLUT_ACTIVE_SHIFT ) {
    switch (button) {
    case 3:
      if ( camera_angle < 100 ) camera_angle += 0.5;
      break;
    case 4:
      if ( camera_angle > 2 ) camera_angle -= 0.5;
      break;
    }
    glutPostRedisplay();
    return;
  }
  
  switch (button) {
  case 3:
    camera_pitch += 0.5;break;
  case 4:
    camera_pitch -= 0.5; break;
  case 5:
    camera_yaw += 0.5;break;
  case 6:
      camera_yaw -= 0.5;break;
  default:
    break;
  }

  glutPostRedisplay();
}

static void motion(int x, int y)
{
  glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
  if ( components == NULL ) return;
  
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit( EXIT_SUCCESS );
  default:
    break;
  }
}
