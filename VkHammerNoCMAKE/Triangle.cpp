#include "Triangle.h"


Triangle::Triangle()
{
	vertices = {
		{{-0.5f, -0.5f, 0}, {1.0f, 0.0f, 0.0f} , {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	indices = {
	0, 1, 2, 2, 3, 0
	};
}


Triangle::~Triangle()
{
}
