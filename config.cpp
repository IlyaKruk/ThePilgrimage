#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void GameConfig::LoadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = line.substr(0, line.find('#'));
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(), line.end());

        if (!line.empty()) {
            ParseConfigLine(line);
        }
    }
}

void GameConfig::ParseConfigLine(const std::string& line) {
    size_t delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
        value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

        configValues[key] = value;
    }
}

int GameConfig::GetInt(const std::string& key, int defaultValue) const {
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        try {
            return std::stoi(it->second);
        }
        catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

float GameConfig::GetFloat(const std::string& key, float defaultValue) const {
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        try {
            return std::stof(it->second);
        }
        catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

std::string GameConfig::GetString(const std::string& key, const std::string& defaultValue) const {
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        return it->second;
    }
    return defaultValue;
}