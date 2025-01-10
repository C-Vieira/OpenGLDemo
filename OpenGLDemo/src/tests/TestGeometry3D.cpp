#include "TestGeometry3D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

namespace test {
	TestGeometry3D::TestGeometry3D()
		: m_Proj(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
		m_Rotationx(glm::radians(0.0f)), m_Rotationy(glm::radians(0.0f)), m_Rotationz(glm::radians(0.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

		const rsize_t MaxCubeCount = 1;
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
		m_Textures[0]  = std::make_unique<Texture>("res/textures/bnnuy.png");

		// Set Uniform Texture samplers array
		int samplers[16] = { 0 };
		m_Shader->SetUniform1iv("u_Textures", 1, samplers);
	}

	TestGeometry3D::~TestGeometry3D()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestGeometry3D::OnUpdate(float deltaTime)
	{
	}

	void TestGeometry3D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		std::array<Vertex, 24> vertices;
		Vertex* buffer = vertices.data();

		// Create Quads
		buffer = CreateQuad(buffer, -0.5f, -0.5f, 0.0f);
		buffer = CreateQuad(buffer, -0.5f, -0.5f, 0.0f);
		buffer = CreateQuad(buffer, -0.5f,  0.5f, 0.0f);
		buffer = CreateQuad(buffer,  0.5f,  0.5f, 0.0f);
		buffer = CreateQuad(buffer, -0.5f, -0.5f, 0.0f);
		buffer = CreateQuad(buffer, -0.5f,  0.5f, 0.0f);

		// Arrange quads into a cube shape
		CreateCube(vertices.data(), 0.0f, 0.0f, 0.0f);

		// Bind new Vertex Buffer data
		m_VertexBuffer->Bind();

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

		// Bind textures to different slots
		m_Textures[0]->Bind(0);

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

	void TestGeometry3D::OnImGuiRender()
	{
		ImGui::SliderFloat("Rotation X", &m_Rotationx, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::SliderFloat("Rotation Y", &m_Rotationy, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::SliderFloat("Rotation Z", &m_Rotationz, glm::radians(0.0f), glm::radians(360.0f));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}