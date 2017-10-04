
#ifndef CSI4130_BOX_SHAPE_H_
#define CSI4130_BOX_SHAPE_H_

#include <cassert>
#include <cstdlib> // Needed in windows for rand

// gl types
#include <GL/glew.h>
//#if WIN32
//#include <gl/wglew.h>
//#else
//#include <GL/glext.h>
//#endif

// glm types
#include <glm/glm.hpp>


class BoxShape {
private:
  const static int g_nPoints;
  const static GLushort g_restart;
  const static int g_nIndices;

	const static int g_nTriangles;

  int d_nColors;
  int d_nTfms;

public:
  // direct specification
  const static GLfloat g_vertex_direct[];
  const static GLfloat draw1[];
  float d_scale_factor = 2.0f;
  
  // indexed specification
  const static GLfloat g_vertex[];
  const static GLushort g_index[];
  // vertex attributes
  glm::vec4* d_colors;
  glm::vec4* d_colors2;
  glm::mat4* d_tfms;
  float d_translate_factor_x = 4.0f;
  float d_translate_factor_y = 4.0f;
  
public:
  inline BoxShape(int _nColors = 84, int _nTfms = 84,  
		  glm::vec3 _minP = glm::vec3(-1.0f,-1.0f,-1.0f),
		  glm::vec3 _maxP = glm::vec3( 1.0f, 1.0f, 1.0f));
  inline ~BoxShape();
  
  // indexed drawing
  inline int getNPoints() const;
  inline glm::vec3 getVertex( int _num ) const;
  inline int getNIndices() const;
  inline GLushort getIndex( int _num ) const;
  inline GLushort getRestart() const;

  // direct drawing
  inline int getNTriangles() const;
  
  // instanced drawing
  inline int getNTransforms() const;
  inline int getNColors() const;

  static inline float randomUnit();

  // Call to change viewing volume
  inline void updateTransforms( int _nTfms, 
				glm::vec3 _minP = glm::vec3(-1.0f,-1.0f,-1.0f),
				glm::vec3 _maxP = glm::vec3( 1.0f, 1.0f, 1.0f));
  
  inline void updateColors( int _nColors );

 private:
  void createColors();
  void createTransforms(glm::vec3 _minP, glm::vec3 _maxP);

  // no copy or assignment
  BoxShape(const BoxShape& _oBoxes );
  BoxShape& operator=( const BoxShape& _oBoxes );
};

BoxShape::BoxShape( int _nColors, int _nTfms,
		    glm::vec3 _minP, glm::vec3 _maxP ) : 
d_nColors(_nColors), d_nTfms(_nTfms){
  d_colors = new glm::vec4[d_nColors];
  d_colors2 = new glm::vec4[d_nColors];
  createColors();
  
  d_tfms = new glm::mat4[d_nTfms];
  createTransforms(_minP,_maxP);
}


BoxShape::~BoxShape() {
  delete[] d_colors;
  delete[] d_colors2;
  delete[] d_tfms;
}

int BoxShape::getNPoints() const {
  return g_nPoints;
}

glm::vec3 BoxShape::getVertex( int _num ) const {
  assert( _num < g_nPoints );
  return glm::vec3(g_vertex[_num*3],g_vertex[_num*3+1],g_vertex[_num*3+2]);
}

int BoxShape::getNIndices() const {
  return g_nIndices;
}

GLushort BoxShape::getIndex( int _num ) const {
  assert( _num < g_nIndices );
  return g_index[_num];
}

GLushort BoxShape::getRestart() const {
  return g_restart;
}


inline int BoxShape::getNTriangles() const {
	return g_nTriangles;
}


int BoxShape::getNColors() const {
  return d_nColors;
}

int BoxShape::getNTransforms() const {
  return d_nTfms;
}

inline void BoxShape::updateTransforms( int _nTfms, 
	  glm::vec3 _minP, glm::vec3 _maxP ) { 
	if ( d_nTfms != _nTfms ) {
		delete[] d_tfms;
		d_nTfms = _nTfms;
	  d_tfms = new glm::mat4[d_nTfms];	
	}
  createTransforms(_minP,_maxP);
}

inline void BoxShape::updateColors( int _nColors ) {
	if ( d_nColors != _nColors ) { 
		delete[] d_colors;
		delete[] d_colors2;
		d_nColors = _nColors; 
	  d_colors = new glm::vec4[d_nColors];
	  d_colors2 = new glm::vec4[d_nColors];
	}  
	createColors();
}

inline float BoxShape::randomUnit() {
  return static_cast<float>(rand())/RAND_MAX; 
}



#endif
