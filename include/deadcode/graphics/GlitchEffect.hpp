/**
 * @file GlitchEffect.hpp
 * @brief Procedural glitch effect for text rendering
 *
 * Provides configurable glitch effects including character displacement,
 * color distortion, and temporal variations for cyberpunk aesthetics.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-30
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <random>
#include <vector>

namespace deadcode
{

/**
 * @brief Glitch effect types
 */
enum class GlitchType
{
    JITTER,     ///< Random jittering
    SLICE,      ///< Horizontal slicing with offset
    BLOCK,      ///< Block displacement
    DUPLICATE,  ///< Character duplication
    WAVE,       ///< Wave distortion
    CHROMATIC,  ///< Chromatic aberration
    ALL         ///< Combination of all effects
};

/**
 * @brief Per-character glitch state
 */
struct CharacterGlitchState
{
    glm::vec2 offset;           ///< Position offset in pixels
    glm::vec3 colorMod;         ///< Color modulation (RGB multiplier)
    float32 alpha;              ///< Alpha override
    float32 scale;              ///< Scale multiplier
    bool visible;               ///< Visibility toggle
    float32 scanlinePhase;      ///< Scanline effect phase
    bool duplicate;             ///< Should duplicate this character
    glm::vec2 duplicateOffset;  ///< Offset for duplicated character
    float32 sliceOffset;        ///< Horizontal slice displacement
    bool inSliceZone;           ///< Is character in a slice zone
};

/**
 * @brief Glitch effect configuration
 */
struct GlitchConfig
{
    bool enabled = true;

    // Timing
    float32 frequency = 3.0f;  ///< Glitches per second
    float32 duration  = 0.1f;  ///< Duration of each glitch (seconds)
    float32 idleTime  = 2.0f;  ///< Time between glitches (seconds)

    // Character displacement
    bool characterDisplacement = true;
    float32 maxJitter          = 3.0f;  ///< Max horizontal jitter (pixels)
    float32 verticalJitter     = 1.0f;  ///< Max vertical jitter (pixels)

    // Color effects
    bool rgbSeparation          = true;
    float32 rgbSeparationAmount = 2.0f;                         ///< RGB channel offset (pixels)
    glm::vec3 glitchColor       = glm::vec3(1.0f, 0.0f, 0.0f);  ///< Glitch tint color

    // Scanline effect
    bool scanlines         = false;
    float32 scanlineSpeed  = 50.0f;  ///< Scanline scroll speed (pixels/sec)
    float32 scanlineHeight = 2.0f;   ///< Height of scanline bands

    // Intensity
    float32 intensity = 0.8f;  ///< Overall effect intensity (0-1)

    // Random character corruption
    bool randomCorruption    = true;
    float32 corruptionChance = 0.1f;  ///< Chance per char (0-1)

    // Advanced effects
    GlitchType glitchType      = GlitchType::ALL;
    bool textSlicing           = true;   ///< Enable text slicing
    float32 sliceHeight        = 0.15f;  ///< Height of slice zones (0-1)
    float32 maxSliceOffset     = 30.0f;  ///< Max horizontal slice offset
    bool textDuplication       = true;   ///< Enable character duplication
    float32 duplicationChance  = 0.15f;  ///< Chance to duplicate (0-1)
    bool blockDisplacement     = true;   ///< Enable block displacement
    float32 blockSize          = 0.2f;   ///< Size of displaced blocks (0-1)
    float32 maxBlockOffset     = 20.0f;  ///< Max block offset
    bool chromaticAberration   = true;   ///< Enhanced chromatic aberration
    float32 chromaticIntensity = 1.5f;   ///< Chromatic aberration strength
};

/**
 * @brief Procedural glitch effect generator
 *
 * Generates time-based glitch effects for text rendering including
 * character displacement, color distortion, and visual artifacts.
 */
