#pragma once
#include <string>
#include "CalculationResult.hpp"

namespace MathEngine {

class Math {   
public:
    Math() = delete;
    static double calculate(std::string input);
    static CalculationResult calculateSafe(std::string input);
};

}