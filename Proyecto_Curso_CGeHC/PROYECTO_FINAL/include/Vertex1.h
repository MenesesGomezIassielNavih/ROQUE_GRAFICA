#ifndef VERTEX1_H
#define VERTEX1_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex1 {
    // position
    glm::vec3 Position;
    // color
    glm::vec4 Color;
};

#endif