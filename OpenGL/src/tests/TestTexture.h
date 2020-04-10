#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "MVPMatrix.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test {
	class TestTexture : public Test
	{
	private:
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<struct MVPMatrix> m_MVP;
		glm::vec3 m_Trans;

	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	

	};
}