class GlitchEffect
{
public:
    /**
     * @brief Constructor with default configuration
     */
    GlitchEffect();

    /**
     * @brief Constructor with custom configuration
     * @param config Glitch configuration
     */
    explicit GlitchEffect(const GlitchConfig& config);

    /**
     * @brief Initialize glitch effect system
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Update glitch state
     * @param deltaTime Time since last update (seconds)
     */
    void update(float32 deltaTime);

    /**
     * @brief Reset glitch effect to initial state
     */
    void reset();

    /**
     * @brief Get glitch state for a specific character
     * @param charIndex Character index in string
     * @param characterCount Total number of characters
     * @return Glitch state for the character
     */
    CharacterGlitchState getCharacterState(uint32 charIndex, uint32 characterCount) const;

    /**
     * @brief Check if glitch is currently active
     * @return true if glitching
     */
    bool
    isActive() const
    {
        return m_isGlitching;
    }

    /**
     * @brief Get current glitch intensity (0-1)
     * @return Intensity value
     */
    float32
    getCurrentIntensity() const
    {
        return m_currentIntensity;
    }

    /**
     * @brief Set glitch configuration
     * @param config New configuration
     */
    void
    setConfig(const GlitchConfig& config)
    {
        m_config = config;
    }

    /**
     * @brief Get current configuration
     * @return Current glitch config
     */
    const GlitchConfig&
    getConfig() const
    {
        return m_config;
    }

    /**
     * @brief Enable/disable glitch effect
     * @param enabled Enable state
     */
    void
    setEnabled(bool enabled)
    {
        m_config.enabled = enabled;
    }

    /**
     * @brief Trigger a manual glitch
     */
    void triggerGlitch();

    /**
     * @brief Set screen dimensions for resolution-scaled effects
     * @param width Screen width in pixels
     * @param height Screen height in pixels
     */
    void setScreenSize(int32 width, int32 height);

private:
    /**
     * @brief Generate random offset for character
     * @return Random 2D offset
     */
    glm::vec2 generateRandomOffset() const;

    /**
     * @brief Generate noise value for character
     * @param charIndex Character index
     * @param seed Noise seed
     * @return Noise value (0-1)
     */
    float32 generateNoise(uint32 charIndex, uint32 seed) const;

    /**
     * @brief Calculate wave-based displacement
     * @param charIndex Character index
     * @param characterCount Total character count
     * @return Displacement offset
     */
    glm::vec2 calculateWaveDisplacement(uint32 charIndex, uint32 characterCount) const;

    /**
     * @brief Calculate text slice displacement
     * @param charIndex Character index
     * @param characterCount Total character count
     * @return Slice offset and in-zone flag
     */
    std::pair<float32, bool> calculateSliceDisplacement(uint32 charIndex,
                                                        uint32 characterCount) const;

    /**
     * @brief Calculate block displacement
     * @param charIndex Character index
     * @param characterCount Total character count
     * @return Block offset
     */
    glm::vec2 calculateBlockDisplacement(uint32 charIndex, uint32 characterCount) const;

    /**
     * @brief Check if character should be duplicated
     * @param charIndex Character index
     * @return Duplication info (should duplicate, offset)
     */
    std::pair<bool, glm::vec2> calculateDuplication(uint32 charIndex) const;

    GlitchConfig m_config;
    bool m_initialized;

    // State
    bool m_isGlitching;
    float32 m_glitchTimer;
    float32 m_idleTimer;
    float32 m_currentIntensity;
    float32 m_elapsedTime;

    // Procedural generation
    mutable std::mt19937 m_randomEngine;
    mutable std::uniform_real_distribution<float32> m_distribution;
    uint32 m_noiseSeed;

    // Screen dimensions for resolution scaling
    int32 m_screenWidth;
    int32 m_screenHeight;
    float32 m_resolutionScale;  ///< Scale factor based on resolution (1.0 at 1920x1080)
};

}  // namespace deadcode
