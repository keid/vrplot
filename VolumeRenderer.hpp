#ifndef VOLUMERENDERER_HPP
#define VOLUMERENDERER_HPP

#include <string>

#if defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

class VolumeRenderer {

public:
  VolumeRenderer( int w, int h);

  void loadShaderSource( const std::string &v_shader_path,
			 const std::string &f_shader_path );

  void loadVolumeData( int x, int y, int z, const unsigned int *data );

  void drawVolume( int w, int h);

  ~VolumeRenderer();

private:

  GLuint tex_volume_;

  GLuint render_buffer_;
  GLuint frame_buffer_;
  GLuint backface_buffer_;
  GLuint final_image_buffer_;

  GLuint shader_program_;

  void init(GLint w, GLint h);
  
  void drawVertex( float x, float y, float z );
  void drawQuads( float x, float y, float z );

  void renderFullscreenQuad();
  void renderBufferToScreen( GLuint tex, GLint w, GLint h );
  void renderBackface();
  void renderVolume();
  
  void enableRenderBuffers();
  void disableRenderBuffers();

  std::string readShaderSource( const std::string &path );
  void compileShader( GLuint shader, const std::string &path );
  void linkShaderProgram( GLuint shader_program );
  std::string getShaderInfo( GLuint shader );
  
};

#endif
