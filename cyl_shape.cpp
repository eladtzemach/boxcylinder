#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cyl_shape.h"
// matrix manipulation
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


const int CylShape::g_nPoints = 210;

const GLushort CylShape::g_restart = 0xFFFF;



const int CylShape::g_nTriangles = 20;


void CylShape::createColors() {
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

const int CylShape::point_pairs[210][2] = {
	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },

	{ 0,1 },{ 0,4 },{ 0,5 },{ 0,9 },{ 0,10 },
	{ 1,6 },{ 1,7 },{ 1,9 },{ 1,10 },
	{ 2,3 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,11 },
	{ 3,4 },{ 3,5 },{ 3,8 },{ 3,11 },
	{ 4,5 },{ 4,8 },{ 4,9 },
	{ 5,10 },{ 5,11 },
	{ 6,7 },{ 6,10 },{ 6,11 },
	{ 7,8 },{ 7,9 },
	{ 8,9 },
	{ 10,11 },
};

float CylShape::g_vertex[12][3] = {
	// Icosahedron vertexes
	{ -.525731087f, 0.0f, -.850650787f }, // 0
	{ .525731087f, 0.0f, -.850650787f }, // 1
	{ .525731087f, 0.0f, .850650787f }, // 2
	{ -.525731087f, 0.0f, .850650787f }, // 3
	{ -.850650787f, -.525731087f, 0.0f }, // 4
	{ -.850650787f, .525731087f, 0.0f }, // 5
	{ .850650787f, .525731087f, 0.0f }, // 6
	{ .850650787f, -.525731087f, 0.0f }, // 7
	{ 0.0f, -.850650787f, .525731087f }, // 8
	{ 0.0f, -.850650787f, -.525731087f }, // 9
	{ 0.0f, .850650787f, -.525731087f }, // 10
	{ 0.0f, .850650787f, .525731087f } // 11
};

glm::vec3 CylShape::midPoint(float a[], float b[]) {

	glm::vec3 m;

	m.x = (a[0] + b[0]) / 2.0f;
	m.y = (a[1] + b[1]) / 2.0f;
	m.z = (a[2] + b[2]) / 2.0f;

	return m;

}

glm::vec3 getEdgeVec(float x[], float y[]) {
	glm::vec3 m;
	m.x = y[0] - x[0];
	m.y = y[1] - x[1];
	m.z = y[2] - x[2];

	

	return m;
	
}


void CylShape::createTransforms(glm::vec3 _minP, glm::vec3 _maxP) {
	glm::vec3 vec1 = { 0.0f, 0.0f, 1.0f };
	std::cout << d_nTfms << std::endl;
	for (int i = 0; i < d_nTfms; ++i) {
		glm::vec3 mid = midPoint(g_vertex[point_pairs[i][0]],
			g_vertex[point_pairs[i][1]]);

		float theta = glm::dot(vec1, getEdgeVec(g_vertex[point_pairs[i][0]],
			g_vertex[point_pairs[i][1]]));

		glm::vec3 normal = glm::cross(vec1, getEdgeVec(g_vertex[point_pairs[i][0]],
			g_vertex[point_pairs[i][1]]));

		d_tfms[i] = glm::mat4();
		if (i < 30) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			
		}
		else if (i > 29 && i < 60) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x, d_translate_factor_y, 1.0f));
		}
		else if (i > 59 && i < 90) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x, d_translate_factor_y - 7.0f, 1.0f));
		}
		else if (i > 89 && i < 120) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y - 7.0f, 1.0f));
		}
		else if (i > 119 && i < 150) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y, 1.0f));
		}
		else if (i > 149 && i < 180) {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x - 3.0f, d_translate_factor_y, -4.0f));
		}
		else {
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(mid.x * d_translate_factor, mid.y * d_translate_factor, mid.z * d_translate_factor));
			d_tfms[i] = glm::translate(d_tfms[i], glm::vec3(d_translate_factor_x - 7.0f, d_translate_factor_y, -8.0f));
		}
		d_tfms[i] = glm::scale(d_tfms[i], glm::vec3(d_scale_factor, d_scale_factor, d_scale_factor));
		d_tfms[i] = glm::rotate(d_tfms[i], theta, glm::vec3(normal.x, normal.y, normal.z));

	}

  return;
}
