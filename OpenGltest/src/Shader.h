#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

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

class Shader
{
private:
	std::string m_filePath;
	unsigned int m_renderId;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();
	void Bind() const;
	void UnBind() const;
	void SetUniform1f(const std::string& name, float v);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int v);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filePath);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};