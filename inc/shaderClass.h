#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);
//{
//	std::ifstream in(filename, std::ios::in | std::ios::binary);
//	if (in) {
//		std::ostringstream contents;
//		contents << in.rdbuf();
//		in.close();
//		return contents.str();
//	}
//	throw(errno);
//}

class Shader
{
	public:
		GLuint ID;
		Shader(const char* vertexPath, const char* fragmentPath);

		void Activate();
		void Delete();
};


#endif // SHADERCLASS_H
