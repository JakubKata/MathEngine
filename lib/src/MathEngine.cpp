#include "MathEngine.hpp" 

double MathEngine::mathengine(std::string input) {
    std::vector<std::string> pressed_input;
    double solution;
    Formater::formater(input);
    Tokenizer::tokenizer(input, pressed_input);
    Algorithm::algorithm(pressed_input, solution);
    return solution;
}
