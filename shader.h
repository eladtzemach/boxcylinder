
#ifndef CSI4130_SHADER_H_
#define CSI4130_SHADER_H_


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#if WIN32
#include <gl/wglew.h>
#else
#include <GL/glext.h>
#endif

using std::cerr;
using std::endl;
using std::vector;

namespace CSI4130 {

void getGlVersion( int& major, int& minor );


inline int _printOpenGLerrors(const char *file, int line) {
  GLenum glErrCode;
  int res=0;
  while (GL_NO_ERROR != (glErrCode = glGetError())) {
    cerr <<"glError in file: " << file << " line: " << line 
    << " -- " << gluErrorString(glErrCode) << endl;
    res = glErrCode;
  }
  return res;
}

// Does not work 
/* inline int errorOut() { */
/*   return _printOpenGLerrors(__FILE__, __LINE__); */
/* } */
#define errorOut() _printOpenGLerrors(__FILE__, __LINE__)

class Shader {
  std::string d_vertShaderTxt;
  std::string d_fragShaderTxt;
  bool d_vertShaderRead;
  bool d_fragShaderRead;
 public:
  Shader() : d_vertShaderRead(false), d_fragShaderRead(false) {}

  /** All functions will return 0 on success */
  // Load a shader from file
  int load( std::string filename, GLuint shaderType );
  // Install a shader previously read
  int installShader( GLuint& handle, GLuint shaderType );

  static int compile( GLuint handle );
  static int installProgram(vector<GLuint> shaderHandles, GLuint& program); 
  static int validateProgram(GLuint program);
};
}

#endif
