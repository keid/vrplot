#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#  include "glext.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

#include "glsl.h"

/*
** �����������֥�������
*/
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;

/*
** ����
*/
static const GLfloat lightpos[] = { 0.0, 0.0, 5.0, 1.0 }; /* ���֡����� */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* ľ�ܸ����� */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* �Ķ������� */

/*
** �����
*/
static void init(void)
{
  /* ���������ץ����Υ���ѥ��롿��󥯷�̤������ѿ� */
  GLint compiled, linked;

  /* ������� */
  glClearColor(0.3, 0.3, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  /* �����ν������ */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  /* GLSL �ν���� */
  if (glslInit()) exit(1);

  /* �����������֥������Ȥκ��� */
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  /* ���������Υ������ץ������ɤ߹��� */
  if (readShaderSource(vertShader, "simple.vert")) exit(1);
  if (readShaderSource(fragShader, "simple.frag")) exit(1);

  /* �С��ƥå������������Υ������ץ����Υ���ѥ��� */
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* �ե饰���ȥ��������Υ������ץ����Υ���ѥ��� */
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  /* �ץ���४�֥������Ȥκ��� */
  gl2Program = glCreateProgram();

  /* �����������֥������ȤΥ��������ץ����ؤ���Ͽ */
  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* �����������֥������Ȥκ�� */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  /* ���������ץ����Υ�� */
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }

  /* ���������ץ�����Ŭ�� */
  glUseProgram(gl2Program);
}

/*
** �����������
*/
static void scene(void)
{
  static const GLfloat diffuse[] = { 0.6, 0.1, 0.1, 1.0 };
  static const GLfloat specular[] = { 0.3, 0.3, 0.3, 1.0 };
  
  /* ��������� */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0f);

#if 1
  /* ����Σ��ѷ������� */
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glVertex3d(-1.0, -1.0,  0.0);
  glVertex3d( 1.0, -1.0,  0.0);
  glVertex3d( 1.0,  1.0,  0.0);
  glVertex3d(-1.0,  1.0,  0.0);
  glEnd();
#else
  glutSolidTeapot(1.0);
#endif
}


/****************************
 ** GLUT �Υ�����Хå��ؿ� **
 ****************************/

/* �ȥ�å��ܡ�������Ѵؿ������ */
#include "trackball.h"

static void display(void)
{
  /* ��ǥ�ӥ塼�Ѵ�����ν���� */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* ���̥��ꥢ */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* �����ΰ��֤����� */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* �����ΰ�ư��ʪ�Τ�������˰�ư��*/
  glTranslated(0.0, 0.0, -3.0);
  
  /* �ȥ�å��ܡ�������ˤ���ž */
  glMultMatrixd(trackballRotation());
  
  /* ����������� */
  scene();
  
  /* ���֥�Хåե���� */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* �ȥ�å��ܡ��뤹���ϰ� */
  trackballRegion(w, h);
  
  /* ������ɥ����Τ�ӥ塼�ݡ��Ȥˤ��� */
  glViewport(0, 0, w, h);
  
  /* Ʃ���Ѵ�����λ��� */
  glMatrixMode(GL_PROJECTION);
  
  /* Ʃ���Ѵ�����ν���� */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);
}

static void idle(void)
{
  /* ���̤������ؤ� */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* �ȥ�å��ܡ��볫�� */
      trackballStart(x, y);
      break;
    case GLUT_UP:
      /* �ȥ�å��ܡ������ */
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
  /* �ȥ�å��ܡ����ư */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC �� q �� Q �򥿥��פ����齪λ */
    exit(0);
  default:
    break;
  }
}

/*
** �ᥤ��ץ����
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
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
