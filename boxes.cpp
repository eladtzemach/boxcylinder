
#define NOMINMAXS
#include <cstdlib>
#include <stack>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <algorithm>

// glm types
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// matrix manipulation
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "box_shape.h"
#include "cyl_shape.h"

using namespace CSI4130;
using std::cerr;
using std::endl;


namespace CSI4130 {

// Window dimensions
struct WindowSize {
  GLfloat d_near;
  GLfloat d_far;
  GLint d_widthPixel;
  GLfloat d_width;
  GLint d_heightPixel;
  GLfloat d_height;
  bool d_perspective;
  WindowSize() : d_near(1.0f), d_far(21.0f),
		 d_widthPixel(512), d_width(12.5f),
		 d_heightPixel(512), d_height(12.5f)
  {}
}; 

/*
 * Helper structure holding the locations of the uniforms to transform matrices
 */
struct Transformations {
  GLint locP;
  GLint locVM;
  GLint locMM; // per instance model matrix
  Transformations() : locP(-1), locVM(-1), locMM(-1) {}
};

struct Attributes {
  GLint locPos;
  GLint locColor;
  Attributes() : locPos(-1), locColor(-1) {} 
};

/** Global variables */
const int g_numBoxes = 84;
const int g_numCyls = 210;
BoxShape g_boxShape;
CylShape g_cylShape;
GLuint g_ebo, g_ebo2, g_ebo3; 
GLuint program;
GLuint g_vao, g_vao2, g_vao3;
Transformations g_tfm, g_tfm2, g_tfm3;
Attributes g_attrib, g_attrib2, g_attrib3;
WindowSize g_winSize;  
glm::vec3 lookat = { .0f, .0f, -(g_winSize.d_far + g_winSize.d_near) / 2.0f };


void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable( GL_DEPTH_TEST );
  errorOut();

  // Make sure that our shaders run
  int major, minor;
  getGlVersion( major, minor );
  cerr << "Running OpenGL "<< major << "." << minor << endl; 
  if ( major < 3 || (major==3 && minor<3)) {
    cerr << "No OpenGL 3.3 or higher" <<endl;
    exit(-1);
  }

