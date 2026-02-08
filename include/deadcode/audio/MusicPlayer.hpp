/**
 * @file MusicPlayer.hpp
 * @brief Background music streaming using Raylib
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
 * @brief Music streaming player
 *
 * Streams and plays background music (OGG, MP3, WAV, FLAC).
 * IMPORTANT: Must call update() every frame for streaming to work.
 */
class MusicPlayer
{
public:
    /**
     * @brief Constructor
     */
    MusicPlayer();

    /**
     * @brief Destructor
     */
    ~MusicPlayer();

    /**
     * @brief Load music from file
     *
     * @param filePath Path to audio file (OGG, MP3, WAV, FLAC)
     * @return true if successful
     */
    bool load(const String& filePath);

    /**
     * @brief Play the music
     */
    void play();

    /**
     * @brief Stop the music
     */
    void stop();

    /**
     * @brief Pause the music
     */
    void pause();

    /**
     * @brief Resume paused music
     */
    void resume();

    /**
     * @brief Set volume
     * @param volume Volume level (0.0-1.0)
     */
    void setVolume(float32 volume);

    /**
     * @brief Set looping
     * @param loop true to loop music
     */
    void setLooping(bool loop);

    /**
     * @brief Update music streaming
     *
     * CRITICAL: Must be called every frame for streaming to work!
     */
    void update();

    /**
     * @brief Check if music is loaded
     * @return true if loaded
     */
    [[nodiscard]] bool isLoaded() const;

    /**
     * @brief Check if music is playing
     * @return true if playing
     */
    [[nodiscard]] bool isPlaying() const;

    // Delete copy constructor/assignment
    MusicPlayer(const MusicPlayer&)            = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;

    // Allow move
    MusicPlayer(MusicPlayer&& other) noexcept;
    MusicPlayer& operator=(MusicPlayer&& other) noexcept;

private:
    Music m_music;
    bool m_loaded;
    bool m_looping;
};

}  // namespace deadcode
