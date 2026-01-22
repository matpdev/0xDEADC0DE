/**
 * @file Config.cpp
 * @brief Implementation of the Config class
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#include "deadcode/core/Config.hpp"

#include "deadcode/core/Logger.hpp"

#include <fstream>
#include <sstream>

namespace deadcode
{

Config::Config() = default;

Config::~Config() = default;

bool
Config::load(const std::string& filepath)
{
    try
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            Logger::error("Failed to open config file: {}", filepath);
            return false;
        }

        file >> m_data;
        m_filepath = filepath;

        Logger::info("Loaded configuration from: {}", filepath);
        return true;
    }
    catch (const nlohmann::json::exception& e)
    {
        Logger::error("JSON parsing error in {}: {}", filepath, e.what());
        return false;
    }
}

bool
Config::reload()
{
    if (m_filepath.empty())
    {
        Logger::warn("Cannot reload config: no file previously loaded");
        return false;
    }

    return load(m_filepath);
}

bool
Config::save(const std::string& filepath) const
{
    try
    {
        std::ofstream file(filepath);
        if (!file.is_open())
        {
            Logger::error("Failed to open config file for writing: {}", filepath);
            return false;
        }

        file << m_data.dump(2);  // Pretty print with 2 space indentation
        Logger::info("Saved configuration to: {}", filepath);
        return true;
    }
    catch (const std::exception& e)
    {
        Logger::error("Error saving config to {}: {}", filepath, e.what());
        return false;
    }
}

bool
Config::has(const std::string& key) const
{
    return !getValueByPath(key).is_null();
}

bool
Config::remove(const std::string& key)
{
    // TODO: Implement path-based removal
    Logger::warn("Config::remove not yet implemented for: {}", key);
    return false;
}

void
Config::clear()
{
    m_data.clear();
}

const nlohmann::json&
Config::getData() const
{
    return m_data;
}

nlohmann::json
Config::getValueByPath(const std::string& path) const
{
    std::istringstream ss(path);
    std::string token;
    nlohmann::json current = m_data;

    while (std::getline(ss, token, '.'))
    {
        if (current.is_object() && current.contains(token))
        {
            current = current[token];
        }
        else
        {
            return nlohmann::json();  // Return null JSON
        }
    }

    return current;
}

void
Config::setValueByPath(const std::string& path, const nlohmann::json& value)
{
    std::istringstream ss(path);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '.'))
    {
        tokens.push_back(token);
    }

    if (tokens.empty())
    {
        return;
    }

    nlohmann::json* current = &m_data;

    for (size_t i = 0; i < tokens.size() - 1; ++i)
    {
        if (!current->is_object())
        {
            *current = nlohmann::json::object();
        }

        if (!current->contains(tokens[i]))
        {
            (*current)[tokens[i]] = nlohmann::json::object();
        }

        current = &(*current)[tokens[i]];
    }

    (*current)[tokens.back()] = value;
}

}  // namespace deadcode
