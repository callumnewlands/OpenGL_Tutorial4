#pragma once
#include "glm-0.9.9.0\glm-0.9.9.0\glm\glm.hpp"

struct vertex
{
	glm::vec3 position;
	glm::vec3 colour;

	vertex() { position = glm::vec3();  }
	vertex(float x, float y, float z) 
	{ 
		position = glm::vec3(x, y, z); 
		colour = glm::vec3(0.8f, 0.4f, 1.0f); // default purple colour
	}
	vertex(float x, float y, float z, float r, float g, float b)
	{
		position = glm::vec3(x, y, z);
		colour = glm::vec3(r, g, b);
	}
	~vertex() { }
};