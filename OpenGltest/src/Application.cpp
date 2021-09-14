#include "GLCore.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    Window *window = new Window(960, 540, "Hello");

    //GLFWwindow* window;

    /* Initialize the library */
    //if (!glfwInit())
    //    return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);

    /* Make the window's context current */
    // glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    // glfwSetWindowSizeCallback(window, window_size_callback);



    float vertex[] = {
         100.0f, 100.0f, 0.0f, 0.0f,
         400.0f, 100.0f, 1.0f, 0.0f, 
         400.0f, 400.0f, 1.0f, 1.0f,
         100.0f, 400.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    // generate gl objects
    VertexArray *va = new VertexArray;
    VertexBuffer *vb = new VertexBuffer(vertex, sizeof(vertex));
    VertextBufferLayout *layout = new VertextBufferLayout;
    IndexBuffer *ib = new IndexBuffer(indices, 6);
    Shader *shader = new Shader("res/shaders/Basic.shader");
    Texture* texture = new Texture("res/textures/tex.png");
    Renderer *renderer = new Renderer();

    layout->Push<float>(2);
    layout->Push<float>(2);
    va->AddBuffer(*vb, *layout);

    float red, green, blue, alpha;
    red = 0.0f;
    green = 0.0f;
    blue = 0.5f;
    alpha = 1.0f;

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

    shader->Bind();
    //shader->SetUniform4f("u_color", red, green, blue, alpha);
    shader->SetUniformMat4f("u_mvp", proj);

    texture->Bind();
    shader->SetUniform1i("u_texture", 0);

    // main loop
    while (!window->WindowCloseCallback())
    {
        /* Render here */
        renderer->Clear();
        renderer->Draw(*va, *ib, *shader);
        window->Run();
    }

    delete shader;
    delete va;
    delete ib;
    //delete window;

    return 0;
}