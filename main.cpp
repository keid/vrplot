#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>

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
#include "SimpleVolumeGenerator.hpp"
#include "volumeGenerator/Demo0.hpp"

vrplot::VolumeRenderer *volume_renderer = NULL;
vrplot::controller::Controller *controller = NULL;
vrplot::FileLoader *file_loader = NULL;
vrplot::volumeGenerator::IVolumeGenerator *volume_gen = NULL;

static int window_w = 640;
static int window_h = 480;

static void resize( int w, int h );

static void cleanup( void ) {
  delete volume_renderer;
  delete controller;
  delete file_loader;
  delete volume_gen;
}

static void init(void)
{

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  volume_renderer = new vrplot::VolumeRenderer( window_w, window_h);
  volume_renderer->loadShaderSource( "simple.vert", "simple.frag" );

  //file_loader = new vrplot::FileLoader("test.dat");
  file_loader = new vrplot::FileLoader();

  controller = new vrplot::controller::Controller();
  controller->invoke();
  
  std::vector< int > index;
  index.push_back(0);
  index.push_back(1);
  index.push_back(2);
  index.push_back(3);
  index.push_back(4);
  index.push_back(5);
  index.push_back(6);
  
  //volume_gen = new vrplot::volumeGenerator::SimpleVolumeGenerator( 256, 256, 256 );
  volume_gen = new vrplot::volumeGenerator::Demo0( 256, 256, 256 );
  volume_gen->generate( *file_loader, index );

  volume_renderer->loadVolumeData( 256, 256, 256, volume_gen->volume() );

  atexit( cleanup );
}

static void display(void)
{

  resize( window_w, window_h );
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glTranslated(-0.5, -0.5, -3.0);

  glMultMatrixd(trackballRotation());
  
  volume_renderer->drawVolume( window_w, window_h );
  
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  glutReshapeWindow( window_w, window_h );
  
  trackballRegion(window_w, window_h);
  
  glViewport(0, 0, window_w, window_h);
  
  glMatrixMode(GL_PROJECTION);
  
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.00, 100.0);
}

static void idle(void)
{
  if ( controller == NULL ) {
    exit( EXIT_FAILURE );
  } else if ( controller->isFinished() ) {
    exit( EXIT_SUCCESS );
  } else {
    glutPostRedisplay();
  }
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

int main(int argc, char *argv[])
{

  glutInit(&argc, argv);
  glutInitWindowSize( window_w, window_h );
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

  glutCreateWindow( vrplot::PROG_NAME.c_str() );
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  
  init();
  
  glutMainLoop();
  
  return 0;
}
