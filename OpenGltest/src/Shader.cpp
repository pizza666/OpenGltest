#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream;

    stream.open(filePath);

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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int lenght;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        char* message = (char*)_malloca(lenght * sizeof(char));
        GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : " fragment") << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

Shader::Shader(const std::string& filePath)
	:m_filePath(filePath), m_renderId(0)
{
    ShaderProgramSource shaderProgramSource = ParseShader(filePath);
    std::cout << shaderProgramSource.VertexSource << std::endl;
    std::cout << shaderProgramSource.FragmentSource << std::endl;
    m_renderId = CreateShader(shaderProgramSource.VertexSource, shaderProgramSource.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_renderId));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_renderId));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v)
{
    GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform1f(const std::string& name, float v)
{
    GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    GLCall(int location = glGetUniformLocation(m_renderId, name.c_str()));
    if (location < 0)
        std::cout << "[OpenGL Error] " << name << " not found!" << std::endl;
    m_uniformLocationCache[name] = location;
	return location;
}
