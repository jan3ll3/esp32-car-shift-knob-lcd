#include "FileManager.h"

FileManager::FileManager() {
    // Constructor implementation
}

bool FileManager::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return false;
    }
    return true;
}

void FileManager::saveImageData(const uint8_t* data, uint16_t size, FileType type) {
    char filename[32];
    const char* typeStr = getTypeString(type);
    snprintf(filename, sizeof(filename), "/spiffs/%s_%d.img", typeStr, millis());
    
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to create image file");
        return;
    }
    
    uint16_t width = sqrt(size / 2);  // Assuming square images
    uint16_t height = width;
    file.write((uint8_t*)&width, sizeof(width));
    file.write((uint8_t*)&height, sizeof(height));
    file.write(data, size);
    file.close();
}

void FileManager::saveTransparentImageData(const uint8_t* colorData, const uint8_t* alphaData, uint16_t width, uint16_t height, FileType type) {
    char filename[32];
    const char* typeStr = getTypeString(type);
    snprintf(filename, sizeof(filename), "/spiffs/%s_%d.timg", typeStr, millis());
    
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to create transparent image file");
        return;
    }
    
    file.write((uint8_t*)&width, sizeof(width));
    file.write((uint8_t*)&height, sizeof(height));
    file.write(colorData, width * height * 2);  // 2 bytes per pixel for RGB565
    file.write(alphaData, width * height);  // 1 byte per pixel for alpha
    file.close();
}

void FileManager::saveTrainingData(const uint8_t* data, uint16_t size, const char* modelName) {
    char filename[32];
    snprintf(filename, sizeof(filename), "/spiffs/%s_%d.dat", modelName, millis());
    
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to create training data file");
        return;
    }
    
    file.write(data, size);
    file.close();
}

bool FileManager::fileExists(const char* path) {
    return SPIFFS.exists(path);
}

void FileManager::listFiles() {
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.print("FILE: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
}

bool FileManager::modelExists(const char* modelName) {
    char filename[32];
    snprintf(filename, sizeof(filename), "/spiffs/%s.tflite", modelName);
    return fileExists(filename);
}

const char* FileManager::getTypeString(FileType type) {
    switch (type) {
        case FileType::IMAGE: return "image";
        case FileType::BACKGROUND: return "background";
        case FileType::TRANSPARENT: return "transparent";
        case FileType::TRAINING: return "training";
        case FileType::MODEL_CONFIG: return "model_config";
        case FileType::THEME: return "theme";
        case FileType::THEME_LAYOUT: return "theme_layout";
        case FileType::THEME_COLORS: return "theme_colors";
        case FileType::CONFIG: return "config";
        case FileType::USER_PREFS: return "user_prefs";
        default: return "unknown";
    }
}

void FileManager::writeHeaderGuard(File& file, const char* name, bool opening) {
    // Implementation for writing header guards
}

void FileManager::writeArrayDeclaration(File& file, const char* name, const uint8_t* data, uint16_t size) {
    // Implementation for writing array declarations
}

bool FileManager::saveThemeData(const uint8_t* data, uint16_t size, FileType type) {
    String filename;
    switch (type) {
        case FileType::THEME:
            filename = "/theme.dat";
            break;
        default:
            return false;
    }

    File file = SPIFFS.open(filename, "w");
    if (!file) {
        return false;
    }

    size_t written = file.write(data, size);
    file.close();
    
    return written == size;  // Return true if all bytes were written
}

void FileManager::saveConfigData(const uint8_t* data, uint16_t size, FileType type) {
    String filename;
    switch (type) {
        case FileType::CONFIG:
            filename = "/config.dat";
            break;
        default:
            return;
    }

    File file = SPIFFS.open(filename, "w");
    if (!file) {
        return;
    }

    file.write(data, size);
    file.close();
} 