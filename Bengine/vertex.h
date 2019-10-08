#pragma once

#include <GL/glew.h>

namespace Bengine {
	//vertex position data: X + Y
	struct Position {
		float x;
		float y;
	};


	//Color data: RGB + alpha
	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};


	//struct to store vertex info
	struct vertex {

		//vertex position
		Position position;

		//vertex color
		Color color;

		//UV texture coordinates
		UV uv;

		//function to set position
		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		//function to set color
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}


		//function to set texture coor
		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}