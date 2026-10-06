#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

	void Activate();
	void Delete();
private:
	void compileErrors(GLuint shader, const char* type);
};


#endif // SHADERCLASS_H
