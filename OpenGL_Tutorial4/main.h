#pragma once

// OpenGl Library Files
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "glm-0.9.9.0\glm-0.9.9.0\glm\glm.hpp"
#include "glm-0.9.9.0\glm-0.9.9.0\glm\gtc\matrix_transform.hpp"
#include "glm-0.9.9.0\glm-0.9.9.0\glm\gtc\type_ptr.hpp"

// Standard Libraries
#include <iostream>
#include <string>
//#include <fstream>
//#include <sstream>
#include <vector>

// My Header Files
#include "Vertex.h"
#include "ShaderProgram.h"

void initGLFW();
int initGLEW();
GLFWwindow* getDefaultWindow();
//unsigned int getShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
void onWindowResize(GLFWwindow* window, int width, int height);
//void checkCompileErrors(unsigned int shader, std::string type);
unsigned int getVAO(std::vector<vertex> vertices);
std::vector<vertex> getTriangleVertices();
std::vector<vertex> getCubeVertices();

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
