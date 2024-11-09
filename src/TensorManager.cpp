#include "TensorManager.h"
#include "FileManager.h"

TensorManager::TensorManager() {
    // Initialize any necessary components here
}

bool TensorManager::loadModel(const char* modelName) {
    // Load the TensorFlow Lite model from SPIFFS
    char filename[32];
    snprintf(filename, sizeof(filename), "/spiffs/%s.tflite", modelName);
    
    if (!SPIFFS.exists(filename)) {
        Serial.printf("Model file not found: %s\n", filename);
        return false;
    }

    File modelFile = SPIFFS.open(filename, "r");
    if (!modelFile) {
        Serial.println("Failed to open model file");
        return false;
    }

    size_t modelSize = modelFile.size();
    uint8_t* modelBuffer = new uint8_t[modelSize];
    if (!modelBuffer) {
        Serial.println("Failed to allocate memory for model");
        modelFile.close();
        return false;
    }

    if (modelFile.read(modelBuffer, modelSize) != modelSize) {
        Serial.println("Failed to read model file");
        delete[] modelBuffer;
        modelFile.close();
        return false;
    }

    model = tflite::GetModel(modelBuffer);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema version is not supported");
        delete[] modelBuffer;
        modelFile.close();
        return false;
    }

    // Create an error reporter
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    // Create interpreter
    static tflite::MicroInterpreter static_interpreter(
        model, 
        resolver,
        tensor_arena,
        kTensorArenaSize,
        error_reporter
    );
    interpreter = &static_interpreter;

    // Allocate tensors
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        return false;
    }

    modelFile.close();
    Serial.printf("Successfully loaded model: %s\n", modelName);
    return true;
}

void TensorManager::observeTrainingData(float* data, size_t dataSize, uint8_t gearPosition) {
    if (trainingData.size() >= MAX_SAMPLES) {
        Serial.println("Training buffer full, consider saving");
        return;
    }

    TrainingSample sample;
    sample.data.assign(data, data + dataSize);
    sample.label = gearPosition;
    trainingData.push_back(sample);
}

bool TensorManager::saveTrainingData() {
    if (trainingData.empty()) {
        return false;
    }

    // Calculate buffer size needed
    size_t totalSize = sizeof(size_t) + sizeof(size_t);  // num_samples + data_size
    for (const auto& sample : trainingData) {
        totalSize += sizeof(float) * sample.data.size() + sizeof(uint8_t);
    }

    // Create buffer
    uint8_t* buffer = new uint8_t[totalSize];
    size_t pos = 0;

    // Write number of samples
    size_t numSamples = trainingData.size();
    memcpy(buffer + pos, &numSamples, sizeof(size_t));
    pos += sizeof(size_t);

    // Write data size per sample
    size_t dataSize = trainingData[0].data.size();
    memcpy(buffer + pos, &dataSize, sizeof(size_t));
    pos += sizeof(size_t);

    // Write samples
    for (const auto& sample : trainingData) {
        memcpy(buffer + pos, sample.data.data(), sizeof(float) * dataSize);
        pos += sizeof(float) * dataSize;
        memcpy(buffer + pos, &sample.label, sizeof(uint8_t));
        pos += sizeof(uint8_t);
    }

    // Save to file using the correct FileManager method
    File file = SPIFFS.open("/training_data.bin", "w");
    if (!file) {
        delete[] buffer;
        return false;
    }
    
    size_t written = file.write(buffer, totalSize);
    file.close();
    delete[] buffer;
    
    return written == totalSize;
}

bool TensorManager::getTrainingData(uint8_t* buffer, size_t* size) {
    File file = SPIFFS.open("/training_data.bin", "r");
    if (!file) {
        return false;
    }

    *size = file.size();
    if (buffer != nullptr) {
        file.read(buffer, *size);
    }
    file.close();
    return true;
}

void TensorManager::clearTrainingData() {
    trainingData.clear();
    SPIFFS.remove("/training_data.bin");
}

uint8_t TensorManager::predictGear(float* sensorData, size_t dataSize) {
    if (!interpreter || !model) {
        return 0;
    }

    // Get input tensor
    TfLiteTensor* input = interpreter->input(0);
    
    // Copy data to input tensor
    for (size_t i = 0; i < dataSize && i < input->bytes / sizeof(float); i++) {
        input->data.f[i] = sensorData[i];
    }

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
        return 0;
    }

    // Get output
    TfLiteTensor* output = interpreter->output(0);
    return static_cast<uint8_t>(output->data.f[0] + 0.5f);  // Round to nearest integer
} 