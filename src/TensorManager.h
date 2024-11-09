#ifndef TENSOR_MANAGER_H
#define TENSOR_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <TensorFlowLite_ESP32.h>

// TensorFlow Lite ESP32 headers
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include <vector>

class TensorManager {
public:
    TensorManager();
    
    // Model management
    bool loadModel(const char* modelName);
    
    // Training data collection
    void observeTrainingData(float* data, size_t dataSize, uint8_t gearPosition);
    bool saveTrainingData();
    bool getTrainingData(uint8_t* buffer, size_t* size);
    void clearTrainingData();

    // Inference
    uint8_t predictGear(float* sensorData, size_t dataSize);

private:
    static const int kTensorArenaSize = 32 * 1024;
    static const size_t MAX_SAMPLES = 1000;  // Adjust based on your needs
    
    struct TrainingSample {
        std::vector<float> data;
        uint8_t label;
    };
    
    std::vector<TrainingSample> trainingData;
    uint8_t tensor_arena[kTensorArenaSize];
    tflite::ErrorReporter* error_reporter;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    tflite::MicroMutableOpResolver<1> resolver;
};

#endif // TENSOR_MANAGER_H 