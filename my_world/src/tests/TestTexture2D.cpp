#pragma once
#include "TestTexture2D.h"

//#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PI atan(1)*4

namespace test {

    TestTexture2D::TestTexture2D()
        : m_TranslationA(1280 / 4, 720 / 2, 0), m_TranslationB(1280 / 4 * 3, 720 / 2, 0),
            m_rgba1(1.0f, 1.0f, 1.0f, 1.0f), m_rgba2(1.0f, 1.0f, 1.0f, 1.0f),
            m_Proj(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f)), 
            m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        float positions[] =
        {
             -100.0f, -100.0f, 0.0f, 0.0f, 0.0f, //1
              100.0f, -100.0f, 1.0f, 0.0f, 0.0f, //2
              100.0f,  100.0f, 1.0f, 1.0f, 0.0f, //3
             -100.0f,  100.0f, 0.0f, 1.0f, 0.0f, //4

              540.0f, -100.0f, 0.0f, 0.0f, 1.0f, //5
              740.0f, -100.0f, 1.0f, 0.0f, 1.0f, //6
              740.0f,  100.0f, 1.0f, 1.0f, 1.0f, //7
              540.0f,  100.0f, 0.0f, 1.0f, 1.0f  //8
        };

        unsigned int indices[] =
        {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 5 * 8 * sizeof(float));
        VertexBufferLayout bLayout;
        bLayout.Push<float>(2);
        bLayout.Push<float>(2);
        bLayout.Push<float>(1);

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, bLayout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4fv("u_Color", m_rgba1);

        m_TextureA = std::make_unique<Texture>("res/textures/cowboy.png");
        m_TextureB = std::make_unique<Texture>("res/textures/mewow.png");
        m_Shader->SetUniform1i("u_Textures", 0);
        m_Shader->SetUniform1i("u_Textures", 1);
        m_TextureA->Bind(0);
        m_TextureB->Bind(1);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        if (m_Disco1)
        {
            m_Shift1 += m_Increment;
            m_rgba1.r = 0.5 * sin(m_Shift1) + 0.5;
            m_rgba1.g = 0.5 * sin(m_Shift1 + (PI * 2 / 3)) + 0.5;
            m_rgba1.b = 0.5 * sin(m_Shift1 + (PI * 2 / 3 * 2)) + 0.5;
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniform4fv("u_Color", m_rgba1);
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat("TranslationA X", &m_TranslationA.x, 0.0f, 1280.0f);
        ImGui::SliderFloat("TranslationA Y", &m_TranslationA.y, 0.0f, 720.0f);
        ImGui::SliderFloat3("RGB A Shift", &m_rgba1.x, 0.0f, 1.0f);
        ImGui::Checkbox("Disco Color A", &m_Disco1);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
	}
}