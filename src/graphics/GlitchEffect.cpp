/**
 * @file GlitchEffect.cpp
 * @brief Implementation of procedural glitch effect
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-30
 */

#include "deadcode/graphics/GlitchEffect.hpp"

#include "deadcode/core/Logger.hpp"

#include <cmath>
#include <random>

namespace deadcode
{

GlitchEffect::GlitchEffect()
    : m_config(),
      m_initialized(false),
      m_isGlitching(false),
      m_glitchTimer(0.0f),
      m_idleTimer(0.0f),
      m_currentIntensity(0.0f),
      m_elapsedTime(0.0f),
      m_distribution(0.0f, 1.0f),
      m_noiseSeed(0),
      m_screenWidth(1920),
      m_screenHeight(1080),
      m_resolutionScale(1.0f)
{
}

GlitchEffect::GlitchEffect(const GlitchConfig& config)
    : m_config(config),
      m_initialized(false),
      m_isGlitching(false),
      m_glitchTimer(0.0f),
      m_idleTimer(0.0f),
      m_currentIntensity(0.0f),
      m_elapsedTime(0.0f),
      m_distribution(0.0f, 1.0f),
      m_noiseSeed(0),
      m_screenWidth(1920),
      m_screenHeight(1080),
      m_resolutionScale(1.0f)
{
}

bool
GlitchEffect::initialize()
{
    if (m_initialized)
    {
        Logger::warn("GlitchEffect already initialized");
        return true;
    }

    Logger::info("Initializing GlitchEffect...");

    // Initialize random number generator
    std::random_device rd;
    m_randomEngine.seed(rd());
    m_noiseSeed = m_distribution(m_randomEngine) * 10000;

    m_idleTimer   = m_config.idleTime;
    m_initialized = true;

    Logger::info("GlitchEffect initialized successfully");
    return true;
}

void
GlitchEffect::update(float32 deltaTime)
{
    if (!m_initialized || !m_config.enabled)
        return;

    m_elapsedTime += deltaTime;

    if (m_isGlitching)
    {
        // Glitch is active
        m_glitchTimer += deltaTime;

        // Intensity ramps up quickly, then fades out
        float32 progress = m_glitchTimer / m_config.duration;
        if (progress < 0.2f)
        {
            // Quick ramp up
            m_currentIntensity = (progress / 0.5f) * m_config.intensity;
        }
        else
        {
            // Gradual fade out
            m_currentIntensity = m_config.intensity * (1.0f - (progress - 0.2f) / 0.8f);
        }

        // End glitch
        if (m_glitchTimer >= m_config.duration)
        {
            m_isGlitching      = false;
            m_glitchTimer      = 0.0f;
            m_currentIntensity = 0.0f;
            m_idleTimer        = m_config.idleTime;
            m_noiseSeed        = m_distribution(m_randomEngine) * 10000;
        }
    }
    else
    {
        // Idle state
        m_idleTimer -= deltaTime;

        if (m_idleTimer <= 0.0f)
        {
            // Trigger new glitch
            triggerGlitch();
        }
    }
}

void
GlitchEffect::reset()
{
    m_isGlitching      = false;
    m_glitchTimer      = 0.0f;
    m_idleTimer        = m_config.idleTime;
    m_currentIntensity = 0.0f;
    m_elapsedTime      = 0.0f;
    m_noiseSeed        = m_distribution(m_randomEngine) * 10000;
}

CharacterGlitchState
GlitchEffect::getCharacterState(uint32 charIndex, uint32 characterCount) const
{
    CharacterGlitchState state;
    state.offset          = glm::vec2(0.0f);
    state.colorMod        = glm::vec3(1.0f);
    state.alpha           = 1.0f;
    state.scale           = 1.0f;
    state.visible         = true;
    state.scanlinePhase   = 0.0f;
    state.duplicate       = false;
    state.duplicateOffset = glm::vec2(0.0f);
    state.sliceOffset     = 0.0f;
    state.inSliceZone     = false;

    if (!m_initialized || !m_config.enabled || !m_isGlitching)
        return state;

    float32 intensity = m_currentIntensity;

    // Text slicing effect - horizontal cuts with displacement
    if (m_config.textSlicing && intensity > 0.15f)
    {
        auto [sliceOffset, inZone] = calculateSliceDisplacement(charIndex, characterCount);
        state.sliceOffset          = sliceOffset * intensity;
        state.inSliceZone          = inZone;
        if (inZone)
        {
            state.offset.x += sliceOffset * intensity;
        }
    }

    // Block displacement - chunks of text move together
    if (m_config.blockDisplacement && intensity > 0.2f)
    {
        glm::vec2 blockOffset = calculateBlockDisplacement(charIndex, characterCount);
        state.offset += blockOffset * intensity;
    }

    // Character displacement
    if (m_config.characterDisplacement && intensity > 0.1f)
    {
        // Combine wave-based and random displacement
        glm::vec2 waveOffset   = calculateWaveDisplacement(charIndex, characterCount);
        glm::vec2 randomOffset = generateRandomOffset();

        float32 noise = generateNoise(charIndex, m_noiseSeed);

        // Apply displacement with noise modulation
        state.offset.x = (waveOffset.x * 0.7f + randomOffset.x * 0.3f) * intensity * noise;
        state.offset.y = (waveOffset.y * 0.7f + randomOffset.y * 0.3f) * intensity * noise;
    }

    // Character duplication
    if (m_config.textDuplication && intensity > 0.3f)
    {
        auto [shouldDuplicate, dupOffset] = calculateDuplication(charIndex);
        if (shouldDuplicate)
        {
            state.duplicate       = true;
            state.duplicateOffset = dupOffset * intensity * m_resolutionScale;
        }
    }

    // Enhanced chromatic aberration
    if (m_config.chromaticAberration && intensity > 0.2f)
    {
        float32 chromatic = m_config.chromaticIntensity;
        // More pronounced color separation
        float32 channelPhase = static_cast<float32>(charIndex % 3) / 3.0f;
        if (channelPhase < 0.33f)
        {
            state.colorMod = glm::vec3(1.0f + intensity * chromatic * 0.8f,
                                       1.0f - intensity * chromatic * 0.5f,
                                       1.0f - intensity * chromatic * 0.7f);
        }
        else if (channelPhase < 0.66f)
        {
            state.colorMod = glm::vec3(1.0f - intensity * chromatic * 0.5f,
                                       1.0f + intensity * chromatic * 0.8f,
                                       1.0f - intensity * chromatic * 0.5f);
        }
        else
        {
            state.colorMod = glm::vec3(1.0f - intensity * chromatic * 0.7f,
                                       1.0f - intensity * chromatic * 0.5f,
                                       1.0f + intensity * chromatic * 0.8f);
        }

        // Add glitch tint with more intensity
        state.colorMod += m_config.glitchColor * intensity * chromatic * 0.4f;
    }
    else if (m_config.rgbSeparation && intensity > 0.2f)
    {
        // Alternate color channels for adjacent characters
        float32 channelPhase = static_cast<float32>(charIndex % 3) / 3.0f;
        if (channelPhase < 0.33f)
        {
            state.colorMod = glm::vec3(1.0f + intensity * 0.5f, 1.0f - intensity * 0.3f,
                                       1.0f - intensity * 0.3f);
        }
        else if (channelPhase < 0.66f)
        {
            state.colorMod = glm::vec3(1.0f - intensity * 0.3f, 1.0f + intensity * 0.5f,
                                       1.0f - intensity * 0.3f);
        }
        else
        {
            state.colorMod = glm::vec3(1.0f - intensity * 0.3f, 1.0f - intensity * 0.3f,
                                       1.0f + intensity * 0.5f);
        }

        // Add glitch tint
        state.colorMod += m_config.glitchColor * intensity * 0.3f;
    }

    // Random corruption (hide characters)
    if (m_config.randomCorruption)
    {
        float32 corruptNoise = generateNoise(charIndex, m_noiseSeed + 1000);
        if (corruptNoise < m_config.corruptionChance * intensity)
        {
            state.visible = false;
        }
    }

    // Scanline effect
    if (m_config.scanlines)
    {
        state.scanlinePhase = std::fmod(m_elapsedTime * m_config.scanlineSpeed,
                                        m_config.scanlineHeight * 10.0f);
    }

    return state;
}

void
GlitchEffect::triggerGlitch()
{
    m_isGlitching      = true;
    m_glitchTimer      = 0.0f;
    m_currentIntensity = 0.0f;
    m_noiseSeed        = m_distribution(m_randomEngine) * 10000;

    Logger::debug("Glitch triggered!");
}

void
GlitchEffect::setScreenSize(int32 width, int32 height)
{
    m_screenWidth  = width;
    m_screenHeight = height;

    // Calculate resolution scale factor relative to 1920x1080 (FullHD reference)
    // Use the smaller dimension ratio to ensure effects are visible on all aspect ratios
    float32 widthScale  = static_cast<float32>(width) / 1920.0f;
    float32 heightScale = static_cast<float32>(height) / 1080.0f;
    m_resolutionScale   = std::min(widthScale, heightScale);

    // Clamp to reasonable range (don't go too small or too large)
    m_resolutionScale = std::max(0.3f, std::min(m_resolutionScale, 3.0f));

    Logger::debug("GlitchEffect screen size updated: {}x{}, scale: {}", width, height,
                  m_resolutionScale);
}

glm::vec2
GlitchEffect::generateRandomOffset() const
{
    float32 x = (m_distribution(m_randomEngine) - 0.5f) * 2.0f * m_config.maxJitter *
                m_resolutionScale;
    float32 y = (m_distribution(m_randomEngine) - 0.5f) * 2.0f * m_config.verticalJitter *
                m_resolutionScale;
    return glm::vec2(x, y);
}

float32
GlitchEffect::generateNoise(uint32 charIndex, uint32 seed) const
{
    // Simple pseudo-random noise using sine waves
    uint32 n    = charIndex * 374761393 + seed * 668265263;
    n           = (n ^ (n >> 13)) * 1274126177;
    float32 val = static_cast<float32>((n ^ (n >> 16)) & 0xFFFFFF) / 16777216.0f;
    return val;
}

glm::vec2
GlitchEffect::calculateWaveDisplacement(uint32 charIndex, uint32 characterCount) const
{
    if (characterCount == 0)
        return glm::vec2(0.0f);

    float32 normalizedPos = static_cast<float32>(charIndex) / static_cast<float32>(characterCount);

    // Multiple sine waves for complex motion
    float32 wave1 = std::sin(m_elapsedTime * 10.0f + normalizedPos * 20.0f);
    float32 wave2 = std::sin(m_elapsedTime * 7.3f + normalizedPos * 15.0f + 1.5f);
    float32 wave3 = std::cos(m_elapsedTime * 13.7f - normalizedPos * 10.0f);

    float32 x = (wave1 * 0.5f + wave2 * 0.3f) * m_config.maxJitter * m_resolutionScale;
    float32 y = wave3 * 0.2f * m_config.verticalJitter * m_resolutionScale;

    return glm::vec2(x, y);
}

std::pair<float32, bool>
GlitchEffect::calculateSliceDisplacement(uint32 charIndex, uint32 characterCount) const
{
    if (characterCount == 0)
        return {0.0f, false};

    float32 normalizedPos = static_cast<float32>(charIndex) / static_cast<float32>(characterCount);

    // Create multiple horizontal "slices" that can move independently
    // Use time-based noise to create moving slice zones
    float32 sliceNoise = generateNoise(static_cast<uint32>(m_elapsedTime * 10.0f), m_noiseSeed);
    float32 sliceZoneCenter = sliceNoise;  // 0-1 range

    // Check if character is in a slice zone
    float32 distanceFromCenter = std::abs(normalizedPos - sliceZoneCenter);
    bool inSliceZone           = distanceFromCenter < m_config.sliceHeight;

    if (!inSliceZone)
        return {0.0f, false};

    // Calculate slice offset - characters in the same zone move together
    float32 sliceDirection =
        (generateNoise(static_cast<uint32>(sliceZoneCenter * 1000), m_noiseSeed) > 0.5f) ? 1.0f
                                                                                         : -1.0f;
    float32 sliceIntensity = 1.0f -
                             (distanceFromCenter / m_config.sliceHeight);  // Stronger at center
    float32 offset = sliceDirection * m_config.maxSliceOffset * sliceIntensity * m_resolutionScale;

    return {offset, true};
}

glm::vec2
GlitchEffect::calculateBlockDisplacement(uint32 charIndex, uint32 characterCount) const
{
    if (characterCount == 0)
        return glm::vec2(0.0f);

    // Divide text into blocks that move together
    uint32 blockSize  = std::max(1u, static_cast<uint32>(characterCount * m_config.blockSize));
    uint32 blockIndex = charIndex / blockSize;

    // Each block gets a consistent random offset
    float32 blockNoiseX = generateNoise(blockIndex, m_noiseSeed + 500);
    float32 blockNoiseY = generateNoise(blockIndex, m_noiseSeed + 600);

    // Only displace some blocks randomly
    if (blockNoiseX < 0.6f)
        return glm::vec2(0.0f);

    float32 x = (blockNoiseX - 0.5f) * 2.0f * m_config.maxBlockOffset * m_resolutionScale;
    float32 y = (blockNoiseY - 0.5f) * 2.0f * m_config.maxBlockOffset * 0.5f * m_resolutionScale;

    return glm::vec2(x, y);
}

std::pair<bool, glm::vec2>
GlitchEffect::calculateDuplication(uint32 charIndex) const
{
    // Randomly duplicate some characters
    float32 dupNoise = generateNoise(charIndex, m_noiseSeed + 700);

    if (dupNoise > m_config.duplicationChance)
        return {false, glm::vec2(0.0f)};

    // Calculate offset for duplicated character
    float32 offsetX = (generateNoise(charIndex, m_noiseSeed + 800) - 0.5f) * 10.0f;
    float32 offsetY = (generateNoise(charIndex, m_noiseSeed + 900) - 0.5f) * 5.0f;

    return {true, glm::vec2(offsetX, offsetY)};
}

}  // namespace deadcode
