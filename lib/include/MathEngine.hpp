#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "CalculationResult.hpp"
#include "Operator.hpp"
#include "Function.hpp"
#include "Formatter.hpp"
#include "Tokenizer.hpp"
#include "Algorithm.hpp"

class MathEngine {   
public:
    MathEngine() = delete;
    static double mathengine(std::string input);
    static CalculationResult calculateSafe(std::string input);
};