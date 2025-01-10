#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <array>

// Macros
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    float TexID;
};

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

Vertex* CreateQuad(Vertex* target, float x, float y, float textureID);
void CreateCube(Vertex* vertices, float xoffset, float yoffset, float zoffset);

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};