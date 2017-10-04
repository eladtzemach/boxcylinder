#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "box_shape.h"
// matrix manipulation
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const GLfloat BoxShape::g_vertex[] = {
  -0.525731087f, 0.0f, -0.850650787f, // 0
  0.525731087f, 0.0f,  -0.850650787f, // 1
  0.525731087f,  0.0f, 0.850650787f, // 2
  -0.525731087f,  0.0f,  0.850650787f, // 3
   -0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	- 0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	-0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	-0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	-0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	-0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f, // 11

	-0.525731087f, 0.0f, -0.850650787f, // 0
	0.525731087f, 0.0f,  -0.850650787f, // 1
	0.525731087f,  0.0f, 0.850650787f, // 2
	-0.525731087f,  0.0f,  0.850650787f, // 3
	-0.850650787f, -0.525731087f, 0.0f, // 4
	-0.850650787f, 0.525731087f,  0.0f, // 5
	0.850650787f,  0.525731087f, 0.0f, // 6
	0.850650787f,  -0.525731087f,  0.0f, // 7
	0.0f, -0.850650787f, 0.525731087f, // 8
	0.0f, -0.850650787f, -0.525731087f, // 9
	0.0f, 0.850650787f, -0.525731087f, // 10
	0.0f, 0.850650787f, 0.525731087f // 11
};

const int BoxShape::g_nPoints = 20;

const GLushort BoxShape::g_restart = 0xFFFF;

const GLushort BoxShape::g_index[] = {
	1, 9, 0,
	10, 1, 0,
	5, 10, 0,
	4, 5, 0,
	9, 4, 0,
	8, 2, 3,
	4, 8, 3,
	5, 4, 3,
	11, 5, 3,
	2, 11, 3,
	11, 2, 6,
	10, 11, 6,
	1, 10, 6,
	7, 1, 6,
	2, 7, 6,
	11, 10, 5,
	9, 8, 4,
	7, 2, 8,
	9, 7, 8,
	1, 7, 9
};

const int BoxShape::g_nIndices = 60;


const int BoxShape::g_nTriangles = 20;

// direct specification
const GLfloat BoxShape::g_vertex_direct[] = {
  -0.5f, -0.5f, -0.5f, // 0
  -0.5f, -0.5f,  0.5f, // 1
  -0.5f,  0.5f, -0.5f, // 2
 
  -0.5f, -0.5f,  0.5f, // 1
  -0.5f,  0.5f, -0.5f, // 2
  -0.5f,  0.5f,  0.5f, // 3
 
  -0.5f,  0.5f, -0.5f, // 2
  -0.5f,  0.5f,  0.5f, // 3
   0.5f,  0.5f, -0.5f, // 6
 
  -0.5f,  0.5f,  0.5f, // 3
   0.5f,  0.5f, -0.5f, // 6
   0.5f,  0.5f,  0.5f, // 7

   0.5f,  0.5f, -0.5f, // 6
   0.5f,  0.5f,  0.5f, // 7
   0.5f, -0.5f, -0.5f, // 4

   0.5f,  0.5f,  0.5f, // 7
   0.5f, -0.5f, -0.5f, // 4
   0.5f, -0.5f,  0.5f, // 5
//---------------------------
  -0.5f,  0.5f, -0.5f, // 2
   0.5f,  0.5f, -0.5f, // 6
  -0.5f, -0.5f, -0.5f, // 0
  
   0.5f,  0.5f, -0.5f, // 6
  -0.5f, -0.5f, -0.5f, // 0
   0.5f, -0.5f, -0.5f, // 4
  
  -0.5f, -0.5f, -0.5f, // 0
   0.5f, -0.5f, -0.5f, // 4
  -0.5f, -0.5f,  0.5f, // 1

   0.5f, -0.5f, -0.5f, // 4
  -0.5f, -0.5f,  0.5f, // 1
   0.5f, -0.5f,  0.5f, // 5

  -0.5f, -0.5f,  0.5f, // 1
   0.5f, -0.5f,  0.5f, // 5
  -0.5f,  0.5f,  0.5f, // 3

   0.5f, -0.5f,  0.5f, // 5
  -0.5f,  0.5f,  0.5f, // 3
   0.5f,  0.5f,  0.5f // 7
};

