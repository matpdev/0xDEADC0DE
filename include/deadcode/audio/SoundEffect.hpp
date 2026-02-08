/**
 * @file SoundEffect.hpp
 * @brief Sound effect playback using Raylib
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"

#include <raylib.h>

namespace deadcode
{

/**
 * @brief Sound effect player
 *
 * Loads and plays short sound effects (WAV, OGG, MP3).
 */
class SoundEffect
{
public:
    /**
     * @brief Constructor
     */
    SoundEffect();

    /**
     * @brief Destructor
     */
    ~SoundEffect();

    /**
     * @brief Load sound effect from file
     *
     * @param filePath Path to audio file (WAV, OGG, MP3)
     * @return true if successful
     */
    bool load(const String& filePath);

    /**
     * @brief Play the sound effect
     */
    void play();

    /**
     * @brief Stop the sound effect
     */
    void stop();

    /**
     * @brief Set volume
     * @param volume Volume level (0.0-1.0)
     */
    void setVolume(float32 volume);

    /**
     * @brief Check if sound is loaded
     * @return true if loaded
     */
    [[nodiscard]] bool isLoaded() const;

    // Delete copy constructor/assignment
    SoundEffect(const SoundEffect&)            = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

    // Allow move
    SoundEffect(SoundEffect&& other) noexcept;
    SoundEffect& operator=(SoundEffect&& other) noexcept;

private:
    Sound m_sound;
    bool m_loaded;
};

}  // namespace deadcode
