#ifndef PILGRIMAGE_CONFIG_H
#define PILGRIMAGE_CONFIG_H

#include <string>
#include <unordered_map>

class GameConfig {
public:
    static GameConfig& GetInstance() {
        static GameConfig instance;
        return instance;
    }

    void LoadConfig(const std::string& filename);
    int GetInt(const std::string& key, int defaultValue) const;
    float GetFloat(const std::string& key, float defaultValue) const;
    std::string GetString(const std::string& key, const std::string& defaultValue) const;

private:
    GameConfig() = default;
    std::unordered_map<std::string, std::string> configValues;

    void ParseConfigLine(const std::string& line);
};

#endif 