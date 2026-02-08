/**
 * @file SoundEffect.cpp
 * @brief Implementation of SoundEffect class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/audio/SoundEffect.hpp"

#include "deadcode/core/Logger.hpp"

#include <raylib.h>

namespace deadcode
{

SoundEffect::SoundEffect() : m_loaded(false)
{
    // Initialize to empty sound
    m_sound = {0};
}

SoundEffect::~SoundEffect()
{
    if (m_loaded)
    {
        UnloadSound(m_sound);
    }
}

SoundEffect::SoundEffect(SoundEffect&& other) noexcept : m_sound(other.m_sound), m_loaded(other.m_loaded)
{
    other.m_sound  = {0};
    other.m_loaded = false;
}

SoundEffect&
SoundEffect::operator=(SoundEffect&& other) noexcept
{
    if (this != &other)
    {
        if (m_loaded)
        {
            UnloadSound(m_sound);
        }

        m_sound        = other.m_sound;
        m_loaded       = other.m_loaded;
        other.m_sound  = {0};
        other.m_loaded = false;
    }
    return *this;
}

bool
SoundEffect::load(const String& filePath)
{
    Logger::info("Loading sound effect: {}", filePath);

    // Unload previous sound if any
    if (m_loaded)
    {
        UnloadSound(m_sound);
        m_loaded = false;
    }

    m_sound = LoadSound(filePath.c_str());

    // Check if sound loaded successfully (stream.buffer will be null on failure)
    if (m_sound.stream.buffer == nullptr)
    {
        Logger::error("Failed to load sound effect: {}", filePath);
        return false;
    }

    m_loaded = true;
    Logger::info("Sound effect loaded successfully: {}", filePath);
    return true;
}

void
SoundEffect::play()
{
    if (m_loaded)
    {
        PlaySound(m_sound);
    }
}

void
SoundEffect::stop()
{
    if (m_loaded)
    {
        StopSound(m_sound);
    }
}

void
SoundEffect::setVolume(float32 volume)
{
    if (m_loaded)
    {
        SetSoundVolume(m_sound, volume);
    }
}

bool
SoundEffect::isLoaded() const
{
    return m_loaded;
}

}  // namespace deadcode
