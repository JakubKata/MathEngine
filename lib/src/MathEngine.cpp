#include "MathEngine.hpp" 

double MathEngine::mathengine(std::string input) {
    std::vector<std::string> pressed_input;
    double solution;
    std::cout << "[DBG] input=" << input << std::endl;
    Formater::formater(input);
    std::cout << "[DBG] formatted=" << input << std::endl;
    Tokenizer::tokenizer(input, pressed_input);
    std::cout << "[DBG] tokenized size=" << pressed_input.size() << std::endl;
    Algorithm::algorithm(pressed_input, solution);
    std::cout << "[DBG] solved=" << solution << std::endl;
    return solution;
}

CalculationResult MathEngine::calculateSafe(std::string input) {
    CalculationResult result;
    
    try {
        result.value = mathengine(input);
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