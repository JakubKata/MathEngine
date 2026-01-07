#include "Tokenizer.hpp"

std::vector<std::string> Tokenizer::processLogic(const std::string &input) {
    std::vector<std::string> tokenized_input;
    std::string digit;
    bool point_in_digit = false;

    for (int i = 0; i < input.size(); ++i) {
        if (OperatorFactory::isOperator(std::string(1, input[i])) || FunctionFactory::isFunction(std::string(1, input[i])) || input[i] == '(' || input[i] == ')') {
            if (!digit.empty()) {
                tokenized_input.push_back(digit);
                digit.clear();
                point_in_digit = false;
            }                
            tokenized_input.push_back(std::string(1, input[i]));
            
        } else if (isdigit(input[i]) || input[i] == '-') {
            digit = digit + input[i];
        } else if (input[i] == '.' && !point_in_digit) {
            digit = digit + input[i];
            point_in_digit = true;
        }
    }

    return tokenized_input;
}    

void Tokenizer::tokenizer(std::string &input, std::vector<std::string> &tokenized_input) {
    tokenized_input = processLogic(input);
}

