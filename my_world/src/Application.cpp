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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            -0.5f, -0.5f, 0.0f, 0.0f, //1
             0.5f, -0.5f, 1.0f, 0.0f, //2
             0.5f,  0.5f, 1.0f, 1.0f, //3
            -0.5f,  0.5f, 0.0f, 1.0f //4
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

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/cowboy.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float red = 0.0f;
        float increment = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (red > 1.0f)
            {
                increment = -0.01f;
            }
            else if (red < 0.0f)
            {
                increment = 0.01f;
            }

            red += increment;
            /* Render End */



            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Swap End */



            /* Poll for and process events */
            glfwPollEvents();
            /* Poll End */
        }
    }

    glfwTerminate();
    return 0;
}