//	#include "mesh.h"
#include<filesystem>
namespace fs = std::filesystem;
#include "model.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL Window", nullptr, nullptr);
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
	glViewport(0, 0, width, height);

	Shader shaderProgram("./assets/shaders/default.vert", "./assets/shaders/default.frag");
//	Shader outliningProgram("./assets/shaders/outlining.vert", "./assets/shaders/outlining.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	// std::string modelPath = "/openglCppSimple/assets/models/crow/scene.gltf";
	// std::string outlinePath = "/openglCppSimple/assets/models/crow-outline/scene.gltf";
	std::string modelPath = "/openglCppSimple/assets/models/statue/scene.gltf";

	// Load in models
	Model model((parentDir + modelPath).c_str());
	// Model outline((parentDir + outlinePath).c_str());
	// Model trees((parentDir + treesPath).c_str());

	while (!glfwWindowShouldClose(window)) {
		// Render here (clear the screen)
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.Inputs(window);

		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		model.Draw(shaderProgram, camera);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		// First method from the tutorial
		// outliningProgram.Activate();
		// glUniform1f(glGetUniformLocation(outliningProgram.ID, "outlining"), 1.08f);
		// model.Draw(outliningProgram, camera);
		
		// Second method from the tutorial
		//outliningProgram.Activate();
		//glUniform1f(glGetUniformLocation(outliningProgram.ID, "outlining"), 0.08f);
		//model.Draw(outliningProgram, camera);
		
		// Third method from the tutorial
		//	outline.Draw(outliningProgram, camera);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);		
		// Poll for and process events

		glfwPollEvents();
	}

	shaderProgram.Delete();
	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
