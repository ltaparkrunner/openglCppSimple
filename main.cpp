#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"
#include "texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Define the vertices of a triangle
GLfloat vertices[] = {
	-0.5f,		-0.5f,		0.0f,		1.0f,	0.0f,	0.0f,		0.0f, 0.0f,	// left  
	-0.5f ,		0.5f,		0.0f,		0.0f,	1.0f,	0.0f,		0.0f, 1.0f,
	0.5f,		0.5f,		0.0f,		0.0f,	0.0f,	1.0f,		1.0f, 1.0f,	// right 
	0.5f,		-0.5f,		0.0f,		1.0f,	1.0f,	1.0f,		1.0f, 0.0f // top   
};

GLuint indices[] = {
	0, 2, 1, // first triangle
	0, 3, 2, // second triangle
};

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	// Tell GLFW we want to use OpenGL 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL Window", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the window's context current
	glfwMakeContextCurrent(window);
	// Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Now we can use OpenGL functions
	gladLoadGL();
	// Set the viewport and clear the screen with a color
	glViewport(0, 0, 800, 800);
	
	Shader shaderProgram("./assets/shaders/default.vert", "./assets/shaders/default.frag");

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture

	Texture popCat("./assets/textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Render here (clear the screen)
		glClearColor(0.27f, 0.33f, 0.37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw the triangle
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		popCat.Bind();
		VAO1.Bind();
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap front and back buffers
		glfwSwapBuffers(window);		
		// Poll for and process events
		glfwPollEvents();
	}

	// Deallocate resources
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();
	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
