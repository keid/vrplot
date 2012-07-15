#include "VolumeRenderer.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>


VolumeRenderer::VolumeRenderer( int w, int h) {
  init(w, h);
  shader_program_ = glCreateProgram();
  
}

void VolumeRenderer::drawVolume( int w, int h) {
  glUseProgram(0);
  
  //glutSolidCube(1.0); return;
  
  enableRenderBuffers();
  
  renderBackface();
  renderVolume();

  disableRenderBuffers();
  
  // TODO : pass windows size
  renderBufferToScreen( final_image_buffer_, w, h );
  //renderBufferToScreen( backface_buffer_, w, h );
}

void VolumeRenderer::loadVolumeData( int x, int y, int z, const unsigned int *data ) {
  glBindTexture( GL_TEXTURE_3D, tex_volume_ );
  glTexImage3D( GL_TEXTURE_3D,
	       0,
	       GL_RGBA,
	       x,
	       y,
	       z,
	       0,
	       GL_RGBA,
	       GL_UNSIGNED_BYTE,
	       data);

}

void VolumeRenderer::init(GLint w, GLint h) {
  glGenTextures(1, &tex_volume_);
  glBindTexture( GL_TEXTURE_3D, tex_volume_ );
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  

  glGenFramebuffersEXT(1, &frame_buffer_);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frame_buffer_);

  
  glGenTextures(1, &backface_buffer_);
  glBindTexture(GL_TEXTURE_2D, backface_buffer_);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F_ARB, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, backface_buffer_, 0);

  glGenTextures(1, &final_image_buffer_);
  glBindTexture(GL_TEXTURE_2D, final_image_buffer_);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F_ARB, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

  glGenRenderbuffersEXT(1, &render_buffer_);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, render_buffer_);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, render_buffer_);
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void VolumeRenderer::drawVertex( float x, float y, float z ) {
  glColor3f(x,y,z);
  glMultiTexCoord3fARB(GL_TEXTURE1_ARB, x, y, z);
  glVertex3f(x,y,z);
}

// this method is used to draw the front and backside of the volume
void VolumeRenderer::drawQuads( float x, float y, float z ) {
  glBegin(GL_QUADS);
  /* Back side */
  glNormal3f(0.0, 0.0, -1.0);
  drawVertex(0.0, 0.0, 0.0);
  drawVertex(0.0, y, 0.0);
  drawVertex(x, y, 0.0);
  drawVertex(x, 0.0, 0.0);

  /* Front side */
  glNormal3f(0.0, 0.0, 1.0);
  drawVertex(0.0, 0.0, z);
  drawVertex(x, 0.0, z);
  drawVertex(x, y, z);
  drawVertex(0.0, y, z);

  /* Top side */
  glNormal3f(0.0, 1.0, 0.0);
  drawVertex(0.0, y, 0.0);
  drawVertex(0.0, y, z);
  drawVertex(x, y, z);
  drawVertex(x, y, 0.0);

  /* Bottom side */
  glNormal3f(0.0, -1.0, 0.0);
  drawVertex(0.0, 0.0, 0.0);
  drawVertex(x, 0.0, 0.0);
  drawVertex(x, 0.0, z);
  drawVertex(0.0, 0.0, z);

  /* Left side */
  glNormal3f(-1.0, 0.0, 0.0);
  drawVertex(0.0, 0.0, 0.0);
  drawVertex(0.0, 0.0, z);
  drawVertex(0.0, y, z);
  drawVertex(0.0, y, 0.0);

  /* Right side */
  glNormal3f(1.0, 0.0, 0.0);
  drawVertex(x, 0.0, 0.0);
  drawVertex(x, y, 0.0);
  drawVertex(x, y, z);
  drawVertex(x, 0.0, z);
  glEnd();
}

void VolumeRenderer::renderFullscreenQuad() {
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
   
  glTexCoord2f(0,0); 
  glVertex2f(0,0);

  glTexCoord2f(1,0); 
  glVertex2f(1,0);

  glTexCoord2f(1, 1); 
  glVertex2f(1, 1);

  glTexCoord2f(0, 1); 
  glVertex2f(0, 1);

  glEnd();
  glEnable(GL_DEPTH_TEST);
}

void VolumeRenderer::renderBufferToScreen( GLuint tex, GLint w, GLint h ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  glLoadIdentity();
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  glViewport(0, 0,w,h);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);

  renderFullscreenQuad();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  
}


