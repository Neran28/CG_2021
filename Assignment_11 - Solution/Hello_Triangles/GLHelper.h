#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLDebug.h>
#include <vector>
#include <fstream>
#include <exception>
#include <string>

class ProgramException : public std::exception
{
public:
	ProgramException(const std::string& whatStr) : whatStr(whatStr) {}
	virtual const char* what() const throw()
	{
		return whatStr.c_str();
	}

private:
	std::string whatStr;
};

namespace Shader
{
	GLuint createShaderFromFile(GLenum type, const std::string& sourcePath);
	void checkAndThrowProgram(GLuint program);
};
