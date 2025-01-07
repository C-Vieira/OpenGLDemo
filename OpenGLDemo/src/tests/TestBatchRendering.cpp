#include "TestBatchRendering.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

namespace test {
	TestBatchRendering::TestBatchRendering()
		: m_Proj(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -9.0f))),
		m_Rotationx(glm::radians(0.0f)), m_Rotationy(glm::radians(0.0f)), m_Rotationz(glm::radians(0.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

		const rsize_t MaxCubeCount = 10;
		const rsize_t MaxQuadCount = 6 * MaxCubeCount;
		const rsize_t MaxVertexCount = MaxQuadCount * 4 * MaxCubeCount;
		const rsize_t MaxIndexCount = MaxQuadCount * 6 * MaxCubeCount;

		// Index buffer
		unsigned int indices[MaxIndexCount];
		unsigned int offset = 0;
		for (rsize_t i = 0; i < MaxIndexCount; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;
			
			offset += 4;
		}

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(MaxVertexCount);

		m_VAO->AddBuffer(*m_VertexBuffer);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36 * MaxCubeCount);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		// Load Textures
		m_Textures[0]  = std::make_unique<Texture>("res/textures/test_tex_face1-front.png");
		m_Textures[1]  = std::make_unique<Texture>("res/textures/test_tex_face1-side.png");
		m_Textures[2]  = std::make_unique<Texture>("res/textures/test_tex_face1-top.png");
		m_Textures[3]  = std::make_unique<Texture>("res/textures/test_tex_face1-bottom.png");
					   
		m_Textures[4]  = std::make_unique<Texture>("res/textures/test_tex_face2-front.png");
		m_Textures[5]  = std::make_unique<Texture>("res/textures/test_tex_face2-side.png");
		m_Textures[6]  = std::make_unique<Texture>("res/textures/test_tex_face2-top.png");
		m_Textures[7]  = std::make_unique<Texture>("res/textures/test_tex_face2-bottom.png");
					   
		m_Textures[8]  = std::make_unique<Texture>("res/textures/test_tex_face3-front.png");
		m_Textures[9]  = std::make_unique<Texture>("res/textures/test_tex_face3-side.png");
		m_Textures[10] = std::make_unique<Texture>("res/textures/test_tex_face3-top.png");
		m_Textures[11] = std::make_unique<Texture>("res/textures/test_tex_face3-bottom.png");

		m_Textures[12] = std::make_unique<Texture>("res/textures/test_tex_face4-front.png");
		m_Textures[13] = std::make_unique<Texture>("res/textures/test_tex_face4-side.png");
		m_Textures[14] = std::make_unique<Texture>("res/textures/test_tex_face4-top.png");
		m_Textures[15] = std::make_unique<Texture>("res/textures/test_tex_face4-bottom.png");

		// Set Uniform Texture samplers array
		int samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		m_Shader->SetUniform1iv("u_Textures", 16, samplers);
	}

	TestBatchRendering::~TestBatchRendering()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestBatchRendering::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRendering::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		std::array<Vertex, 24 * 10> vertices;
		Vertex* buffer = vertices.data();

		// Create Quads
		for (int i = 0, j = 0; i < 4; i++, j += 4) {
			buffer = CreateQuad(buffer, -0.5f, -0.5f, 0.0f + j);
			buffer = CreateQuad(buffer, -0.5f, -0.5f, 3.0f + j);
			buffer = CreateQuad(buffer, -0.5f,  0.5f, 1.0f + j);
			buffer = CreateQuad(buffer,  0.5f,  0.5f, 1.0f + j);
			buffer = CreateQuad(buffer, -0.5f, -0.5f, 3.0f + j);
			buffer = CreateQuad(buffer, -0.5f,  0.5f, 2.0f + j);
		}

		// Arrange quads into a cube shape
		CreateCube(vertices.data(), 0.0f, 0.0f);
		CreateCube(vertices.data() + 24, 1.0f, 1.0f);
		CreateCube(vertices.data() + 24 * 2, -1.0f, -1.0f);
		CreateCube(vertices.data() + 24 * 3, 1.0f, -1.0f);

		// Bind new Vertex Buffer data
		m_VertexBuffer->Bind();

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

		// Bind textures to different slots
		for (int i = 0; i < 16; i++) {
			m_Textures[i]->Bind(i);
		}

		{
			// Rotation
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			model = glm::rotate(model, m_Rotationy, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, m_Rotationx, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, m_Rotationz, glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestBatchRendering::OnImGuiRender()
	{
		ImGui::SliderFloat("Rotation X", &m_Rotationx, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::SliderFloat("Rotation Y", &m_Rotationy, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::SliderFloat("Rotation Z", &m_Rotationz, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}