void VolumeRenderer::renderBackface() {
  glFramebufferTexture2DEXT
    ( GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D,
      backface_buffer_,
      0
    );
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  drawQuads(1.0,1.0, 1.0);
  glDisable(GL_CULL_FACE);

}

void VolumeRenderer::renderVolume() {

  // NOT completed
  
  glFramebufferTexture2DEXT
    ( GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D,
      final_image_buffer_, 
      0
    );
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  //cgGLEnableProfile(vertexProfile);
  //cgGLEnableProfile(fragmentProfile);
  //cgGLBindProgram(vertex_main);
  //cgGLBindProgram(fragment_main);
  //cgGLSetParameter1f( cgGetNamedParameter( fragment_main, "stepsize") , stepsize);
  
  //set_tex_param("tex",backface_buffer,fragment_main,param1);
  //set_tex_param("volume_tex",volume_texture,fragment_main,param2);

  glUseProgram( shader_program_ );
  
  glUniform1i
    (
     glGetUniformLocation( shader_program_, "tex"), 
     0
    );

  glUniform1i
    (
     glGetUniformLocation( shader_program_, "volume_tex"), 
     1
    );
  
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, backface_buffer_);
  
  glActiveTexture(GL_TEXTURE1); // 3D texture of volume data
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D, tex_volume_);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  drawQuads(1.0,1.0, 1.0);
  glDisable(GL_CULL_FACE);

  glActiveTexture( GL_TEXTURE1 );
  glDisable(GL_TEXTURE_3D);

  glActiveTexture( GL_TEXTURE0 );
  glDisable(GL_TEXTURE_2D);

  glUseProgram( 0 );

}


void VolumeRenderer::enableRenderBuffers() {
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, frame_buffer_);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, render_buffer_);
}

void VolumeRenderer::disableRenderBuffers() {
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void VolumeRenderer::loadShaderSource( const std::string &v_shader_path,
				       const std::string &f_shader_path ) {

  GLuint shader;
 
  // Vertex shader 
  shader = glCreateShader(GL_VERTEX_SHADER);
  compileShader( shader, v_shader_path );
  
  glAttachShader(shader_program_, shader);
  glDeleteShader( shader );

  // Fragment shader
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  compileShader( shader, f_shader_path );
  glAttachShader( shader_program_, shader);
  glDeleteShader( shader );

  // Link program
  linkShaderProgram( shader_program_ );
 
}

std::string VolumeRenderer::readShaderSource( const std::string &path ) {
  std::ifstream ifs( path.c_str() );
  if ( !ifs.good() ) {
    throw std::runtime_error( "VolumeRenderer::readShaderSource::File " + path + "cannot open." );
  }
  
  std::string source;
  while( !ifs.eof() ) {
    std::string buf;
    std::getline( ifs, buf );
    source += buf + std::string("\n");
  }
  
  return source;
}

void VolumeRenderer::compileShader( GLuint shader, const std::string &path ) {
  GLint compiled;

  std::string source = readShaderSource( path );
  
  const GLchar *p_str = NULL;
  GLint p_length = 0;
  p_str = static_cast<const GLchar*>(source.c_str());
  p_length = source.size();

  glShaderSource( shader, 1, &p_str, &p_length);
  glCompileShader( shader );
  glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled);

  std::string result = getShaderInfo( shader );
  
  if( result.size() > 0 ) {
    std::cerr << "Compiling " << path << std::endl;
    std::cerr << result << std::endl;
  }
  
  if ( compiled == GL_FALSE ) {
    throw std::runtime_error( "VolumeRenderer::compileShader::error::" + path );
  }
}

void VolumeRenderer::linkShaderProgram( GLuint shader_program ) {
  
  GLint linked;
  
  glLinkProgram( shader_program );
  glGetProgramiv( shader_program, GL_LINK_STATUS, &linked);
  if ( linked == GL_FALSE ) {
    throw std::runtime_error( "VolumeRenderer::linkShaderProgram::error" );
  }
 
}

std::string VolumeRenderer::getShaderInfo( GLuint shader ) {
  GLsizei bufSize;
  std::string result;
  
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog;

    infoLog = (GLchar *)malloc(bufSize);

    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);

    result = std::string( infoLog );

    free( infoLog );
  }
  
  return result;
}

VolumeRenderer::~VolumeRenderer() {

}

