#pragma once
#include <string>

namespace MathEngine {

struct CalculationResult {
    double value;
    bool isSuccess;
    std::string errorMessage;
};

}