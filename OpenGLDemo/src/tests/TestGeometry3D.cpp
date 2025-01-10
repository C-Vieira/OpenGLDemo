#include "TestGeometry3D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

namespace test {
	TestGeometry3D::TestGeometry3D()
		: m_Proj(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -6.0f))),
		m_Rotationx(glm::radians(0.0f)), m_Rotationy(glm::radians(0.0f)), m_Rotationz(glm::radians(0.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

		const float yoffset = 1.0f;
		const float xoffset = 0.5f;

		float positions[] = {
			// Shape 1
			// Face 1 - Front Triangle
			-0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 0
			 0.0f,  1.0f + yoffset,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 1
			 0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 2

			// Face - Back Triangle
			-0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 3
			 0.0f,  1.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 4
			 0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 5

			// Face 3 - Bottom Quad
			-0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 6
			 0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 7
			 0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 8
			-0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 9

			// Face 4 - Top Left Quad
			-0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 10
			 0.0f,  1.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 11
			 0.0f,  1.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 12
			-0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 13

			// Face 5 - Top Right Quad
			 0.5f,  0.0f + yoffset,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 14
			 0.0f,  1.0f + yoffset,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 15
			 0.0f,  1.0f + yoffset, -0.2f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 16
			 0.5f,  0.0f + yoffset, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 17

			// Shape 2
			// Face 1 - Front Triangle
			-0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 18
			 0.0f + xoffset,  1.0f,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 19
			 0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 20

			// Face - Back Triangle
			-0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 21
			 0.0f + xoffset,  1.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 22
			 0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 23

			 // Face 3 - Bottom Quad
			 -0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 24
			  0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 25
			  0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 26
			 -0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 27

			 // Face 4 - Top Left Quad
			 -0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 28
			  0.0f + xoffset,  1.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 29
			  0.0f + xoffset,  1.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 30
			 -0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 31

			// Face 5 - Top Right Quad
			 0.5f + xoffset,  0.0f,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 32
			 0.0f + xoffset,  1.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 33
			 0.0f + xoffset,  1.0f, -0.2f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 34
			 0.5f + xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 35

			// Shape 3
			// Face 1 - Front Triangle
			-0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 36
			 0.0f - xoffset,  1.0f,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 37
			 0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 38

			// Face - Back Triangle
			-0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 0.5f, 0.5f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 39
			 0.0f - xoffset,  1.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 40
			 0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 1.0f, /* <--TexIndex */ // Index - 41

			// Face 3 - Bottom Quad
			-0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 42
			 0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 43
			 0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 44
			-0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 45

			// Face 4 - Top Left Quad
			-0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 46
			 0.0f - xoffset,  1.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 47
			 0.0f - xoffset,  1.0f, -0.2f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 48
			-0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 49

			// Face 5 - Top Right Quad
			 0.5f - xoffset,  0.0f,  0.0f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 50
			 0.0f - xoffset,  1.0f,  0.0f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 51
			 0.0f - xoffset,  1.0f, -0.2f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 52
			 0.5f - xoffset,  0.0f, -0.2f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ 0.0f, /* <--TexIndex */ // Index - 53
		};

		// Index buffer
		unsigned int indices[] = {
			// Shape 1
			0,  1,  2,
			3,  4,  5,
			6,  7,  8,  8,  9,  6,
			10, 11, 12, 12, 13, 10,
			14, 15, 16, 16, 17, 14,
			
			// Shape 2
			18, 19, 20,
			21, 22, 23,
			24, 25, 26, 26, 27, 24,
			28, 29, 30, 30, 31, 28,
			32, 33, 34, 34, 35, 32,

			// Shape 3
			36, 37, 38,
			39, 40, 41,
			42, 43, 44, 44, 45, 42,
			46, 47, 48, 48, 49, 46,
			50, 51, 52, 52, 53, 50
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 54 * 6 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VAO->AddBufferWithLayout(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 24 * 3);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		// Load Textures
		m_Textures[0]  = std::make_unique<Texture>("res/textures/test_tex_color.png");
		m_Textures[1]  = std::make_unique<Texture>("res/textures/test_tex_shine.png");

		// Set Uniform Texture samplers array
		int samplers[16] = { 0, 1 };
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

		// Bind textures to different slots
		m_Textures[0]->Bind(0);
		m_Textures[1]->Bind(1);

		{
			// Rotation
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 0.1f, 0.0f));

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