const GLfloat BoxShape::draw1[] = { 
    0.0f,  -12.0f, 0.0f,
	6.0f,  -4.0f, 0.0f,
	4.0f,  6.0f, 0.0f,
	-4.0f,  6.0f, 0.0f,
	-6.0f,  -4.0f, 0.0f };


void BoxShape::createColors() {
  // create a jet color map

  int step = d_nColors/4;
  int step2 = step/2;

  for (int i = 0; i < step2; ++i ) {
    d_colors[i].r = 0.0f;
    d_colors[i].g = 0.0f;
    d_colors[i].b = 0.5f + (i*0.5f)/step2;
    d_colors[i].a = 1.0f;
	d_colors2[i].r = 1.0f;
	d_colors2[i].g = 1.0f;
	d_colors2[i].b = 1.0f;
	d_colors2[i].a = 1.0f;

  }
  for (int i = step2; i < step+step2; ++i ) {
    d_colors[i].r = 0.0f;
    d_colors[i].g = static_cast<float>(i-step2)/step;
    d_colors[i].b = 1.0f;
    d_colors[i].a = 1.0f;
	d_colors2[i].r = 1.0f;
	d_colors2[i].g = 1.0f;
	d_colors2[i].b = 1.0f;
	d_colors2[i].a = 1.0f;
  }
  for (int i = step+step2; i<2*step+step2; ++i ) {
    d_colors[i].r = static_cast<float>(i-step-step2)/step;
    d_colors[i].g = 1.0f;
    d_colors[i].b = static_cast<float>(2*step+step2-i)/step;
    d_colors[i].a = 1.0f;
	d_colors2[i].r = 1.0f;
	d_colors2[i].g = 1.0f;
	d_colors2[i].b = 1.0f;
	d_colors2[i].a = 1.0f;
  }
  for (int i = 2*step+step2; i<3*step+step2; ++i ) {
    d_colors[i].r = 1.0f;
    d_colors[i].g = static_cast<float>(3*step+step2-i)/step;
    d_colors[i].b = 0.0f;
    d_colors[i].a = 1.0f;
	d_colors2[i].r = 1.0f;
	d_colors2[i].g = 1.0f;
	d_colors2[i].b = 1.0f;
	d_colors2[i].a = 1.0f;
  }
  for (int i = 3*step+step2; i<d_nColors; ++i ) {
    d_colors[i].r = static_cast<float>(d_nColors-i)/step + 0.5f;
    d_colors[i].g = 0.0f;
    d_colors[i].b = 0.0f;
    d_colors[i].a = 1.0f;
	d_colors2[i].r = 1.0f;
	d_colors2[i].g = 1.0f;
	d_colors2[i].b = 1.0f;
	d_colors2[i].a = 1.0f;
  }
  /*
  for (int i = 0; i < d_nColors; ++i ) {
    std::cerr << i << " [" << d_colors[i].r
    	<< " " << d_colors[i].g
      << " " << d_colors[i].b << "]" << std::endl;
  }
  */
  return;
}



void BoxShape::createTransforms(glm::vec3 _minP, glm::vec3 _maxP) {

	int i = 1;
	for (int j = 0; j < d_nTfms; ++j) {

		

		d_tfms[j] = glm::mat4();
		if (j < 12) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
		}
		else if (j > 11 && j < 24) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x, d_translate_factor_y, 1.0f));
		}
		else if (j > 23 && j < 36) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x , d_translate_factor_y - 7.0f, 1.0f));
		}
		else if (j > 35 && j < 48) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y - 7.0f, 1.0f));
		}
		else if (j > 47 && j < 60) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y, 1.0f));
		}
		else if (j > 59 && j < 72) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x - 3.0f, d_translate_factor_y, -4.0f));
		}
		else if (j > 71 && j < 84) {
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(g_vertex[i - 1] * d_scale_factor, g_vertex[i] * d_scale_factor, g_vertex[i + 1] * d_scale_factor));
			d_tfms[j] = glm::translate(d_tfms[j], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y, -8.0f));
		}

		d_tfms[j] = glm::scale(d_tfms[j], glm::vec3(.8f, .8f, .8f));

	

		i = i + 3;

	}
  return;
}
