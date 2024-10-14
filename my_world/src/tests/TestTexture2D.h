#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {
	
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_TextureA;
		std::unique_ptr<Texture> m_TextureB;

		glm::mat4 m_Proj, m_View;

		glm::vec3 m_TranslationA, m_TranslationB;
		glm::vec4 m_rgba1, m_rgba2;
		bool m_Disco1 = false;
		bool m_Disco2 = false;
		float m_Shift1 = 0.0f;
		float m_Shift2 = 0.0f;
		float m_Increment = 0.01f;
		float m_q0pos[2];
		float m_q1pos[2];
	};
}