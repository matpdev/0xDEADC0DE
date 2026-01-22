/**
 * @file TextRenderer.cpp
 * @brief Implementation of TextRenderer class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/graphics/TextRenderer.hpp"

#include "deadcode/core/Logger.hpp"

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/matrix_transform.hpp>

namespace deadcode
{

TextRenderer::TextRenderer() : m_VAO(0), m_VBO(0), m_initialized(false) {}

TextRenderer::~TextRenderer()
{
    shutdown();
}

bool
TextRenderer::initialize(int32 screenWidth, int32 screenHeight)
{
    Logger::info("Initializing TextRenderer...");

    // Create shader for text rendering
    const String vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    const String fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

    if (!m_shader.loadFromSource(vertexShaderSource, fragmentShaderSource))
    {
        Logger::error("Failed to create text shader");
        return false;
    }

    // Setup projection matrix
    m_projection = glm::ortho(0.0f, static_cast<float32>(screenWidth), 0.0f,
                              static_cast<float32>(screenHeight));

    m_shader.use();
    m_shader.setMat4("projection", m_projection);

    // Setup rendering data
    setupRenderData();

    m_initialized = true;
    Logger::info("TextRenderer initialized successfully");
    return true;
}

void
TextRenderer::shutdown()
{
    if (!m_initialized)
        return;

    Logger::info("Shutting down TextRenderer...");

    // Delete character textures
    for (auto& pair : m_characters)
    {
        glDeleteTextures(1, &pair.second.textureID);
    }
    m_characters.clear();

    // Delete OpenGL resources
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }

    m_initialized = false;
}

bool
TextRenderer::loadFont(const String& fontPath, uint32 fontSize)
{
    Logger::info("Loading font: {} (size: {})", fontPath, fontSize);

    // Initialize FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        Logger::error("Failed to initialize FreeType library");
        return false;
    }

    // Load font face
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
    {
        Logger::error("Failed to load font: {}", fontPath);
        FT_Done_FreeType(ft);
        return false;
    }

    // Set font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 ASCII characters
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Logger::warn("Failed to load glyph for character: {}", static_cast<char>(c));
            continue;
        }

        // Generate texture
        uint32 texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character = {texture,
                               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               static_cast<uint32>(face->glyph->advance.x)};

        m_characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // Cleanup FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Logger::info("Font loaded successfully: {}", fontPath);
    return true;
}

void
TextRenderer::renderText(const String& text, float32 x, float32 y, float32 scale,
                         const glm::vec3& color)
{
    if (!m_initialized)
        return;

    // Activate corresponding render state
    m_shader.use();
    m_shader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    // Iterate through all characters
    for (auto c : text)
    {
        auto it = m_characters.find(c);
        if (it == m_characters.end())
            continue;

        Character ch = it->second;

        float32 xpos = x + ch.bearing.x * scale;
        float32 ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float32 w = ch.size.x * scale;
        float32 h = ch.size.y * scale;

        // Update VBO for each character
        float32 vertices[6][4] = {
            {    xpos, ypos + h, 0.0f, 0.0f},
            {    xpos,     ypos, 0.0f, 1.0f},
            {xpos + w,     ypos, 1.0f, 1.0f},

            {    xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w,     ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursors for next glyph
        x += (ch.advance >> 6) * scale;  // Bitshift by 6 to get value in pixels
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void
TextRenderer::updateScreenSize(int32 width, int32 height)
{
    m_projection = glm::ortho(0.0f, static_cast<float32>(width), 0.0f,
                              static_cast<float32>(height));

    m_shader.use();
    m_shader.setMat4("projection", m_projection);

    Logger::debug("TextRenderer screen size updated: {}x{}", width, height);
}

float32
TextRenderer::getTextWidth(const String& text, float32 scale) const
{
    float32 width = 0.0f;

    for (char c : text)
    {
        auto it = m_characters.find(c);
        if (it != m_characters.end())
        {
            const Character& ch = it->second;
            // Advance is in 1/64th pixels, convert to pixels
            width += static_cast<float32>(ch.advance >> 6) * scale;
        }
    }

    return width;
}

void
TextRenderer::setupRenderData()
{
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float32) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float32), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}  // namespace deadcode
