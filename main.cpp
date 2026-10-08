#include<filesystem>
namespace fs = std::filesystem;
#include "model.h"
#include <glm/gtx/string_cast.hpp>

const unsigned int width = 800;
const unsigned int height = 800;

// Number of samples per pixel for MSAA
unsigned int samples = 8;

// Controls the gamma function
float m_gamma = 2.2f;

float rectangleVertices[] =
{
	//  Coords   // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

// Vertices for plane with texture
std::vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for plane with texture
std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

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
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);
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

	Shader shaderProgram("./assets/shaders/default.vert", "./assets/shaders/default.frag", "./assets/shaders/default.geom");
	Shader framebufferProgram("./assets/shaders/framebuffer.vert", "./assets/shaders/framebuffer.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), m_gamma);

	glEnable(GL_DEPTH_TEST);

	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);

	// Create Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Create Frame Buffer Object
	unsigned int postProcessingFBO;
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Create Framebuffer Texture
	unsigned int postProcessingTexture;
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Error checking framebuffer
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;

	// Paths to textures
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string diffusePath = "/openglCppSimple/assets/textures/diffuse.png";
	std::string normalPath = "/openglCppSimple/assets/textures/normal.png";

	std::vector<Texture> textures =
	{
		Texture((parentDir + diffusePath).c_str(), "diffuse", 0)
	};

	// Plane with the texture
	Mesh plane(vertices, indices, textures);
	// Normal map for the plane
	Texture normalMap((parentDir + normalPath).c_str(), "normal", 1);

	while (!glfwWindowShouldClose(window)) {
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "YoutubeOpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

		// Bind the custom framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		
		glClearColor(pow(0.07f, m_gamma), pow(0.13f, m_gamma), pow(0.17f, m_gamma), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		camera.Inputs(window);

		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Send the light matrix to the shader
		shaderProgram.Activate();
		normalMap.Bind();
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "normal0"), 1);

		// Draw the normal model
		plane.Draw(shaderProgram, camera);

		// Make it so the multisampling FBO is read while the post-processing FBO is drawn
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		// Conclude the multisampling and copy it to the post-processing FBO
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);		
		// Poll for and process events

		glfwPollEvents();
	}

	shaderProgram.Delete();
	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);
	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
