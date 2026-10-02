#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H
#include <glad/glad.h>
extern GLuint lightVAO, lightVBO, lightEBO;
void setupLightingCube();
void drawLightingCube();
void cleanUp();
extern GLfloat lightVertices[];
extern GLuint lightIndices[];
extern GLsizeiptr sizeLightV;
extern GLsizeiptr sizeLightI;
#endif // LIGHTSOURCES_H