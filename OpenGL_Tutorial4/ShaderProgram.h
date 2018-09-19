#pragma once

#include <string>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class shaderProgram
{
private:
	unsigned int _id;
	void checkCompileErrors(unsigned int shader, std::string type);
public:
	shaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	unsigned int getUniformLocation(const char* uniformName);
	void use();
	~shaderProgram();
};

