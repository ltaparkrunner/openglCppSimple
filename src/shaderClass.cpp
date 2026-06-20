#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	// std::cout << "get_file_contents start: " << filename << std::endl;
	// std::ifstream in(filename, std::ios::in | std::ios::binary);
	// std::cout << "get_file_contents just after std::ifstream" << std::endl;
	// if (in) {
	// 	std::cout << "get_file_contents just after in" << std::endl;
	// 	std::string contents;
	// 	std::cout << "get_file_contents in.seekg" << std::endl;
	// 	in.seekg(0, std::ios::end);
	// 	contents.resize(in.tellg());
	// 	std::cout << "get_file_contents in.seekg 2" << std::endl;
	// 	in.seekg(0, std::ios::beg);
	// 	std::cout << "get_file_contents in.read" << std::endl;
	// 	in.read(&contents[0], contents.size());
	// 	std::cout << "get_file_contents in.close" << std::endl;
	// 	in.close();
	// 	return contents;
	// }
	// throw(errno);
	std::cout << "get_file_contents start: " << filename << std::endl;
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = get_file_contents(vertexPath);

	std::string fragmentCode = get_file_contents(fragmentPath);
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	// Build and compile our shader program
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");
	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");
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

void Shader::compileErrors(GLuint shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}