#include "lightsources.h"
// или #include <GL/glew.h> в зависимости от вашей библиотеки
// #include <GLFW/glfw3.h>
// #include <iostream>

// Ваши исходные данные
GLfloat lightVertices[] = {
    -20.0f, -20.0f,  20.0f,
    -20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f,  20.0f,
    -20.0f,  20.0f,  20.0f,
    -20.0f,  20.0f, -20.0f,
     20.0f,  20.0f, -20.0f,
     20.0f,  20.0f,  20.0f
};

GLuint lightIndices[] = {
    0, 1, 2,  0, 2, 3,
    0, 4, 7,  0, 7, 3,
    3, 7, 6,  3, 6, 2,
    2, 6, 5,  2, 5, 1,
    1, 5, 4,  1, 4, 0,
    4, 5, 6,  4, 6, 7
};
GLsizeiptr sizeLightV = sizeof(lightVertices);
GLsizeiptr sizeLightI = sizeof(lightIndices);
// Переменные для хранения ID объектов OpenGL
GLuint lightVAO, lightVBO, lightEBO;

void setupLightingCube() {
    // 1. Генерируем объекты
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    // 2. Привязываем VAO (все последующие настройки VBO и EBO сохранятся внутри него)
    glBindVertexArray(lightVAO);

    // 3. Настраиваем VBO (Буфер вершин)
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    // 4. Настраиваем EBO (Буфер индексов)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

    // 5. Указываем OpenGL, как интерпретировать данные вершин (только позиции vec3)
    // location = 0: совпадает с layout (location = 0) в вершинном шейдере
    // 3: размер компонента (X, Y, Z)
    // GL_FLOAT: тип данных
    // GL_FALSE: нужно ли нормализовать данные
    // 3 * sizeof(float): шаг (stride) до следующей вершины
    // (void*)0: смещение от начала буфера
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 6. Отвязываем буферы для безопасности
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    
    // ВАЖНО: EBO нельзя отвязывать, пока привязан VAO! 
    // Он автоматически отвязался, когда мы вызвали glBindVertexArray(0).
}

void drawLightingCube() {
    // Активируем шейдерную программу куба ламп перед отрисовкой
    // glUseProgram(lightShaderProgram);

    // Привязываем VAO со всеми настройками геометрии
    glBindVertexArray(lightVAO);
    
    // Отрисовываем куб по индексам
    // 36 — это количество элементов в lightIndices (12 треугольников * 3 индекса)
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    // Отвязываем обратно
    glBindVertexArray(0);
}

void cleanUp() {
    // Не забываем освобождать память OpenGL при выходе из программы
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &lightEBO);
}
