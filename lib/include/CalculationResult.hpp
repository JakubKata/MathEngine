#pragma once
#include <string>

struct CalculationResult {
    double value;
    bool isSuccess;
    std::string errorMessage;
};