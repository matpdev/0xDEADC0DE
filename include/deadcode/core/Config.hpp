/**
 * @file Config.hpp
 * @brief Configuration management system using JSON
 *
 * Provides type-safe access to configuration values loaded from JSON files
 * with support for defaults and runtime updates.
 *
 * @author 0xDEADC0DE Team
 * @date 2026-01-21
 */

#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace deadcode
{

/**
 * @brief Configuration manager for game settings
 *
 * Loads and manages configuration from JSON files.
 * Supports:
 * - Hot-reloading of configuration
 * - Type-safe value access
 * - Default values
 * - Hierarchical key access (e.g., "graphics.resolution.width")
 */
class Config
{
public:
    /**
     * @brief Constructor
     */
    Config();

    /**
     * @brief Destructor
     */
    ~Config();

    /**
     * @brief Load configuration from a JSON file
     *
     * @param filepath Path to the JSON configuration file
     * @return true if loaded successfully
     */
    bool load(const std::string& filepath);

    /**
     * @brief Reload configuration from the last loaded file
     *
     * @return true if reloaded successfully
     */
    bool reload();

    /**
     * @brief Save current configuration to a file
     *
     * @param filepath Path to save the configuration
     * @return true if saved successfully
     */
    bool save(const std::string& filepath) const;

    /**
     * @brief Get a configuration value
     *
     * @tparam T Type of the value to retrieve
     * @param key Configuration key (supports dot notation)
     * @param defaultValue Default value if key doesn't exist
     * @return The configuration value or default
     */
    template <typename T>
    [[nodiscard]] T
    get(const std::string& key, const T& defaultValue = T{}) const
    {
        try
        {
            auto value = getValueByPath(key);
            if (value.is_null())
            {
                return defaultValue;
            }
            return value.get<T>();
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    /**
     * @brief Set a configuration value
     *
     * @tparam T Type of the value to set
     * @param key Configuration key (supports dot notation)
     * @param value Value to set
     */
    template <typename T>
    void
    set(const std::string& key, const T& value)
    {
        setValueByPath(key, value);
    }

    /**
     * @brief Check if a configuration key exists
     *
     * @param key Configuration key to check
     * @return true if key exists
     */
    [[nodiscard]] bool has(const std::string& key) const;

    /**
     * @brief Remove a configuration key
     *
     * @param key Configuration key to remove
     * @return true if key was removed
     */
    bool remove(const std::string& key);

    /**
     * @brief Clear all configuration
     */
    void clear();

    /**
     * @brief Get the raw JSON data
     *
     * @return Reference to the JSON object
     */
    [[nodiscard]] const nlohmann::json& getData() const;

private:
    /**
     * @brief Get a value by dot-separated path
     *
     * @param path Dot-separated path (e.g., "graphics.resolution.width")
     * @return JSON value at path or null
     */
    [[nodiscard]] nlohmann::json getValueByPath(const std::string& path) const;

    /**
     * @brief Set a value by dot-separated path
     *
     * @param path Dot-separated path
     * @param value Value to set
     */
    void setValueByPath(const std::string& path, const nlohmann::json& value);

    nlohmann::json m_data;
    std::string m_filepath;
};

}  // namespace deadcode
