#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* m_window;
	int m_width;
	int m_height;
public:
	Window(int width, int height, const std::string& title);
	~Window();
	void Bind();
	void WindowSizeCallback(GLFWwindow* window, int width, int height);
	int WindowCloseCallback();
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	void Run();
};

