/**
 * @file Shader.cpp
 * @brief Implementation of Shader class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/graphics/Shader.hpp"

#include "deadcode/core/Logger.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace deadcode
{

Shader::Shader() : m_programID(0) {}

Shader::~Shader()
{
    if (m_programID != 0)
    {
        glDeleteProgram(m_programID);
    }
}

Shader::Shader(Shader&& other) noexcept : m_programID(other.m_programID)
{
    other.m_programID = 0;
}

Shader&
Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (m_programID != 0)
        {
            glDeleteProgram(m_programID);
        }
        m_programID       = other.m_programID;
        other.m_programID = 0;
    }
    return *this;
}

bool
Shader::loadFromFiles(const String& vertexPath, const String& fragmentPath)
{
    // Read vertex shader
    std::ifstream vertexFile(vertexPath);
    if (!vertexFile.is_open())
    {
        Logger::error("Failed to open vertex shader: {}", vertexPath);
        return false;
    }

    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    String vertexSource = vertexStream.str();

    // Read fragment shader
    std::ifstream fragmentFile(fragmentPath);
    if (!fragmentFile.is_open())
    {
        Logger::error("Failed to open fragment shader: {}", fragmentPath);
        return false;
    }

    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    String fragmentSource = fragmentStream.str();

    return loadFromSource(vertexSource, fragmentSource);
}

bool
Shader::loadFromSource(const String& vertexSource, const String& fragmentSource)
{
    // Compile shaders
    uint32 vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    if (vertexShader == 0)
    {
        return false;
    }

    uint32 fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    // Link program
    m_programID = linkProgram(vertexShader, fragmentShader);

    // Clean up shaders (they're linked into the program now)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return m_programID != 0;
}

void
Shader::use() const
{
    if (m_programID != 0)
    {
        glUseProgram(m_programID);
    }
}

uint32
Shader::getID() const
{
    return m_programID;
}

bool
Shader::isValid() const
{
    return m_programID != 0;
}

void
Shader::setInt(const String& name, int32 value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void
Shader::setFloat(const String& name, float32 value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void
Shader::setVec2(const String& name, const glm::vec2& value) const
{
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void
Shader::setVec3(const String& name, const glm::vec3& value) const
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void
Shader::setVec4(const String& name, const glm::vec4& value) const
{
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void
Shader::setMat3(const String& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void
Shader::setMat4(const String& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

uint32
Shader::compileShader(const String& source, uint32 type)
{
    uint32 shader   = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check compilation status
    int32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        const char* shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        Logger::error("Shader compilation failed ({}): {}", shaderType, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

uint32
Shader::linkProgram(uint32 vertexShader, uint32 fragmentShader)
{
    uint32 program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check linking status
    int32 success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        Logger::error("Shader program linking failed: {}", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

int32
Shader::getUniformLocation(const String& name) const
{
    return glGetUniformLocation(m_programID, name.c_str());
}

}  // namespace deadcode
