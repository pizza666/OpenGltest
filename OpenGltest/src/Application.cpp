#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError() 
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ": " << line << std::endl;;
        return false;
    }
    return true;
}

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
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    // generate vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // generate vertex array object
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // glue them though attrib pointrs
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    // generate index buffer object
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    ShaderProgramSource shaderProgramSource = ParseShader("res/shaders/Basic.shader");
    std::cout << shaderProgramSource.VertexSource << std::endl;
    std::cout << shaderProgramSource.FragmentSource << std::endl;

    unsigned int shader = CreateShader(shaderProgramSource.VertexSource, shaderProgramSource.FragmentSource);
  
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");

    float red, green, blue, alpha;
    red = 0.0f;
    green = 0.0f;
    blue = 0.5f;
    alpha = 1.0f;
    float inc = 0.01f;

    // unbind all
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    // main loop
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glUseProgram(shader));

        glUniform4f(location, red, green, blue, alpha);
        GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr));

        // cycle red channel
        if (red > 1.0f) 
            inc = -0.01f;
        else if (red < 0.0f) 
            inc = 0.01f;

        red += inc;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
 
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}