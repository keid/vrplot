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

#include "VolumeRenderer.hpp"
#include "Controller.hpp"

VolumeRenderer *volume_renderer = NULL;

int volume_data_size = 256;
unsigned int *volume_data = NULL;

static int window_w = 640;
static int window_h = 480;

static void resize( int w, int h );

static void init(void)
{

  volume_data = new unsigned int[ volume_data_size * volume_data_size * volume_data_size ];
  for(int z = 0; z < volume_data_size; ++z ) {
    for(int y = 0; y < volume_data_size; ++y ) {
      for(int x = 0; x < volume_data_size; ++x ) {
	int index  = x + y * volume_data_size + z * volume_data_size * volume_data_size;

	float dist = std::sqrt( (x-volume_data_size/2)*(x-volume_data_size/2)
				+ (y-volume_data_size/2)*(y-volume_data_size/2)
				+ (z-volume_data_size/2)*(z-volume_data_size/2) );
	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;
	unsigned int a = 0;

	float wave = sin( dist * 0.1 );
	
	if( wave > 0 ) {
	  r = 255 * std::abs(wave);
	  b = 0;
	} else {
	  r = 0;
	  b = 255 * std::abs(wave);
	}

	if ( (x == volume_data_size/2) || ( y == volume_data_size/2 ) ) {
	  a = std::max(r, b) / (dist / 50);
	}
	
	if ( dist < volume_data_size / 2 ) {
	  a = std::max(r, b) / (dist / 5);
	} else {
	  a = 0;
	}

	if ( x > volume_data_size /2 && y > volume_data_size /2  ) {
	  a = 0;
	}
	
	volume_data[ index ] = ((a&0xff)<<24) | ((b&0xff)<<16) | ((g&0xff)<<8) | (r&0xff);
      }
    }
  }

  /////////////

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_CULL_FACE);
  glClearColor(0.0, 0.0, 0.0, 0);
  
  volume_renderer = new VolumeRenderer( window_w, window_h);
  volume_renderer->loadShaderSource( "simple.vert", "simple.frag" );
}

static void display(void)
{

  resize( window_w, window_h );
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glTranslated(-0.5, -0.5, -3.0);
  
  glMultMatrixd(trackballRotation());
  
  volume_renderer->loadVolumeData
    ( volume_data_size,
      volume_data_size,
      volume_data_size,
      volume_data );
  
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
  glutPostRedisplay();
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
    exit(0);
  default:
    break;
  }
}

int main(int argc, char *argv[])
{
  
  vrplot::Controller cont;

  cont.invoke();
  
  glutInit(&argc, argv);
  glutInitWindowSize( window_w, window_h );
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("VolumeRenderer Demo");
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
