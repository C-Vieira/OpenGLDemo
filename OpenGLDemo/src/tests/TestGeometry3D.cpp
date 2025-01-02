#include "TestGeometry3D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

namespace test {
	TestGeometry3D::TestGeometry3D()
		: m_Proj(glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
		m_Rotationx(glm::radians(0.0f)), m_Rotationy(glm::radians(0.0f)), m_Rotationz(glm::radians(0.0f))
	{
		//Vertex buffer - Cube!
		// Gone...

		// Index buffer
		unsigned int indices[] = {
			0 , 1 , 2 , 2 , 3 , 0 ,
			4 , 5 , 6 , 6 , 7 , 4 ,
			8 , 9 , 10, 10, 11, 8 ,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

		m_VAO = std::make_unique<VertexArray>();
		
		//m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 24 * 6 * sizeof(float)); // 24 vertices * 6 float attribs

		m_VertexBuffer = std::make_unique<VertexBuffer>(24);

		/*VertexBufferLayout layout;
		layout.Push<float>(3); // Positions
		layout.Push<float>(2); // Texture Coords
		layout.Push<float>(1);*/ // Texture Index

		m_VAO->AddBuffer(*m_VertexBuffer);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		// Load Textures
		m_Textures[0] = std::make_unique<Texture>("res/textures/test_tex_face.png");
		m_Textures[1] = std::make_unique<Texture>("res/textures/test_tex_side.png");

		// Set Uniform Texture samplers array
		int samplers[3] = { 0, 1 };
		m_Shader->SetUniform1iv("u_Textures", 2, samplers);
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

		// Create Quads
		auto q1 = CreateQuad(-0.5f, -0.5f, 0.0f);
		auto q2 = CreateQuad(-0.5f, -0.5f, 1.0f);
		auto q3 = CreateQuad(-0.5f,  0.5f, 1.0f);
		auto q4 = CreateQuad( 0.5f,  0.5f, 1.0f);
		auto q5 = CreateQuad(-0.5f, -0.5f, 1.0f);
		auto q6 = CreateQuad(-0.5f,  0.5f, 1.0f);

		// Allocate buffer memory
		Vertex vertices[24];
		auto quadSize = q1.size();
		memcpy(vertices, q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices + quadSize, q2.data(), q2.size() * sizeof(Vertex));

		memcpy(vertices + quadSize * 2, q3.data(), q3.size() * sizeof(Vertex));
		memcpy(vertices + quadSize * 3, q4.data(), q4.size() * sizeof(Vertex));

		memcpy(vertices + quadSize * 4, q5.data(), q5.size() * sizeof(Vertex));
		memcpy(vertices + quadSize * 5, q6.data(), q6.size() * sizeof(Vertex));

		// Arrange quads into a cube shape
		CreateCube(vertices);

		// Bind new Vertex Buffer data
		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		// Bind textures to different slots
		m_Textures[0]->Bind(0);
		m_Textures[1]->Bind(1);

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