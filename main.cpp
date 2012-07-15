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



#include "glsl.h"

#include "VolumeRenderer.hpp"

/*
** シェーダオブジェクト
*/
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;

/*
** ¸÷¸»
*/
static const GLfloat lightpos[] = { 0.0, 0.0, 5.0, 1.0 }; /* °ÌÃÖ¡¡¡¡¡¡ */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* Ä¾ÀÜ¸÷¶¯ÅÙ */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* ´Ä¶­¸÷¶¯ÅÙ */

VolumeRenderer *volume_renderer = NULL;

int volume_data_size = 256;
unsigned int *volume_data = NULL;

static int window_w = 640;
static int window_h = 480;

static void resize( int w, int h );

/*
** ½é´ü²½
*/
static void init(void)
{

  volume_data = new unsigned int[ volume_data_size * volume_data_size * volume_data_size ];
  for(int z = 0; z < volume_data_size; ++z ) {
    for(int y = 0; y < volume_data_size; ++y ) {
      for(int x = 0; x < volume_data_size; ++x ) {
	int index  = x + y * volume_data_size + z * volume_data_size * volume_data_size;
	//volume_data[ index ] = static_cast<unsigned int>(x)<<24 + 0xffff<<8;
	
	int dist_x = x - volume_data_size / 2;
	int dist_y = y - volume_data_size / 2;
	int dist_z = z - volume_data_size / 2;
	int dist = std::sqrt( dist_x * dist_x + dist_y * dist_y + dist_z * dist_z );
		
	unsigned int r = 10;
	unsigned int g = 10;
	unsigned int b = 10;
	unsigned int a = 0;

	if ( z > 50 && z < 100 ) {
	  b = 255;
	  a = z - 50;
	}

	if ( x > 50 && x < 100 ) {
	  r = 255;
	  a = y;
	}
	
	if ( y > 50 && y < 100 ) {
	  g = 255;
	  a = x/32;
	}
	
	volume_data[ index ] = ((a&0xff)<<24) | ((b&0xff)<<16) | ((g&0xff)<<8) | (r&0xff);
      }
    }
  }
  
  /* シェーダのコンパイル/リンク結果格納先識別子 */
  GLint compiled, linked;

  /* Initial settings */
  //glClearColor(0.3, 0.3, 1.0, 0.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_CULL_FACE);
  glClearColor(0.0, 0.0, 0.0, 0);
  
  /* Setting light environment */
  /*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  */

  /* Initializing GLSL */
  if (glslInit()) exit(1);

  /* Vertex shader configulation */
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  if (readShaderSource(vertShader, "volume.vert")) exit(1);
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* Fragment shader configulation */
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (readShaderSource(fragShader, "volume.frag")) exit(1);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  gl2Program = glCreateProgram();

  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* Shader objects are not needed after attaching */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }

  /* Enable shader program */
  glUseProgram(gl2Program);

  volume_renderer = new VolumeRenderer( window_w, window_h);
  volume_renderer->loadShaderSource( "simple.vert", "simple.frag" );
}

/****************************
 ** GLUT ¤Î¥³¡¼¥ë¥Ð¥Ã¥¯´Ø¿ô **
 ****************************/

/* ¥È¥é¥Ã¥¯¥Ü¡¼¥ë½èÍýÍÑ´Ø¿ô¤ÎÀë¸À */
#include "trackball.h"

static void display(void)
{

  resize( window_w, window_h );
  /* ¥â¥Ç¥ë¥Ó¥å¡¼ÊÑ´¹¹ÔÎó¤Î½é´ü²½ */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* ²èÌÌ¥¯¥ê¥¢ */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* ¸÷¸»¤Î°ÌÃÖ¤òÀßÄê */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* »ëÅÀ¤Î°ÜÆ°¡ÊÊªÂÎ¤ÎÊý¤ò±ü¤Ë°ÜÆ°¡Ë*/
  //glTranslated(0.5, 0.5, -3.0);
  glTranslated(0.0, 0.0, -3.0);
  
  /* ¥È¥é¥Ã¥¯¥Ü¡¼¥ë½èÍý¤Ë¤è¤ë²óÅ¾ */
  glMultMatrixd(trackballRotation());
  
  volume_renderer->loadVolumeData
    ( volume_data_size,
      volume_data_size,
      volume_data_size,
      volume_data );
  
  volume_renderer->drawVolume( window_w, window_h );
  
  /* ¥À¥Ö¥ë¥Ð¥Ã¥Õ¥¡¥ê¥ó¥° */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  glutReshapeWindow( window_w, window_h );
  
  /* ¥È¥é¥Ã¥¯¥Ü¡¼¥ë¤¹¤ëÈÏ°Ï */
  trackballRegion(window_w, window_h);
  
  /* ¥¦¥£¥ó¥É¥¦Á´ÂÎ¤ò¥Ó¥å¡¼¥Ý¡¼¥È¤Ë¤¹¤ë */
  glViewport(0, 0, window_w, window_h);
  
  /* Æ©»ëÊÑ´¹¹ÔÎó¤Î»ØÄê */
  glMatrixMode(GL_PROJECTION);
  
  /* Æ©»ëÊÑ´¹¹ÔÎó¤Î½é´ü²½ */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.00, 100.0);
}

static void idle(void)
{
  /* ²èÌÌ¤ÎÉÁ¤­ÂØ¤¨ */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* ¥È¥é¥Ã¥¯¥Ü¡¼¥ë³«»Ï */
      trackballStart(x, y);
      break;
    case GLUT_UP:
      /* ¥È¥é¥Ã¥¯¥Ü¡¼¥ëÄä»ß */
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
  /* ¥È¥é¥Ã¥¯¥Ü¡¼¥ë°ÜÆ° */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC ¤« q ¤« Q ¤ò¥¿¥¤¥×¤·¤¿¤é½ªÎ» */
    exit(0);
  default:
    break;
  }
}

/*
** ¥á¥¤¥ó¥×¥í¥°¥é¥à
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize( window_w, window_h );
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
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
