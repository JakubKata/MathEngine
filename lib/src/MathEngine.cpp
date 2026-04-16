#include "MathEngine.hpp"
#include <exception>
#include <vector>
#include "Algorithm.hpp"
#include "Formatter.hpp"
#include "Tokenizer.hpp"

namespace MathEngine {

double Math::calculate(std::string input) {
    std::vector<std::string> pressed_input;
    double solution;
    Formatter::formatter(input);
    Tokenizer::tokenizer(input, pressed_input);
    Algorithm::algorithm(pressed_input, solution);
    return solution;
}

CalculationResult Math::calculateSafe(std::string input) {
    CalculationResult result;
    
    try {
        result.value = calculate(input);
        result.isSuccess = true;
        result.errorMessage = ""; 
    } 
    catch (const std::exception& e) {
        result.value = 0.0;
        result.isSuccess = false;
        result.errorMessage = e.what();
    } 
    catch (...) {
        result.value = 0.0;
        result.isSuccess = false;
        result.errorMessage = "Unknown critical engine error";
    }

    return result;
}

}