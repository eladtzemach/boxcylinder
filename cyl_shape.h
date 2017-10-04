
#ifndef CSI4130_CYL_SHAPE_H_
#define CSI4130_CYL_SHAPE_H_

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


class CylShape {
private:
  const static int g_nPoints;
  const static GLushort g_restart;

	const static int g_nTriangles;

  int d_nColors;
  int d_nTfms;

public:
  
  // indexed specification
  GLfloat *cyl_vert = new GLfloat[3 * 20];
  const double CONST_PI = 3.141592653589793238463;
  const static int point_pairs [][2];
  static float g_vertex[][3];
  float d_scale_factor = 1.2f; 
  float d_translate_factor = 2.0f;
  float d_translate_factor_x = 4.0f;
  float d_translate_factor_y = 4.0f;

  // vertex attributes
  glm::vec4* d_colors;
  glm::vec4* d_colors2;
  glm::mat4* d_tfms;
  
public:
  inline CylShape(int _nColors = 210, int _nTfms = 210,
		  glm::vec3 _minP = glm::vec3(-1.0f,-1.0f,-1.0f),
		  glm::vec3 _maxP = glm::vec3( 1.0f, 1.0f, 1.0f));
  inline ~CylShape();
  
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
  glm::vec3 midPoint(float a[], float b[]);

  // Call to change viewing volume
  inline void updateTransforms( int _nTfms, 
				glm::vec3 _minP = glm::vec3(-1.0f,-1.0f,-1.0f),
				glm::vec3 _maxP = glm::vec3( 1.0f, 1.0f, 1.0f));
  
  inline void updateColors( int _nColors );

 private:
  void createColors();
  void createTransforms(glm::vec3 _minP, glm::vec3 _maxP);

  // no copy or assignment
  CylShape(const CylShape& _oCylss );
  CylShape& operator=( const CylShape& _oCyls );
};

CylShape::CylShape( int _nColors, int _nTfms,
		    glm::vec3 _minP, glm::vec3 _maxP ) : 
d_nColors(_nColors), d_nTfms(_nTfms){
  d_colors = new glm::vec4[d_nColors];
  d_colors2 = new glm::vec4[d_nColors];
  createColors();
  
  d_tfms = new glm::mat4[d_nTfms];
  createTransforms(_minP,_maxP);

  for (int i = 0; i < 20; ++i)
  {
	  const GLdouble step = 2.0 * CONST_PI / (double)(18);
	  cyl_vert[3 * i + 0] = 0.25 * std::cos(i * step);
	  cyl_vert[3 * i + 1] = 0.25 * std::sin((i * step + 0.5 * step));
	  cyl_vert[3 * i + 2] = 2.0 * (i % 2 == 0 ? -0.5 : 0.5);
  }
}


CylShape::~CylShape() {
  delete[] d_colors;
  delete[] d_tfms;
}

int CylShape::getNPoints() const {
  return g_nPoints;
}

glm::vec3 CylShape::getVertex( int _num ) const {
  assert( _num < g_nPoints );
  return glm::vec3(cyl_vert[_num*3], cyl_vert[_num*3+1], cyl_vert[_num*3+2]);
}


GLushort CylShape::getRestart() const {
  return g_restart;
}


inline int CylShape::getNTriangles() const {
	return g_nTriangles;
}


int CylShape::getNColors() const {
  return d_nColors;
}

int CylShape::getNTransforms() const {
  return d_nTfms;
}

inline void CylShape::updateTransforms( int _nTfms,
	  glm::vec3 _minP, glm::vec3 _maxP ) { 
	if ( d_nTfms != _nTfms ) {
		delete[] d_tfms;
		d_nTfms = _nTfms;
	  d_tfms = new glm::mat4[d_nTfms];	
	}
  createTransforms(_minP,_maxP);
}

inline void CylShape::updateColors( int _nColors ) {
	if ( d_nColors != _nColors ) { 
		delete[] d_colors;
		delete[] d_colors2;
		d_nColors = _nColors; 
	  d_colors = new glm::vec4[d_nColors];
	  d_colors2 = new glm::vec4[d_nColors];
	}  
	createColors();
}

inline float CylShape::randomUnit() {
  return static_cast<float>(rand())/RAND_MAX; 
}



#endif
