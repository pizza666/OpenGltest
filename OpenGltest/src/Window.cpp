#include "Window.h"
#include "GLCore.h"
#include <iostream>
#include <string>

Window::Window(int width, int height, const std::string& title)
    :m_width(width), m_height(height)
{
    int glfwError = glfwInit();
    if (glfwError != GLFW_TRUE)
        std::cout << "[GLFW] error " << glfwError << " could not create window";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(this->m_window);
    glfwSwapInterval(1);

    int glewError = glewInit();

    if (glewInit() != GLEW_OK)
        std::cout << "[GLEW] error " << glewGetErrorString(glewError) << std::endl;
    
    //glfwSetWindowSizeCallback(m_window, (GLFWwindowsizefun) WindowSizeCallback);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Bind()
{
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}

void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    m_width = width;
    m_height = height;
}

int Window::WindowCloseCallback()
{
    return glfwWindowShouldClose(this->m_window);
}

/// <summary>
/// Runs the Window and the needed tasks, events etc.
/// </summary>
void Window::Run()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}