  // Load shaders
  vector<GLuint> sHandles;
  GLuint handle;
  Shader boxes;
  if ( !boxes.load("boxes.vs", GL_VERTEX_SHADER )) {
    boxes.installShader( handle, GL_VERTEX_SHADER );
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  if ( !boxes.load("boxes.fs", GL_FRAGMENT_SHADER )) {
    boxes.installShader( handle, GL_FRAGMENT_SHADER ); 
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  cerr << "No of handles: " << sHandles.size() << endl;
  Shader::installProgram(sHandles, program); 
  errorOut();


  // End of Load the second set of shaders

  // find the locations of uniforms and attributes. Store them in a
  // global structure for later access

  // Activate program in order to be able to get uniform and attribute locations 
  glUseProgram(program);
  errorOut();   




  ////////////////////////////////////////  Create the Color /////////////////////////////////////////
  // vertex attributes
  g_attrib.locPos = glGetAttribLocation(program, "position");
  g_attrib.locColor = glGetAttribLocation(program, "color");
  // transform uniforms and attributes
  g_tfm.locMM = glGetAttribLocation( program, "ModelMatrix");
  g_tfm.locVM = glGetUniformLocation( program, "ViewMatrix");
  g_tfm.locP = glGetUniformLocation( program, "ProjectionMatrix");
  errorOut();

    // Generate a VAO
  glGenVertexArrays(1, &g_vao );
  glBindVertexArray( g_vao );

  // Element array buffer object
  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(GLushort) * g_boxShape.getNIndices(), g_boxShape.g_index, GL_STATIC_DRAW );
  errorOut();

  GLuint vbo;
  glGenBuffers( 1, &vbo );
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo );
  glBufferData(GL_ARRAY_BUFFER, 
	       sizeof(GLfloat) * 3 * g_boxShape.getNPoints(),
	       g_boxShape.g_vertex, GL_STATIC_DRAW);
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(g_attrib.locPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(g_attrib.locPos); 
  errorOut();

  // Color buffer
  g_boxShape.updateColors(g_numBoxes); // ensure that we have enough colors
	GLuint cbo;
  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_boxShape.getNColors(),
	       g_boxShape.d_colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(g_attrib.locColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_attrib.locColor);
  // Ensure the colors are used per instance and not for each vertex
  glVertexAttribDivisor(g_attrib.locColor, 1);

  // ensure that we have enough transforms
  g_boxShape.updateTransforms(g_numBoxes,
      glm::vec3(-g_winSize.d_width/2.0f, 
		-g_winSize.d_height/2.0f, 
		-(g_winSize.d_far - g_winSize.d_near)/2.0f), 
		glm::vec3( g_winSize.d_width/2.0f,
			   g_winSize.d_height/2.0f,
			   (g_winSize.d_far - g_winSize.d_near)/2.0f));
  // Matrix attribute
  GLuint mmbo;
  glGenBuffers(1, &mmbo);
  glBindBuffer(GL_ARRAY_BUFFER, mmbo);
  glBufferData(GL_ARRAY_BUFFER, 
	       sizeof(glm::mat4) * g_boxShape.getNTransforms(), 
	       g_boxShape.d_tfms, GL_DYNAMIC_DRAW);
  // Need to set each column separately.
  for (int i = 0; i < 4; ++i) {
    // Set up the vertex attribute
    glVertexAttribPointer(g_tfm.locMM + i,             // Location
			  4, GL_FLOAT, GL_FALSE,       // Column with four floats
			  sizeof(glm::mat4),           // Stride for next matrix
			  (void *)(sizeof(GLfloat) * 4 * i)); // Offset for ith column
    glEnableVertexAttribArray(g_tfm.locMM + i);
    // Matrix per instance
    glVertexAttribDivisor(g_tfm.locMM  + i, 1);
  }
  // set the projection matrix with a uniform
  glm::mat4 Projection = glm::ortho( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f, 
				     -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
				     g_winSize.d_near, g_winSize.d_far );
  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));
  errorOut();



  ////////////////////////////////////////  Create the Cylinder /////////////////////////////////////////
  glBindVertexArray(0);

  // vertex attributes
  g_attrib2.locPos = glGetAttribLocation(program, "position");
  g_attrib2.locColor = glGetAttribLocation(program, "color");
  // transform uniforms and attributes
  g_tfm2.locMM = glGetAttribLocation(program, "ModelMatrix");
  g_tfm2.locVM = glGetUniformLocation(program, "ViewMatrix");
  g_tfm2.locP = glGetUniformLocation(program, "ProjectionMatrix");
  errorOut();

  // Generate a VAO
  glGenVertexArrays(1, &g_vao2);
  glBindVertexArray(g_vao2);

  // Element array buffer object
  glGenBuffers(1, &g_ebo2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	  sizeof(GLushort) * g_boxShape.getNIndices(),
	  g_boxShape.g_index, GL_STATIC_DRAW);
  errorOut();

  GLuint vbo2;
  glGenBuffers(1, &vbo2);
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glBufferData(GL_ARRAY_BUFFER,
	  sizeof(GLfloat) * 3 * g_boxShape.getNPoints(),
	  g_boxShape.g_vertex, GL_STATIC_DRAW);
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(g_attrib2.locPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_attrib2.locPos);
  errorOut();

  // Color buffer
  //g_boxShape.updateColors(g_numBoxes); // ensure that we have enough colors
  GLuint cbo2;
  glGenBuffers(1, &cbo2);
  glBindBuffer(GL_ARRAY_BUFFER, cbo2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_boxShape.getNColors(),
	  g_boxShape.d_colors2, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(g_attrib2.locColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_attrib2.locColor);
  // Ensure the colors are used per instance and not for each vertex
  glVertexAttribDivisor(g_attrib2.locColor, 1);

  // ensure that we have enough transforms

  // Matrix attribute
  GLuint mmbo2;
  glGenBuffers(1, &mmbo2);
  glBindBuffer(GL_ARRAY_BUFFER, mmbo2);
  glBufferData(GL_ARRAY_BUFFER,
	  sizeof(glm::mat4) * g_boxShape.getNTransforms(),
	  g_boxShape.d_tfms, GL_DYNAMIC_DRAW);
  // Need to set each column separately.
  for (int i = 0; i < 4; ++i) {
	  // Set up the vertex attribute
	  glVertexAttribPointer(g_tfm2.locMM + i,             // Location
		  4, GL_FLOAT, GL_FALSE,       // Column with four floats
		  sizeof(glm::mat4),           // Stride for next matrix
		  (void *)(sizeof(GLfloat) * 4 * i)); // Offset for ith column
	  glEnableVertexAttribArray(g_tfm2.locMM + i);
	  // Matrix per instance
	  glVertexAttribDivisor(g_tfm2.locMM + i, 1);
  }
  // set the projection matrix with a uniform
  glm::mat4 Projection2 = glm::ortho(-g_winSize.d_width / 2.0f, g_winSize.d_width / 2.0f,
	  -g_winSize.d_height / 2.0f, g_winSize.d_height / 2.0f,
	  g_winSize.d_near, g_winSize.d_far);
  glUniformMatrix4fv(g_tfm2.locP, 1, GL_FALSE, glm::value_ptr(Projection));
  errorOut();



  ////////////////////////////////////////  Create the Cylinder /////////////////////////////////////////
  glBindVertexArray(0);

  // vertex attributes
  g_attrib3.locPos = glGetAttribLocation(program, "position");
  g_attrib3.locColor = glGetAttribLocation(program, "color");
  // transform uniforms and attributes
  g_tfm3.locMM = glGetAttribLocation(program, "ModelMatrix");
  g_tfm3.locVM = glGetUniformLocation(program, "ViewMatrix");
  g_tfm3.locP = glGetUniformLocation(program, "ProjectionMatrix");
  errorOut();

  // Generate a VAO
  glGenVertexArrays(1, &g_vao3);
  glBindVertexArray(g_vao3);

  GLuint vbo3;
  glGenBuffers(1, &vbo3);
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo3);
  glBufferData(GL_ARRAY_BUFFER,
	  sizeof(GLfloat) * 3 * g_cylShape.getNPoints(),
	  g_cylShape.cyl_vert, GL_STATIC_DRAW);
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(g_attrib3.locPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_attrib3.locPos);
  errorOut();

  // Color buffer
  //g_boxShape.updateColors(g_numBoxes); // ensure that we have enough colors
  GLuint cbo3;
  glGenBuffers(1, &cbo3);
  glBindBuffer(GL_ARRAY_BUFFER, cbo3);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_cylShape.getNColors(),
	  g_cylShape.d_colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(g_attrib3.locColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_attrib3.locColor);
  // Ensure the colors are used per instance and not for each vertex
  glVertexAttribDivisor(g_attrib3.locColor, 1);

  // ensure that we have enough transforms

  // Matrix attribute
  GLuint mmbo3;
  glGenBuffers(1, &mmbo3);
  glBindBuffer(GL_ARRAY_BUFFER, mmbo3);
  glBufferData(GL_ARRAY_BUFFER,
	  sizeof(glm::mat4) * g_cylShape.getNTransforms(),
	  g_cylShape.d_tfms, GL_DYNAMIC_DRAW);
  // Need to set each column separately.
  for (int i = 0; i < 4; ++i) {
	  // Set up the vertex attribute
	  glVertexAttribPointer(g_tfm3.locMM + i,             // Location
		  4, GL_FLOAT, GL_FALSE,       // Column with four floats
		  sizeof(glm::mat4),           // Stride for next matrix
		  (void *)(sizeof(GLfloat) * 4 * i)); // Offset for ith column
	  glEnableVertexAttribArray(g_tfm3.locMM + i);
	  // Matrix per instance
	  glVertexAttribDivisor(g_tfm3.locMM + i, 1);
  }
  // set the projection matrix with a uniform
  glm::mat4 Projection3 = glm::ortho(-g_winSize.d_width / 2.0f, g_winSize.d_width / 2.0f,
	  -g_winSize.d_height / 2.0f, g_winSize.d_height / 2.0f,
	  g_winSize.d_near, g_winSize.d_far);
  glUniformMatrix4fv(g_tfm3.locP, 1, GL_FALSE, glm::value_ptr(Projection3));
  errorOut();

}


