#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertex[] = {
        -0.5f,-0.5f, 0.0f, 0.0f,
         0.5f,-0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f
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


    shader->Bind();
    //shader->SetUniform4f("u_color", red, green, blue, alpha);

    texture->Bind();
    shader->SetUniform1i("u_texture", 0);

    float inc = 0.01f;

    // main loop
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        renderer->Clear();
        renderer->Draw(*va, *ib, *shader);
        //shader->SetUniform4f("u_Color", red, green, blue, alpha);

        // cycle red channel
        /*    if (red > 1.0f) 
            inc = -0.01f;
        else if (red < 0.0f) 
            inc = 0.01f;
        red += inc;*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
 
    }

    delete shader;
    delete va;
    delete ib;

    glfwTerminate();
    return 0;
}