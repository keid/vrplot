#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

#include "glsl.h"

/*
** GLSL �ν����
*/
int glslInit(void)
{
  int error = 0;
  
  return error;
}

/*
** �����������Υ������ץ����������ɤ߹���
*/
int readShaderSource(GLuint shader, const char *file)
{
  FILE *fp;
  const GLchar *source;
  GLsizei length;
  int ret;
  
  /* �ե�����򳫤� */
  fp = fopen(file, "rb");
  if (fp == NULL) {
    perror(file);
    return -1;
  }
  
  /* �ե�����������˰�ư�����߰��֡ʤĤޤ�ե����륵�����ˤ����� */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);
  
  /* �ե����륵�����Υ������� */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
    fprintf(stderr, "Could not allocate read buffer.\n");
    return -1;
  }
  
  /* �ե��������Ƭ�����ɤ߹��� */
  fseek(fp, 0L, SEEK_SET);
  ret = fread((void *)source, 1, length, fp) != (size_t)length;
  fclose(fp);
  
  /* ���������Υ������ץ����Υ����������֥������Ȥؤ��ɤ߹��� */
  if (ret)
    fprintf(stderr, "Could not read file: %s.\n", file);
  else
    glShaderSource(shader, 1, &source, &length);
  
  /* ���ݤ�������γ��� */
  free((void *)source);
  
  return ret;
}

/*
** ���������ξ����ɽ������
*/
void printShaderInfoLog(GLuint shader)
{
  GLsizei bufSize;
  
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog;
    
    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;
      
      glGetShaderInfoLog(shader, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}

/*
** �ץ����ξ����ɽ������
*/
void printProgramInfoLog(GLuint program)
{
  GLsizei bufSize;
  
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog;
    
    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;
      
      glGetProgramInfoLog(program, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}
