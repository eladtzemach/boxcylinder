
#include <fstream>
#include <sstream>
#include "shader.h"


namespace CSI4130 {

void getGlVersion( int& major, int& minor )
{
  char dot;
  std::string verStr = std::string((const char*)glGetString( GL_VERSION ));
  std::istringstream converter(verStr);
  converter >> major;
  converter >> dot;
  if ( dot == '.' ) {
    converter >> minor;
  } else {
    minor = 0;
  }
  return;
}


int Shader::load( std::string filename, GLuint shaderType ) {
  std::ostringstream os;
  std::string line;
  std::ifstream inFile( filename.c_str());
  if ( !inFile ) {
    cerr << "Error: unable to open: " << filename << endl;
    return -1;
  }
  // Read the shader file but keep all characters (including ws and cr)
  while (getline(inFile, line)) {
    os << line << endl;
    if ( inFile.fail() ) {
      cerr << "Read error: " << filename << endl;
      inFile.close();
      return -1;
    }
  }
  inFile.close();
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    d_vertShaderTxt=os.str();
    d_vertShaderRead=true;
    break;
  case GL_FRAGMENT_SHADER:
    d_fragShaderTxt=os.str();
    d_fragShaderRead=true;
    break;
  default:
    cerr << "Invalid shader type: " <<  shaderType << endl;
    return -2;
  }
  return _printOpenGLerrors(__FILE__,__LINE__);
}
  

int Shader::installShader( GLuint& handle, GLuint shaderType ) {
  handle = glCreateShader( shaderType );
  if ( handle == GL_INVALID_OPERATION ) {
    cerr << "Invalid operation: " << shaderType << endl;
    return -1;
  } else if ( handle == GL_INVALID_ENUM ) {
    cerr << "Invalid Enum: " << endl;
    return -1;
  }
  const GLchar* shaderTxt;
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    if ( d_vertShaderRead ) {
      // cerr << d_vertShaderTxt << endl;
      shaderTxt = static_cast<const GLchar*>(d_vertShaderTxt.c_str());
    } else { 
      return -2;
    }
    break;
  case GL_FRAGMENT_SHADER:
    if ( d_fragShaderRead ) {
      // cerr << d_fragShaderTxt << endl;
      shaderTxt = static_cast<const GLchar*>(d_fragShaderTxt.c_str());
    } else { 
      return -2;
    }
    break;
  default:
    cerr << "Invalid shader type: " <<  shaderType << endl;
    return -2;
  }
  glShaderSource( handle, 1, &shaderTxt, NULL );
  return _printOpenGLerrors(__FILE__,__LINE__);
}

int Shader::compile( GLuint handle ) {
  glCompileShader(handle);
  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success );
  if ( !success ) { 
    // get and print the shader info log
    int bSize;
    GLchar *infoLog;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetShaderInfoLog(handle, bSize, NULL, infoLog);
      cerr << "Compile error: (" << handle << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  }
  return _printOpenGLerrors(__FILE__,__LINE__);
}


int Shader::installProgram(vector<GLuint> shaderHandles,
			   GLuint& program) 
{
  program = glCreateProgram();
  for ( vector<GLuint>::const_iterator iter=shaderHandles.begin(); 
	iter != shaderHandles.end(); ++iter ) { 
    glAttachShader(program, *iter);
  }
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success );
  if ( !success ) {
    int bSize;
    GLchar *infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetProgramInfoLog(program, bSize, NULL, infoLog);
      cerr << "Link errors (" << program << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  } else {
    // delete shaders, part of the program
    for ( vector<GLuint>::const_iterator iter=shaderHandles.begin(); 
	  iter != shaderHandles.end(); ++iter ) { 
      glDeleteShader(*iter);
    }
  }
  return _printOpenGLerrors(__FILE__,__LINE__);
}

 
int Shader::validateProgram(GLuint program) {
  glValidateProgram(program);
  GLint success;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success );
  if ( !success ) {
    int bSize;
    GLchar *infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bSize);
    if (bSize > 0) {
      infoLog = new GLchar[bSize+1];
      glGetProgramInfoLog(program, bSize, NULL, infoLog);
      cerr << "Validate error: (" << program << ") :" << infoLog << endl;
      delete [] infoLog;
    }
    return -1;
  }
  return _printOpenGLerrors(__FILE__,__LINE__);
}

}
