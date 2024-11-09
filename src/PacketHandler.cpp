#include "PacketHandler.h"
#include "DeviceState.h"
#include "FileManager.h"
#include "BLESerialManager.h"

PacketHandler::PacketHandler(DeviceState& state, const uint8_t* pins) 
    : deviceState(state), hallSensorPins(pins), fileManager() {
}

void PacketHandler::handlePacket(const Packet& packet) {
    switch(packet.commandId) {
        case CMD_INIT:
            handleInitCommand(packet);
            break;
        case CMD_THEMES:
            handleThemesCommand(packet);
            break;
        case CMD_DATA:
            if (packet.length >= 2) {  // Make sure we have enough data
                uint8_t dataType = packet.payload[0];
                if (dataType == DATA_TRAINING) {
                    uint8_t requestedGear = packet.payload[1];
                    handleTrainingDataRequest(requestedGear);
                }
            }
            break;
        case CMD_CONFIG:
            handleConfigCommand(packet);
            break;
        case CMD_ACK:
            handleAck(packet);
            break;
        case CMD_NACK:
            handleNack(packet);
            break;
        case CMD_STATUS:
            handleStatus(packet);
            break;
        case CMD_NOTIFICATION:
            handleNotification(packet);
            break;
        default:
            Serial.println("Unknown command received");
            break;
    }
}

void PacketHandler::handleInitCommand(const Packet& packet) {
    Serial.println("Handling Init Command");
    deviceState.setInitialized(true);
    // Create and send confirmation packet
    Packet response = {0};
    response.commandId = CMD_INIT;  // Same command ID as received
    response.length = 1;  // 1 byte payload
    response.payload[0] = deviceState.isInitialized() ? 0x01 : 0x00;  // Send initialization status
    
    // TODO: Send response packet back to sender
    Serial.println("Sending init confirmation packet");
    // Calculate checksum for response packet
    uint8_t checksumData[2] = {response.commandId, response.length};
    uint8_t calculatedChecksum = 0;
    for (int i = 0; i < 2; i++) {
        calculatedChecksum ^= checksumData[i];
    }
    for (int i = 0; i < response.length; i++) {
        calculatedChecksum ^= response.payload[i];
    }
    response.checksum = calculatedChecksum;

    // Build complete packet
    uint8_t packetData[256] = {0};
    packetData[0] = response.commandId;
    packetData[1] = response.length;
    memcpy(&packetData[2], response.payload, response.length);
    packetData[2 + response.length] = response.checksum;

    // Send packet over Serial
    Serial.write(packetData, response.length + 3);
    // Send ACK response
    // TODO: Implement response mechanism
}

void PacketHandler::handleThemesCommand(const Packet& packet) {
    Serial.println("Handling Themes Command");
    // Handle theme updates
    
    // payload[0] might contain theme ID
}

void PacketHandler::handleConfigCommand(const Packet& packet) {
    uint8_t numGears = 0;  // Move declaration to top
    uint8_t configType = packet.payload[0];

    switch(configType) {
        case CONFIG_GET:
            // Handle get config
            break;
            
        case CONFIG_SET:
            numGears = packet.payload[1];  // Now we can assign safely
            // Handle set config
            break;
            
        case CONFIG_RESET:
            // Handle reset
            break;
            
        default:
            Serial.println("Unknown config command");
            break;
    }
}

void PacketHandler::handleAck(const Packet& packet) {
    Serial.println("ACK received - packet successfully received by phone");
}

void PacketHandler::handleNack(const Packet& packet) {
    Serial.println("Handling NACK");
    // Handle negative acknowledgment
    // Get error code from payload
    uint8_t errorCode = packet.payload[0];
    Serial.printf("NACK received with error code: 0x%02X\n", errorCode);

    // Log the specific error
    switch(errorCode) {
        case 0x01:
            Serial.println("Error: Invalid command");
            break;
        case 0x02:
            Serial.println("Error: Checksum mismatch");
            break;
        case 0x03:
            Serial.println("Error: Invalid payload length");
            break;
        case 0x04:
            Serial.println("Error: Invalid payload data");
            break;
        default:
            Serial.printf("Error: Unknown error code 0x%02X\n", errorCode);
            break;
    }
}

void PacketHandler::handleStatus(const Packet& packet) {
    Serial.println("Handling Status Request");
    // Send back device status
    // Could include battery, connection strength, etc.
}

void PacketHandler::handleNotification(const Packet& packet) {
    Serial.println("Handling Notification");
    // Display or process notification
    
    // Could be alerts, messages, etc.
}

