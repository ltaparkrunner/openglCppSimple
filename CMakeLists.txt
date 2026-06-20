cmake_minimum_required(VERSION 3.10)
project(OpenGL_Linux_Project CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find standard OpenGL package
find_package(OpenGL REQUIRED)

# 2. Find GLFW package (Handles window and inputs)
find_package(glfw3 REQUIRED)

# 3. Find X11 and Thread libraries (Often required by Linux OpenGL)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

# 4. Define your source files (Add your .cpp and .h files here)
set(SOURCE_FILES
    main.cpp
    glad.c # Include your OpenGL loader if using GLAD
    src/camera.cpp
    src/EBO.cpp
    src/shaderClass.cpp
    src/stb.cpp
    src/texture.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/mesh.cpp
    src/model.cpp
)

# 5. Define the executable output
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_source_files_properties(glad.c PROPERTIES LANGUAGE CXX)
target_include_directories(${PROJECT_NAME}         #MyOpenGLProject 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc 
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/include
)

# 6. Link the libraries to your executable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    glfw
    ${CMAKE_THREAD_LIBS_INIT}
    #glad
    X11
)

# 7. Copy assets/shaders to the build directory (Optional but helpful)
# file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