void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glBindVertexArray(0);
  glBindVertexArray(g_vao);
  glUseProgram(program);

  // Instead of moving the coordinate system into the scene,
  // use lookAt -- use the sun as the reference coordinates
  glm::mat4 ModelView = glm::lookAt( lookat,
				     glm::vec3(0, 0, 0),// at is the center of the cube
				     glm::vec3(0, 1.0f, 0 )); // y is up
  
   // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locVM, 1, GL_FALSE, glm::value_ptr(ModelView));
  // VAO is still bound - to be clear bind again
   glBindVertexArray(g_vao);
   glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(g_boxShape.getRestart());
  glDrawElementsInstanced(GL_TRIANGLES, g_boxShape.getNIndices(),
	  GL_UNSIGNED_SHORT, 0, g_numBoxes);


  glBindVertexArray(0);
  glBindVertexArray(g_vao2);
  glDrawElementsInstanced(GL_LINES, g_boxShape.getNIndices(),
    GL_UNSIGNED_SHORT, 0, g_numBoxes);

  glBindVertexArray(0);
  glBindVertexArray(g_vao3);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 20, g_numCyls);



  errorOut();
  // swap buffers
  glFlush();
  glutSwapBuffers();
}


/**
 * OpenGL reshape function - main window
 */
void reshape( GLsizei _width, GLsizei _height ) {
  GLfloat minDim = std::min(g_winSize.d_width,g_winSize.d_height);
  // adjust the view volume to the correct aspect ratio
  if ( _width > _height ) {
    g_winSize.d_width = minDim  * (GLfloat)_width/(GLfloat)_height;
    g_winSize.d_height = minDim;
  } else {
    g_winSize.d_width = minDim;
    g_winSize.d_height = minDim * (GLfloat)_height/(GLfloat)_width;
  }
  glm::mat4 Projection;
  if ( g_winSize.d_perspective ) {
    Projection = glm::frustum( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f, 
			       -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
			       g_winSize.d_near, g_winSize.d_far ); 
  } else {
    Projection = glm::ortho( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f, 
			     -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
			     g_winSize.d_near, g_winSize.d_far );
  }
  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));
  g_winSize.d_widthPixel = _width;
  g_winSize.d_heightPixel = _height;
  // reshape our viewport
  glViewport( 0, 0, 
	      g_winSize.d_widthPixel,
	      g_winSize.d_heightPixel );
}


void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
  case 'q':
    exit(0);
    break;
  case 'P':
    // switch to perspective
    g_winSize.d_perspective = true;
    reshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
    break;
  case 'a':
	  lookat.x = lookat.x + 0.1;
	  break;
  case 's':
	  lookat.x = lookat.x - 0.1;
	  break;
  case 'p':
    // switch to perspective
    g_winSize.d_perspective = false;
    reshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
    break;
  case 'Z':
    // increase near plane
    g_winSize.d_near += 0.1f;
    g_winSize.d_far += 0.1f;
    reshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
    break;
  case 'z':
    // decrease near plane
    if ( g_winSize.d_near > 0.1f ) {
      g_winSize.d_near -= 0.1f;
      g_winSize.d_far -= 0.1f;
    }
    reshape( g_winSize.d_widthPixel, g_winSize.d_heightPixel );
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   GLenum err = glewInit();
   if (GLEW_OK != err) {
     /* Problem: glewInit failed, something is seriously wrong. */
     cerr << "Error: " << glewGetErrorString(err) << endl;
     return -1;
   }
   cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
   cerr << "Before init" << endl;
   init();
   cerr << "After init" << endl;
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}

