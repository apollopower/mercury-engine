#include "TestTexture.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "MVPMatrix.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{
	

	TestTexture::TestTexture()
		: m_Trans(0, 0, 0)
	{

		// Blending to be refactored...
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 1.0f, // 3
		};

		unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
		};

		m_MVP = std::make_unique<struct MVPMatrix>(
			// Model (the object)
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)),
			// View (the 'camera')
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)),
			// Projection (the normalization of vertices on screen)
			glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f)
		);

		m_VA = std::make_unique<VertexArray>();
		m_VB = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2); // for the texture 'coordinates' (q for self: why cant we just do one Push<float>(4)???)
		m_VA->AddBuffer(*m_VB, layout);

		// index buffers, in order to 'reuse' positions and not duplicate them in memory
		m_IB = std::make_unique<IndexBuffer>(indices, 6);


		// parsing and loading in our shader functions from the shader file provided
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();


		m_Texture = std::make_unique<Texture>("res/textures/id_logo.png");
		
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniformMat4f("u_MVP", m_MVP->matrix());

	}

	TestTexture::~TestTexture()
	{
	}

	void TestTexture::OnUpdate(float deltaTime)
	{
	}

	void TestTexture::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		renderer.Draw(*m_VA, *m_IB, *m_Shader);
		m_Shader->SetUniformMat4f("u_MVP", m_MVP->matrix());
	}

	void TestTexture::OnImGuiRender()
	{

		ImGui::SliderFloat3("Trans A", &m_Trans.x, -1.0f, 1.0f);
		m_MVP->translate(m_Trans);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}