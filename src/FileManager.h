#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <SPIFFS.h>
#include <Arduino.h>

enum class FileType {
    // Image types
    IMAGE,
    BACKGROUND,
    TRANSPARENT,
    
    // Data types
    TRAINING,
    MODEL_CONFIG,
    
    // Theme types
    THEME,
    THEME_LAYOUT,
    THEME_COLORS,
    
    // Config types
    CONFIG,
    USER_PREFS
};

class FileManager {
public:
    FileManager();
    bool begin();
    
    // File saving methods
    void saveImageData(const uint8_t* data, uint16_t size, FileType type);
    void saveTransparentImageData(const uint8_t* colorData, const uint8_t* alphaData, uint16_t width, uint16_t height, FileType type);
    void saveTrainingData(const uint8_t* data, uint16_t size, const char* modelName);
    void saveConfigData(const uint8_t* data, uint16_t size, FileType type);
    static bool saveThemeData(const uint8_t* data, uint16_t length, FileType type);
    
    // Utility functions
    bool fileExists(const char* path);
    void listFiles();
    bool modelExists(const char* modelName);
    
    // Utility methods
    uint32_t getTotalSpace() { return SPIFFS.totalBytes(); }
    uint32_t getUsedSpace() { return SPIFFS.usedBytes(); }
    uint32_t getFreeSpace() { return SPIFFS.totalBytes() - SPIFFS.usedBytes(); }
    bool format() { return SPIFFS.format(); }
    
    // Check if we have enough space for a file
    bool hasEnoughSpace(uint32_t size) {
        return (getFreeSpace() > size + 1024); // 1KB safety margin
    }

private:
    const char* getTypeString(FileType type);
    void writeHeaderGuard(File& file, const char* name, bool opening);
    void writeArrayDeclaration(File& file, const char* name, const uint8_t* data, uint16_t size);
};

#endif 