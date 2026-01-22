/**
 * @file Shader.hpp
 * @brief OpenGL shader program wrapper
 *
 * Manages shader compilation, linking, and uniform management.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>

#include <string>

namespace deadcode
{

/**
 * @brief OpenGL shader program wrapper
 *
 * Handles shader compilation, linking, and uniform variable management.
 */
class Shader
{
public:
    /**
     * @brief Default constructor
     */
    Shader();

    /**
     * @brief Destructor, releases shader program
     */
    ~Shader();

    /**
     * @brief Load and compile shaders from files
     *
     * @param vertexPath Path to vertex shader source
     * @param fragmentPath Path to fragment shader source
     * @return true if successful
     */
    bool loadFromFiles(const String& vertexPath, const String& fragmentPath);

    /**
     * @brief Compile shaders from source strings
     *
     * @param vertexSource Vertex shader source code
     * @param fragmentSource Fragment shader source code
     * @return true if successful
     */
    bool loadFromSource(const String& vertexSource, const String& fragmentSource);

    /**
     * @brief Use this shader program
     */
    void use() const;

    /**
     * @brief Get the OpenGL program ID
     * @return Shader program ID
     */
    [[nodiscard]] uint32 getID() const;

    /**
     * @brief Check if shader is valid
     * @return true if shader compiled and linked successfully
     */
    [[nodiscard]] bool isValid() const;

    // Uniform setters
    void setInt(const String& name, int32 value) const;
    void setFloat(const String& name, float32 value) const;
    void setVec2(const String& name, const glm::vec2& value) const;
    void setVec3(const String& name, const glm::vec3& value) const;
    void setVec4(const String& name, const glm::vec4& value) const;
    void setMat3(const String& name, const glm::mat3& value) const;
    void setMat4(const String& name, const glm::mat4& value) const;

    // Delete copy constructor and assignment
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    // Allow move construction and assignment
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

private:
    /**
     * @brief Compile a shader
     *
     * @param source Shader source code
     * @param type Shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)
     * @return Shader ID or 0 on failure
     */
    uint32 compileShader(const String& source, uint32 type);

    /**
     * @brief Link shader program
     *
     * @param vertexShader Compiled vertex shader ID
     * @param fragmentShader Compiled fragment shader ID
     * @return Program ID or 0 on failure
     */
    uint32 linkProgram(uint32 vertexShader, uint32 fragmentShader);

    /**
     * @brief Get uniform location (cached)
     *
     * @param name Uniform name
     * @return Uniform location or -1 if not found
     */
    int32 getUniformLocation(const String& name) const;

    uint32 m_programID;
};

}  // namespace deadcode
