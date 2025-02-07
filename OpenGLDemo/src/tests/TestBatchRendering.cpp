#include "TestBatchRendering.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

#include "Camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;
bool firtsMouse = true;

void ProcessCameraInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Geometry
const rsize_t MaxCubeCount = 80;
const rsize_t MaxQuadCount = 6 * MaxCubeCount;
const rsize_t MaxVertexCount = MaxQuadCount * 4 * MaxCubeCount;
const rsize_t MaxIndexCount = MaxQuadCount * 6 * MaxCubeCount;

int cubeAmount = 10;
bool generated = false;

namespace test {
	TestBatchRendering::TestBatchRendering()
		: m_Proj(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -9.0f))),
		m_Rotationx(glm::radians(0.0f)), m_Rotationy(glm::radians(0.0f)), m_Rotationz(glm::radians(0.0f))
	{
		// CallBacks and Input Mode for Camera
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_callback);
		glfwSetScrollCallback(glfwGetCurrentContext(), scroll_callback);

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

		srand(time(NULL));

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
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

		if (!generated) {

			std::array<Vertex, 24 * MaxCubeCount> vertices;
			Vertex* buffer = vertices.data();

			// Create Quads
			for (int i = 0; i < cubeAmount; i++) {
				int j = rand() % 4;
				switch (j)
				{
				case 0:
					j = 0;
					break;
				case 1:
					j = 4;
					break;
				case 2:
					j = 8;
					break;
				case 3:
					j = 12;
					break;
				}

				buffer = CreateQuad(buffer, -0.5f, -0.5f, 0.0f + j);
				buffer = CreateQuad(buffer, -0.5f, -0.5f, 3.0f + j);
				buffer = CreateQuad(buffer, -0.5f,  0.5f, 1.0f + j);
				buffer = CreateQuad(buffer,  0.5f,  0.5f, 1.0f + j);
				buffer = CreateQuad(buffer, -0.5f, -0.5f, 3.0f + j);
				buffer = CreateQuad(buffer, -0.5f,  0.5f, 2.0f + j);
			}

			for (int i = 0; i < cubeAmount; i++) {
				int randNegativeX = rand() % 2;
				int randNegativeY = rand() % 2;
				int randNegativeZ = rand() % 2;

				int randXOffset = rand() % 21;
				int randYOffset = rand() % 21;
				int randZOffset = rand() % 21;
				
				if (randNegativeX) randXOffset *= -1;
				if (randNegativeY) randYOffset *= -1;
				if (randNegativeZ) randZOffset *= -1;

				CreateCube(vertices.data() + 24 * i, randXOffset, randYOffset, randZOffset);
			}

			// Bind new Vertex Buffer data
			m_VertexBuffer->Bind();

			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

			generated = true;
		}

		// Bind textures to different slots
		for (int i = 0; i < 16; i++) {
			m_Textures[i]->Bind(i);
		}

		{
			// Rotation
			glm::mat4 model = glm::mat4(1.0f);

			//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			//model = glm::rotate(model, m_Rotationy, glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, m_Rotationx, glm::vec3(0.0f, 1.0f, 0.0f));
			//model = glm::rotate(model, m_Rotationz, glm::vec3(0.0f, 0.0f, 1.0f));

			// Per-Frame Time Logic
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Input
			ProcessCameraInput(glfwGetCurrentContext());

			// Camera
			m_View = camera.GetViewMatrix();

			// Zoom
			m_Proj = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestBatchRendering::OnImGuiRender()
	{
		// Regenerate Geometry
		if (ImGui::Button("Regenerate"))
			generated = false;
		ImGui::SliderInt("Cube Amount", &cubeAmount, 0, MaxCubeCount);
		ImGui::Text("TIP: Press \"F\" to show cursor");
		ImGui::Text("TIP: Press \"G\" to hide cursor");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}

void ProcessCameraInput(GLFWwindow* window) {
	// Set Cursor Visibility
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firtsMouse = true;
	}
	if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyBoard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyBoard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firtsMouse) {
		lastX = xpos;
		lastY = ypos;
		firtsMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y coords go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
