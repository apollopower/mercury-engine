#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "MVPMatrix.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>


namespace test
{
	class TestRenderTriangle : public Test
	{
	private:
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<struct MVPMatrix> m_MVP;

	public:
		TestRenderTriangle();
		~TestRenderTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		
	};

}