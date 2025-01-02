#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): at "
            << file << "\n"
            << function << ": on line "
            << line
            << std::endl;
        return false;
    }
    return true;
}

std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
{
    float size = 1.0f;

    Vertex v0;
    v0.Position = glm::vec3( x, y, 0.0f);
    v0.TexCoords = glm::vec2( 0.0f, 0.0f );
    v0.TexID = textureID;

    Vertex v1;
    v1.Position = glm::vec3(x + size, y, 0.0f);
    v1.TexCoords = glm::vec2( 1.0f, 0.0f );
    v1.TexID = textureID;

    Vertex v2;
    v2.Position = glm::vec3( x + size, y + size, 0.0f);
    v2.TexCoords = glm::vec2( 1.0f, 1.0f );
    v2.TexID = textureID;

    Vertex v3;
    v3.Position = glm::vec3( x, y + size, 0.0f);
    v3.TexCoords = glm::vec2( 0.0f, 1.0f );
    v3.TexID = textureID;

    return { v0, v1, v2, v3 };
}

// Recieve an array of 24 vertices and arrange their positions into a cube
void CreateCube(Vertex* vertices)
{
    if (!vertices) return;

    // Face 1
    vertices[0].Position = { -0.5f, -0.5f, -0.5f };
    vertices[1].Position = {  0.5f, -0.5f, -0.5f };
    vertices[2].Position = {  0.5f,  0.5f, -0.5f };
    vertices[3].Position = { -0.5f,  0.5f, -0.5f };

    // Face 2
    vertices[4].Position  = { -0.5f, -0.5f,  0.5f };
    vertices[5].Position  = {  0.5f, -0.5f,  0.5f };
    vertices[6].Position  = {  0.5f,  0.5f,  0.5f };
    vertices[7].Position  = { -0.5f,  0.5f,  0.5f };

    // Face 3
    vertices[8].Position  = { -0.5f,  0.5f,  0.5f };
    vertices[9].Position  = { -0.5f,  0.5f, -0.5f };
    vertices[10].Position = { -0.5f, -0.5f, -0.5f };
    vertices[11].Position = { -0.5f, -0.5f,  0.5f };

    // Face 4
    vertices[12].Position = { 0.5f,  0.5f,  0.5f };
    vertices[13].Position = { 0.5f,  0.5f, -0.5f };
    vertices[14].Position = { 0.5f, -0.5f, -0.5f };
    vertices[15].Position = { 0.5f, -0.5f,  0.5f };

    // Face 5
    vertices[16].Position = { -0.5f, -0.5f, -0.5f };
    vertices[17].Position = {  0.5f, -0.5f, -0.5f };
    vertices[18].Position = {  0.5f, -0.5f,  0.5f };
    vertices[19].Position = { -0.5f, -0.5f,  0.5f };

    // Face 6
    vertices[20].Position = { -0.5f,  0.5f, -0.5f };
    vertices[21].Position = {  0.5f,  0.5f, -0.5f };
    vertices[22].Position = {  0.5f,  0.5f,  0.5f };
    vertices[23].Position = { -0.5f,  0.5f,  0.5f };
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
