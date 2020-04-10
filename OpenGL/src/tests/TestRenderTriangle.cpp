#include "TestRenderTriangle.h"

#include "VertexBufferLayout.h"

namespace test
{



	TestRenderTriangle::TestRenderTriangle()
	{
		float positions[] = {
				-0.5f, -0.5f, // 0
				 0.5f, -0.5f, // 1
				 0.0f,  0.5f, // 2
		};

		unsigned int indices[] = {
			0, 1, 2,
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
		m_VB = std::make_unique<VertexBuffer>(positions, 2 * 3 * sizeof(float));
		m_IB = std::make_unique<IndexBuffer>(indices, 3);
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		VertexBufferLayout layout;
		layout.Push<float>(2);
		m_VA->AddBuffer(*m_VB, layout);

		m_Shader->SetUniformMat4f("u_MVP", m_MVP->matrix());

	}

	TestRenderTriangle::~TestRenderTriangle()
	{
	}

	void TestRenderTriangle::OnUpdate(float deltaTime)
	{
	}

	void TestRenderTriangle::OnRender()
	{
		// Render stuff
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		renderer.Draw(*m_VA, *m_IB, *m_Shader);
		
	}

	void TestRenderTriangle::OnImGuiRender()
	{
		// Maybe Change color of triangle??
	}

}
