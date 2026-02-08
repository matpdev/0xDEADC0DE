/**
 * @file MusicPlayer.cpp
 * @brief Implementation of MusicPlayer class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/audio/MusicPlayer.hpp"

#include "deadcode/core/Logger.hpp"

#include <raylib.h>

namespace deadcode
{

MusicPlayer::MusicPlayer() : m_loaded(false), m_looping(false)
{
    // Initialize to empty music
    m_music = {0};
}

MusicPlayer::~MusicPlayer()
{
    if (m_loaded)
    {
        UnloadMusicStream(m_music);
    }
}

MusicPlayer::MusicPlayer(MusicPlayer&& other) noexcept
    : m_music(other.m_music), m_loaded(other.m_loaded), m_looping(other.m_looping)
{
    other.m_music  = {0};
    other.m_loaded = false;
}

MusicPlayer&
MusicPlayer::operator=(MusicPlayer&& other) noexcept
{
    if (this != &other)
    {
        if (m_loaded)
        {
            UnloadMusicStream(m_music);
        }

        m_music        = other.m_music;
        m_loaded       = other.m_loaded;
        m_looping      = other.m_looping;
        other.m_music  = {0};
        other.m_loaded = false;
    }
    return *this;
}

bool
MusicPlayer::load(const String& filePath)
{
    Logger::info("Loading music: {}", filePath);

    // Unload previous music if any
    if (m_loaded)
    {
        UnloadMusicStream(m_music);
        m_loaded = false;
    }

    m_music = LoadMusicStream(filePath.c_str());

    // Check if music loaded successfully (stream.buffer will be null on failure)
    if (m_music.stream.buffer == nullptr)
    {
        Logger::error("Failed to load music: {}", filePath);
        return false;
    }

    m_loaded = true;
    Logger::info("Music loaded successfully: {}", filePath);
    return true;
}

void
MusicPlayer::play()
{
    if (m_loaded)
    {
        PlayMusicStream(m_music);
        m_music.looping = m_looping;
    }
}

void
MusicPlayer::stop()
{
    if (m_loaded)
    {
        StopMusicStream(m_music);
    }
}

void
MusicPlayer::pause()
{
    if (m_loaded)
    {
        PauseMusicStream(m_music);
    }
}

void
MusicPlayer::resume()
{
    if (m_loaded)
    {
        ResumeMusicStream(m_music);
    }
}

void
MusicPlayer::setVolume(float32 volume)
{
    if (m_loaded)
    {
        SetMusicVolume(m_music, volume);
    }
}

void
MusicPlayer::setLooping(bool loop)
{
    m_looping       = loop;
    m_music.looping = loop;
}

void
MusicPlayer::update()
{
    if (m_loaded && isPlaying())
    {
        UpdateMusicStream(m_music);
    }
}

bool
MusicPlayer::isLoaded() const
{
    return m_loaded;
}

bool
MusicPlayer::isPlaying() const
{
    if (!m_loaded)
        return false;

    return IsMusicStreamPlaying(m_music);
}

}  // namespace deadcode
