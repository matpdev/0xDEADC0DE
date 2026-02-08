/**
 * @file AudioManager.hpp
 * @brief Audio system management using Raylib
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#pragma once

#include "deadcode/core/Types.hpp"

namespace deadcode
{

/**
 * @brief Audio system manager
 *
 * Manages audio initialization, shutdown, and global volume control.
 */
class AudioManager
{
public:
    /**
     * @brief Constructor
     */
    AudioManager();

    /**
     * @brief Destructor
     */
    ~AudioManager();

    /**
     * @brief Initialize audio system
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Shutdown audio system
     */
    void shutdown();

    /**
     * @brief Set master volume
     * @param volume Volume level (0.0-1.0)
     */
    void setMasterVolume(float32 volume);

    /**
     * @brief Set music volume
     * @param volume Volume level (0.0-1.0)
     */
    void setMusicVolume(float32 volume);

    /**
     * @brief Set sound effects volume
     * @param volume Volume level (0.0-1.0)
     */
    void setSFXVolume(float32 volume);

    /**
     * @brief Get master volume
     * @return Current master volume (0.0-1.0)
     */
    [[nodiscard]] float32 getMasterVolume() const;

    /**
     * @brief Get music volume
     * @return Current music volume (0.0-1.0)
     */
    [[nodiscard]] float32 getMusicVolume() const;

    /**
     * @brief Get SFX volume
     * @return Current SFX volume (0.0-1.0)
     */
    [[nodiscard]] float32 getSFXVolume() const;

    // Delete copy and move
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;

private:
    bool m_initialized;
    float32 m_masterVolume;
    float32 m_musicVolume;
    float32 m_sfxVolume;
};

}  // namespace deadcode
