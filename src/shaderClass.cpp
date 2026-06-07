#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	std::cout << "Reading file: " << filename << std::endl;
	if (in) {
		//std::ostringstream contents;
		//contents << in.rdbuf();
		//in.close();
		//return contents.str();
		std::string contents;
		std::cout << "Reading file 2: " << filename << std::endl;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::cout << "Loading shader files... " << vertexPath << " " << fragmentPath << std::endl;
	std::string vertexCode = get_file_contents(vertexPath);

	std::string fragmentCode = get_file_contents(fragmentPath);
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	// Build and compile our shader program
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	// Link shaders to create a shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// Delete the shader objects once we've linked them into the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

