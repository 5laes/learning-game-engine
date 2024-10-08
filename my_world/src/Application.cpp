#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define PI atan(1)*4

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    const int window_width = 1280;
    const int window_height = 720;
    window = glfwCreateWindow(window_width, window_height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // GLEW can only be initialized after creating a valid GL context window
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: Failed to initialize GLEW!!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // a vertex can contain a lot more than just positions for example texture coordinates as well and more
        float positions[] =
        {
             -100.0f, -100.0f, 0.0f, 0.0f, //1
              100.0f, -100.0f, 1.0f, 0.0f, //2
              100.0f,  100.0f, 1.0f, 1.0f, //3
             -100.0f,  100.0f, 0.0f, 1.0f  //4
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // Blend enables drawing transparent jpg
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout bLayout;
        bLayout.Push<float>(2);
        bLayout.Push<float>(2);
        va.AddBuffer(vb, bLayout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        glm::vec4 rgba1(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 rgba2(1.0f, 1.0f, 1.0f, 1.0f);
        shader.SetUniform4fv("u_Color", rgba1);

        Texture texture("res/textures/cowboy.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        glm::vec3 translationA((window_width) / 4, (window_height) / 2, 0);
        glm::vec3 translationB((window_width) / 4 * 3, (window_height) / 2, 0);

        bool disco1 = false;
        bool disco2 = false;

        float shift1 = 0.0f;
        float shift2 = 0.0f;
        float increment = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // just some random shit to test ImGui
            if (disco1)
            {
                shift1 += increment;
                rgba1.r = 0.5 * sin(shift1) + 0.5;
                rgba1.g = 0.5 * sin(shift1 + (PI * 2 / 3)) + 0.5;
                rgba1.b = 0.5 * sin(shift1 + (PI * 2 / 3 * 2)) + 0.5;
            }
            if (disco2)
            {
                shift2 += increment;
                rgba2.r = 0.5 * sin(shift2) + 0.5;
                rgba2.g = 0.5 * sin(shift2 + (PI * 2 / 3)) + 0.5;
                rgba2.b = 0.5 * sin(shift2 + (PI * 2 / 3 * 2)) + 0.5;
            }

            

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniform4fv("u_Color", rgba1);
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniform4fv("u_Color", rgba2);
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }




            {
                ImGui::Begin("Hello, world!");
                ImGui::SliderFloat("TranslationA X", &translationA.x, 0.0f, (float)window_width);
                ImGui::SliderFloat("TranslationA Y", &translationA.y, 0.0f, (float)window_height);
                ImGui::SliderFloat3("RGB A Shift", &rgba1.x, 0.0f, 1.0f);
                ImGui::SliderFloat("TranslationB X", &translationB.x, 0.0f, (float)window_width);
                ImGui::SliderFloat("TranslationB Y", &translationB.y, 0.0f, (float)window_height);
                ImGui::SliderFloat3("RGB B Shift", &rgba2.x, 0.0f, 1.0f);
                ImGui::Checkbox("Disco Color A", &disco1);
                ImGui::Checkbox("Disco Color B", &disco2);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            /*render end*/


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Swap End */



            /* Poll for and process events */
            glfwPollEvents();
            /* Poll End */
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}