uint8_t PacketHandler::calculateChecksum(const uint8_t* data, uint16_t length) {
    uint8_t sum = 0;
    for (uint16_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return sum;
}

void PacketHandler::sendPacket(const Packet& packet) {
    // Build complete packet
    uint8_t packetData[256] = {0};
    packetData[0] = packet.commandId;
    packetData[1] = packet.length;
    memcpy(&packetData[2], packet.payload, packet.length);
    packetData[2 + packet.length] = packet.checksum;

    // Send packet
    Serial.write(packetData, packet.length + 3);
}

void PacketHandler::handleData(const Packet& packet) {
    Serial.println("Handling Data Transfer");
    
    // Static variables persist between function calls to maintain transfer state
    static uint8_t* dataBuffer = nullptr;        // Buffer to store complete data
    static uint16_t totalPackets = 0;            // Total number of packets expected
    static uint16_t currentPacket = 0;           // Current packet number being processed
    static uint32_t dataSize = 0;                // Total size of the complete data
    static DataType dataType = DATA_IMAGE;       // Type of data being transferred
    static bool isReceivingData = false;         // Flag to track if we're in a transfer

    // Check if this is a metadata packet (indicated by 0x00 in first byte)
    if (packet.payload[0] == 0x00) {
        // Cast the payload pointer to our metadata structure for easy access
        // This assumes the payload matches our DataMetadata structure layout
        DataMetadata* metadata = (DataMetadata*)packet.payload;
        
        // Extract information from metadata
        dataType = static_cast<DataType>(metadata->dataType);    // Get data type
        totalPackets = metadata->totalPackets;                   // Get total packets
        dataSize = metadata->totalSize;                          // Get total data size

        // Log transfer details
        Serial.printf("Starting new data transfer:\n");
        Serial.printf("Data Type: 0x%02X\n", dataType);
        Serial.printf("Total Packets: %d\n", totalPackets);
        Serial.printf("Data Size: %d bytes\n", dataSize);
        Serial.printf("Flags: 0x%02X\n", metadata->flags);

        // Clean up any existing buffer and allocate new one
        if (dataBuffer != nullptr) {
            delete[] dataBuffer;                 // Free existing buffer if any
        }
        dataBuffer = new uint8_t[dataSize];     // Allocate new buffer
        currentPacket = 0;                       // Reset packet counter
        isReceivingData = true;                  // Set receiving flag

        // Send acknowledgment for metadata
        Packet ack = {0};                       // Create empty packet
        ack.commandId = CMD_ACK;                // Set as ACK command
        ack.length = 2;                         // ACK payload is 2 bytes
        ack.payload[0] = ACK_METADATA;          // Indicate metadata ACK
        ack.payload[1] = metadata->dataType;    // Echo back the data type
        sendPacket(ack);                        // Send the ACK
    }
    // Handle data packets when we're expecting them
    else if (isReceivingData && packet.payload[0] != 0x00) {
        // Extract packet index from first two bytes
        uint16_t packetIndex = (packet.payload[0] << 8) | packet.payload[1];
        
        // Calculate actual data size in this packet (excluding index bytes)
        uint16_t dataChunkSize = packet.length - 2;
        
        // Calculate where in the buffer this data should go
        uint32_t bufferOffset = packetIndex * (MAX_PACKET_SIZE - 2);

        // Verify packet index is valid
        if (packetIndex < totalPackets) {
            // Copy data to the correct position in buffer
            // Skip first 2 bytes (index) and copy remaining data
            memcpy(dataBuffer + bufferOffset, &packet.payload[2], dataChunkSize);
            currentPacket++;                     // Increment packet counter

            // Log progress
            Serial.printf("Received packet %d of %d\n", currentPacket, totalPackets);

            // Send acknowledgment for this data packet
            Packet ack = {0};
            ack.commandId = CMD_ACK;
            ack.length = 3;                      // ACK payload is 3 bytes
            ack.payload[0] = ACK_DATA_CHUNK;     // Indicate data chunk ACK
            ack.payload[1] = packet.payload[0];  // Echo back packet index (high byte)
            ack.payload[2] = packet.payload[1];  // Echo back packet index (low byte)
            sendPacket(ack);

            // Check if this was the last packet
            if (currentPacket == totalPackets) {
                Serial.println("Data transfer complete");
                
                // Process the complete data based on its type
                switch(dataType) {
                    // Image-related data types
                    case DATA_IMAGE:
                        fileManager.saveImageData(dataBuffer, dataSize, FileType::IMAGE);
                        break;
                        
                    case DATA_BACKGROUND:
                        fileManager.saveImageData(dataBuffer, dataSize, FileType::BACKGROUND);
                        break;
                        
                    case DATA_TRANSPARENT:
                        fileManager.saveImageData(dataBuffer, dataSize, FileType::TRANSPARENT);
                        break;
                    
                    // Training/Model data types
                    case DATA_TRAINING:
                    case DATA_MODEL_WEIGHTS:
                        fileManager.saveTrainingData(dataBuffer, dataSize, "model");
                        break;
                        
                    case DATA_MODEL_CONFIG:
                        fileManager.saveConfigData(dataBuffer, dataSize, FileType::MODEL_CONFIG);
                        break;
                    
                    // Theme data types
                    case DATA_THEME_CONFIG:
                        fileManager.saveThemeData(dataBuffer, dataSize, FileType::THEME);
                        break;
                    case DATA_THEME_LAYOUT:
                        fileManager.saveThemeData(dataBuffer, dataSize, FileType::THEME_LAYOUT);
                        break;
                    case DATA_THEME_COLORS:
                        fileManager.saveThemeData(dataBuffer, dataSize, FileType::THEME_COLORS);
                        break;

                    // Device configuration data
                    case DATA_DEVICE_CONFIG:
                        fileManager.saveConfigData(dataBuffer, dataSize, FileType::CONFIG);
                        break;
                    case DATA_USER_PREFS:
                        fileManager.saveConfigData(dataBuffer, dataSize, FileType::USER_PREFS);
                        break;

                    default:
                        Serial.printf("Unknown data type: 0x%02X\n", dataType);
                        Packet nack = {0};
                        nack.commandId = CMD_NACK;
                        nack.length = 2;
                        nack.payload[0] = ERR_INVALID_TYPE;
                        nack.payload[1] = static_cast<uint8_t>(dataType);
                        sendPacket(nack);
                        break;
                }

                // Clean up after transfer
                delete[] dataBuffer;             // Free the buffer
                dataBuffer = nullptr;            // Reset pointer
                isReceivingData = false;         // Reset receiving flag

                // Send completion acknowledgment
                Packet complete = {0};
                complete.commandId = CMD_ACK;
                complete.length = 2;
                complete.payload[0] = ACK_COMPLETE;   // Indicate transfer complete
                complete.payload[1] = static_cast<uint8_t>(dataType);  // Echo data type
                sendPacket(complete);
            }
        }
        else {
            // Handle invalid packet index
            Packet nack = {0};
            nack.commandId = CMD_NACK;
            nack.length = 3;
            nack.payload[0] = ERR_PACKET_INDEX;      // Indicate invalid index error
            nack.payload[1] = packet.payload[0];     // Echo invalid index (high byte)
            nack.payload[2] = packet.payload[1];     // Echo invalid index (low byte)
            sendPacket(nack);
        }
    }
    else {
        // Handle unexpected packet state
        Packet nack = {0};
        nack.commandId = CMD_NACK;
        nack.length = 2;
        nack.payload[0] = ERR_INVALID_STATE;        // Indicate invalid state error
        nack.payload[1] = packet.commandId;         // Echo received command
        sendPacket(nack);
    }
}

void PacketHandler::handleTrainingDataRequest(uint8_t gearPosition) {
    if (!bleManager) return;
    
    const uint8_t NUM_SAMPLES = 10;  // Number of samples to collect per request
    
    // Create a buffer to store all samples
    String dataPacket = "";
    dataPacket.reserve(NUM_SAMPLES * ACTIVE_HALL_SENSORS * 4 + 10);  // Reserve space for data + overhead
    
    // Add header information
    dataPacket += "TD:";  // Training Data marker
    dataPacket += String(gearPosition);
    dataPacket += ":";
    
    // Collect multiple samples
    for (int sample = 0; sample < NUM_SAMPLES; sample++) {
        for (int i = 0; i < ACTIVE_HALL_SENSORS; i++) {
            int rawValue = analogRead(hallSensorPins[i]);
            int mappedValue = map(rawValue, 0, 4095, 0, 100);
            dataPacket += String(mappedValue);
            dataPacket += ",";
        }
        delay(50);  // Small delay between samples
        
        // Send packet after each sample to avoid buffer overflow
        if (dataPacket.length() > 100) {  // Arbitrary threshold
            bleManager->sendData(dataPacket);
            dataPacket = "";  // Clear buffer
            dataPacket.reserve(NUM_SAMPLES * ACTIVE_HALL_SENSORS * 4 + 10);
        }
    }
    
    // Send any remaining data
    if (dataPacket.length() > 0) {
        bleManager->sendData(dataPacket);
    }
}


