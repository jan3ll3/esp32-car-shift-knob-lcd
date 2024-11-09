#ifndef PACKET_DEFINITIONS_H
#define PACKET_DEFINITIONS_H

#include <Arduino.h>

// Main Command Types
enum CommandID {
    CMD_INIT = 0x01,            // Initialize connection
    CMD_THEMES = 0x02,          // Theme-related commands
    CMD_DATA = 0x03,            // Data transfer commands
    CMD_CONFIG = 0x04,          // Configuration commands
    CMD_ACK = 0x05,             // Acknowledgment
    CMD_NACK = 0x06,            // Negative acknowledgment
    CMD_STATUS = 0x07,          // Status information
    CMD_NOTIFICATION = 0x08     // Notification data
};

// Data Subtypes (for CMD_DATA)
enum DataType {
    // Image Types (0x10 - 0x1F)
    DATA_IMAGE = 0x10,          // Standard image
    DATA_BACKGROUND = 0x11,     // Background image
    DATA_TRANSPARENT = 0x12,    // Image with transparency
    
    // Training/Model Types (0x20 - 0x2F)
    DATA_TRAINING = 0x20,       // Training/Calibration data
    DATA_MODEL_WEIGHTS = 0x21,  // Model weights
    DATA_MODEL_CONFIG = 0x22,   // Model configuration
    
    // Theme Data Types (0x30 - 0x3F)
    DATA_THEME_CONFIG = 0x30,   // Theme configuration
    DATA_THEME_LAYOUT = 0x31,   // Theme layout data
    DATA_THEME_COLORS = 0x32,   // Theme color data
    
    // Device Config Types (0x40 - 0x4F)
    DATA_DEVICE_CONFIG = 0x40,  // Device configuration
    DATA_USER_PREFS = 0x41      // User preferences
};

// Theme Command Subtypes (for CMD_THEMES)
enum ThemeCommand {
    THEME_LIST = 0x01,          // List available themes
    THEME_SELECT = 0x02,        // Select active theme
    THEME_DELETE = 0x03,        // Delete a theme
    THEME_UPDATE = 0x04,        // Update theme settings
    THEME_CREATE = 0x05         // Create new theme
};

// Config Command Subtypes (for CMD_CONFIG)
enum ConfigCommand {
    CONFIG_GET = 0x01,          // Get configuration
    CONFIG_SET = 0x02,          // Set configuration
    CONFIG_RESET = 0x03,        // Reset to defaults
    CONFIG_SAVE = 0x04,         // Save current config
    CONFIG_LOAD = 0x05          // Load saved config
};

// Status Command Subtypes (for CMD_STATUS)
enum StatusCommand {
    STATUS_BATTERY = 0x01,      // Battery status
    STATUS_MEMORY = 0x02,       // Memory usage
    STATUS_SYSTEM = 0x03,       // System status
    STATUS_ERRORS = 0x04,       // Error log
    STATUS_PERFORMANCE = 0x05   // Performance metrics
};

// ACK Subtypes
enum AckType {
    ACK_METADATA = 0x00,        // Metadata received
    ACK_DATA_CHUNK = 0x01,      // Data chunk received
    ACK_COMPLETE = 0x02,        // Transfer complete
    ACK_CONFIG = 0x03,          // Configuration accepted
    ACK_THEME = 0x04           // Theme command completed
};

// NACK Error Codes
enum NackError {
    ERR_INVALID_COMMAND = 0x01,     // Invalid command
    ERR_CHECKSUM = 0x02,            // Checksum mismatch
    ERR_PACKET_SIZE = 0x03,         // Invalid packet size
    ERR_PACKET_INDEX = 0x04,        // Invalid packet index
    ERR_BUFFER_OVERFLOW = 0x05,     // Buffer overflow
    ERR_INVALID_STATE = 0x06,       // Invalid state for command
    ERR_STORAGE_FULL = 0x07,        // Storage full
    ERR_FILE_ERROR = 0x08,          // File operation error
    ERR_INVALID_TYPE = 0x09,        // Invalid data type
    ERR_AUTHENTICATION = 0x0A       // Authentication failed
};

// Packet structure
struct Packet {
    uint8_t commandId;      // Main command type
    uint8_t length;         // Payload length
    uint8_t payload[256];   // Payload data
    uint8_t checksum;       // Packet checksum
};

// Metadata structure (for first packet in data transfers)
struct DataMetadata {
    uint8_t indicator;      // Always 0x00
    uint8_t dataType;       // Type from DataType enum
    uint16_t totalPackets;  // Total number of packets
    uint32_t totalSize;     // Total size in bytes
    uint8_t flags;         // Optional flags for special handling
    uint8_t reserved[3];   // Reserved for future use
};

#endif // PACKET_DEFINITIONS_H 