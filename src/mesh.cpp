#include "mesh.h"
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures
    , unsigned int instancing, std::vector <glm::mat4> instanceMatrix)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;
    Mesh::instancing = instancing;

    mVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
	VBO instanceVBO(instanceMatrix);
	VBO VBO(vertices);
	EBO EBO(indices);

	mVAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	mVAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	mVAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	mVAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
    if (instancing != 1)
	{
		instanceVBO.Bind();
		// Can't link to a mat4 so you need to link four vec4s
		mVAO.LinkAttrib(instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		mVAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		mVAO.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		mVAO.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		// Makes it so the transform is only switched when drawing the next instance
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}
	mVAO.Unbind();
	VBO.Unbind();
    instanceVBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera,
    glm::mat4 matrix,
    glm::vec3 translation,
    glm::quat rotation,
    glm::vec3 scale
    ){
    shader.Activate();
    mVAO.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for(unsigned int i=0; i<textures.size(); i++){
        std::string num;
        std::string type = textures[i].type;

        if(type == "diffuse") {
            num = std::to_string(numDiffuse++);
        }
        else if(type == "specular") {
            num = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    // Check if instance drawing should be performed
	if (instancing == 1)
	{
        // Initialize matrices
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 sca = glm::mat4(1.0f);

        // Transform the matrices to their correct form
        trans = glm::translate(trans, translation);
        rot = glm::mat4_cast(rotation);
        sca = glm::scale(sca, scale);

        // Push the matrices to the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

        // Draw the actual mesh
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
	else
	{
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instancing);
	}
}
