#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

enum class ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    try {
        std::ifstream stream;

        stream.open(filepath);

        std::string line;
        std::stringstream ss[2];

        ShaderType type = ShaderType::NONE;

        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
            
        }
        return { ss[0].str(), ss[1].str() };
    }
    catch (_exception e) {
        std::cout << e.name << std::endl;
    }



}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
   unsigned int id = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   if (!result)
   {
       int lenght;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
       char* message = (char*) alloca(lenght * sizeof(char));
       glGetShaderInfoLog(id, lenght, &lenght, message);
       std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : " fragment") << std::endl;
       std::cout << message << std::endl;
       glDeleteShader(id);
       return 0;
   }

   return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
   unsigned int program = glCreateProgram();
   unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
   unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

   glAttachShader(program, vs);
   glAttachShader(program, fs);
   glLinkProgram(program);
   glValidateProgram(program);

   glDeleteShader(vs);
   glDeleteShader(fs);

   return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // float triangle positions
    float vertex[6] = {
        -0.5,-0.5,
         0.0, 0.5,
         0.5,-0.5,
    };

    // generate buffer ID
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    ShaderProgramSource shaderProgramSource = ParseShader("res/shaders/Basic.shader");
    std::cout << shaderProgramSource.VertexSource << std::endl;
    std::cout << shaderProgramSource.FragmentSource << std::endl;


    unsigned int shader = CreateShader(shaderProgramSource.VertexSource, shaderProgramSource.FragmentSource);
  
    glUseProgram(shader);

    /* Loop until the user closes the window */
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
 
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}