#ifndef TENSORFLOW_LITE_MANAGER_H
#define TENSORFLOW_LITE_MANAGER_H

class TensorFlowLiteManager {
public:
    TensorFlowLiteManager();
    void initialize(const char* model_path);
};

#endif // TENSORFLOW_LITE_MANAGER_H 