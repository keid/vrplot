#include "VolumeRenderer.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

namespace vrplot {

VolumeRenderer::VolumeRenderer( ) {
  init();
  
  shader_program_ = glCreateProgram();

  pthread_mutex_init(&mutex_volume_,NULL);
}

void VolumeRenderer::drawVolume( int w, int h) {
  is_updated_ = false;
  
  // Check whether new volume data is passed
  updateVolumeImage();

  // 
  updateBufferSize( w, h );
  
  glUseProgram(0);
  enableRenderBuffers();
  renderBackface();
  renderVolume();
  disableRenderBuffers();
  
  renderBufferToScreen( final_image_buffer_, w, h );
}

void VolumeRenderer::loadVolumeData( int x, int y, int z, const void *data ) {
  pthread_mutex_lock( &mutex_volume_ );
  
  memmove( pre_volume_, data, x * y * z * 4 );
  is_volume_updated_ = true;
  
  pthread_mutex_unlock( &mutex_volume_ );

  postUpdate();
}

bool VolumeRenderer::isUpdated() const {
  return is_updated_;
}

void VolumeRenderer::postUpdate() {
  is_updated_ = true;
}

void VolumeRenderer::init() {

  is_updated_ = true;
  
  width_ = height_ = 256;

  volume_tex_size_ = 256;
  
  pre_volume_ = malloc( volume_tex_size_ * volume_tex_size_ * volume_tex_size_ * 4 );
  memset( pre_volume_, 0, volume_tex_size_ * volume_tex_size_ * volume_tex_size_ * 4);
  is_volume_updated_ = false;
  
  glGenTextures(1, &tex_volume_);
  glBindTexture( GL_TEXTURE_3D, tex_volume_ );
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glTexImage3D( GL_TEXTURE_3D,
		0,
		GL_RGBA,
		volume_tex_size_,
		volume_tex_size_,
		volume_tex_size_,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pre_volume_);

  allocateBuffers( width_, height_ );
}

void VolumeRenderer::drawVertex( float x, float y, float z ) {
  glColor3f(x,y,z);
  glMultiTexCoord3fARB(GL_TEXTURE1_ARB, x, y, z);
  glVertex3f(x,y,z);
}

// this method is used to draw the front and backside of the volume
void VolumeRenderer::drawQuads( float x, float y, float z ) {
  
  static bool is_initialized = false;
  static GLuint display_list;

  if ( !is_initialized ) {
    is_initialized = true;
    display_list = glGenLists(1);
    glNewList( display_list, GL_COMPILE );
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
    glEndList();
  }

  glCallList( display_list );
}

void VolumeRenderer::renderFullscreenQuad() {
  static bool is_initialized = false;
  static GLuint display_list;

  if ( !is_initialized ) {
    is_initialized = true;
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
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

    glEndList();
  }
  
  glDisable(GL_DEPTH_TEST);
  glCallList( display_list );
  glEnable(GL_DEPTH_TEST);

}

void VolumeRenderer::renderBufferToScreen( GLuint tex, GLint w, GLint h ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
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
  
  glDisable(GL_TEXTURE_2D);
    
  glMatrixMode(GL_MODELVIEW);
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
  glUseProgram( 0 );
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  drawQuads(1.0,1.0, 1.0);
  glDisable(GL_CULL_FACE);

}

void VolumeRenderer::renderVolume() {
  glFramebufferTexture2DEXT
    ( GL_FRAMEBUFFER_EXT,
      GL_COLOR_ATTACHMENT0_EXT,
      GL_TEXTURE_2D,
      final_image_buffer_, 
      0
    );
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
  glBindRenderbufferEXT(GL_FRAMEBUFFER_EXT, 0);
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

    infoLog = new GLchar[bufSize];

    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, infoLog);

    result = std::string( infoLog );

    delete [] infoLog;
  }
  
  return result;
}

void VolumeRenderer::updateVolumeImage() {

  pthread_mutex_lock( &mutex_volume_ );
  
  if( is_volume_updated_ ) {
    
    glBindTexture( GL_TEXTURE_3D, tex_volume_ );

    glTexImage3D( GL_TEXTURE_3D,
		  0,
		  GL_RGBA,
		  volume_tex_size_,
		  volume_tex_size_,
		  volume_tex_size_,
		  0,
		  GL_RGBA,
		  GL_UNSIGNED_BYTE,
		  pre_volume_);
    /*
      glTexSubImage3D( GL_TEXTURE_3D,
      0,
      0,
      0,
      0,
      volume_tex_size_,
      volume_tex_size_,
      volume_tex_size_,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      pre_volume_);
    */
  }
  
  is_volume_updated_ = false;
  pthread_mutex_unlock( &mutex_volume_ );

}

void VolumeRenderer::updateBufferSize( GLint w, GLint h ) {
  if ( (w == width_) && (h == height_ ) )return;

  width_ = w;
  height_ = h;
  
  glDeleteFramebuffers( 1, &frame_buffer_ );
  glDeleteTextures( 1, &backface_buffer_ );
  glDeleteTextures( 1, &final_image_buffer_ );
  glDeleteRenderbuffers(1, &render_buffer_ );

  allocateBuffers( width_, height_ );
}

void VolumeRenderer::allocateBuffers( GLint width, GLint height ) {

  glGenFramebuffersEXT(1, &frame_buffer_);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frame_buffer_);
  
  // initializing frame buffer for backface 
  glGenTextures(1, &backface_buffer_);
  glBindTexture(GL_TEXTURE_2D, backface_buffer_);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

  // initializing frame buffer for final image
  glGenTextures(1, &final_image_buffer_);
  glBindTexture(GL_TEXTURE_2D, final_image_buffer_);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA16F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

  glGenRenderbuffersEXT(1, &render_buffer_);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, render_buffer_);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, render_buffer_);

  disableRenderBuffers();
}

VolumeRenderer::~VolumeRenderer() {
  glDeleteTextures( 1, &tex_volume_ );
  glDeleteFramebuffers( 1, &frame_buffer_ );
  glDeleteTextures( 1, &backface_buffer_ );
  glDeleteTextures( 1, &final_image_buffer_ );
  glDeleteRenderbuffers(1, &render_buffer_ );
}

}

