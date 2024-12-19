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
		float positions[] = {
			// Face 1
			-0.5f, -0.5f, -0.5f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ // Index - 0
			 0.5f, -0.5f, -0.5f, /* <--Positions */ 1.0f, 0.0f,	/* <--TexCoords */ // Index - 1
			 0.5f,  0.5f, -0.5f, /* <--Positions */ 1.0f, 1.0f,	/* <--TexCoords */ // Index - 2
			-0.5f,  0.5f, -0.5f, /* <--Positions */ 0.0f, 1.0f,	/* <--TexCoords */ // Index - 3

			// Face 2
			-0.5f, -0.5f,  0.5f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ // Index - 4
			 0.5f, -0.5f,  0.5f, /* <--Positions */ 1.0f, 0.0f,	/* <--TexCoords */ // Index - 5
			 0.5f,  0.5f,  0.5f, /* <--Positions */ 1.0f, 1.0f,	/* <--TexCoords */ // Index - 6
			-0.5f,  0.5f,  0.5f, /* <--Positions */ 0.0f, 1.0f,	/* <--TexCoords */ // Index - 7

			// Face 3
			-0.5f,  0.5f,  0.5f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ // Index - 8
			-0.5f,  0.5f, -0.5f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ // Index - 9
			-0.5f, -0.5f, -0.5f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ // Index - 10
			-0.5f, -0.5f,  0.5f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ // Index - 11

			// Face 4
			 0.5f,  0.5f,  0.5f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ // Index - 12
			 0.5f,  0.5f, -0.5f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ // Index - 13
			 0.5f, -0.5f, -0.5f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ // Index - 14
			 0.5f, -0.5f,  0.5f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ // Index - 15

			 // Face 5 
			-0.5f, -0.5f, -0.5f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ // Index - 16
			 0.5f, -0.5f, -0.5f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ // Index - 17
			 0.5f, -0.5f,  0.5f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ // Index - 18
			-0.5f, -0.5f,  0.5f, /* <--Positions */ 0.0f, 0.0f,	/* <--TexCoords */ // Index - 19

			// Face 6
			-0.5f,  0.5f, -0.5f, /* <--Positions */ 0.0f, 1.0f, /* <--TexCoords */ // Index - 20
			 0.5f,  0.5f, -0.5f, /* <--Positions */ 1.0f, 1.0f, /* <--TexCoords */ // Index - 21
			 0.5f,  0.5f,  0.5f, /* <--Positions */ 1.0f, 0.0f, /* <--TexCoords */ // Index - 22
			-0.5f,  0.5f,  0.5f, /* <--Positions */ 0.0f, 0.0f, /* <--TexCoords */ // Index - 23
		};

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
		
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 24 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/bnnuy.png");
		m_Shader->SetUniform1i("u_Texture", 0);
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

		m_Texture->Bind();

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