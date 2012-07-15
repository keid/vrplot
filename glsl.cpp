#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#if defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

#include "glsl.h"

/*
** GLSL の初期化
*/
int glslInit(void)
{
  int error = 0;
  
  return error;
}

int readShaderSource(GLuint shader, const char *file)
{

  std::ifstream ifs( file );
  if ( !ifs.good() ) {
    std::cerr << "File " << file << " open error." << std::endl;
    return -1;
  }
  
  std::string source;
  while( !ifs.eof() ) {
      std::string buf;
    std::getline( ifs, buf );
    source += buf + std::string("\n");
  }

  const GLchar *p_str = static_cast<const GLchar*>(source.c_str());
  const GLint p_length = source.size();

  glShaderSource( shader, 1, &p_str, &p_length);
  
  return 0;
}

/*
** シェーダの情報を表示する
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
      std::cerr << "InfoLog:" << std::endl;
      std::cerr << std::string(infoLog) << std::endl;
      free(infoLog);
    }
    else
      std::cerr << "Could not allocate InfoLog buffer." << std::endl;
  }
}

/*
** プログラムの情報を表示する
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
