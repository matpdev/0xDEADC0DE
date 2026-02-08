/**
 * @file AudioManager.cpp
 * @brief Implementation of AudioManager class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-02-08
 */

#include "deadcode/audio/AudioManager.hpp"

#include "deadcode/core/Logger.hpp"

#include <raylib.h>

namespace deadcode
{

AudioManager::AudioManager()
    : m_initialized(false), m_masterVolume(1.0f), m_musicVolume(0.7f), m_sfxVolume(0.8f)
{
}

AudioManager::~AudioManager()
{
    if (m_initialized)
    {
        shutdown();
    }
}

bool
AudioManager::initialize()
{
    Logger::info("Initializing audio system...");

    InitAudioDevice();

    if (!IsAudioDeviceReady())
    {
        Logger::error("Failed to initialize audio device");
        return false;
    }

    SetMasterVolume(m_masterVolume);

    m_initialized = true;
    Logger::info("Audio system initialized successfully");
    return true;
}

void
AudioManager::shutdown()
{
    if (!m_initialized)
        return;

    Logger::info("Shutting down audio system...");

    CloseAudioDevice();

    m_initialized = false;
}

void
AudioManager::setMasterVolume(float32 volume)
{
    m_masterVolume = volume;
    if (m_initialized)
    {
        SetMasterVolume(volume);
    }
}

void
AudioManager::setMusicVolume(float32 volume)
{
    m_musicVolume = volume;
}

void
AudioManager::setSFXVolume(float32 volume)
{
    m_sfxVolume = volume;
}

float32
AudioManager::getMasterVolume() const
{
    return m_masterVolume;
}

float32
AudioManager::getMusicVolume() const
{
    return m_musicVolume;
}

float32
AudioManager::getSFXVolume() const
{
    return m_sfxVolume;
}

}  // namespace deadcode
