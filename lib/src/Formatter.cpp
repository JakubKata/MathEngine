#include "Formatter.hpp"

std::string Formater::processLogic(std::string &input) {
    if (input.empty()) {
        input = "0.0";
    }

    int parenthesis_open = 0;
    int parenthesis_close = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            parenthesis_open = parenthesis_open + 1;
        } else if (input[i] == ')') {
            parenthesis_close = parenthesis_close + 1;
        }
    }
    while (parenthesis_close != parenthesis_open) {
        if (parenthesis_open > parenthesis_close) {
            parenthesis_close = parenthesis_close + 1;
            input = input + ')';
        } else if (parenthesis_open < parenthesis_close) {
            parenthesis_open = parenthesis_open + 1;
            input = '(' + input;
        }
    }
    
    std::string input_format;

    for (int i = 0; i < input.size(); ++i) {            
        if (FunctionFactory::formatterFunction(input, input_format, i)) {
            continue;
        } 
        if (i + 1 < input.size() && input[i] == '^' && input[i+1] == '-') {
            input_format = input_format + "^(-";
            i = i + 2;
            while(i < input.size() && (isdigit(input[i]) || input[i] == '.')) {
                input_format = input_format + input[i];
                i = i + 1;
            }
            i = i - 1;
            input_format = input_format + ')';
        } else if (input[i] == '(' || input[i] == ')' || input[i] == '-' || OperatorFactory::isOperator(std::string(1, input[i])) || input[i] == '.' || isdigit(input[i])) {
            input_format = input_format + input[i];
        }
    }

    input = input_format;
    input_format.clear();

    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(' && i > 0 && (input[i-1] == ')' || isdigit(input[i-1]))) {
            input_format = input_format + "*(";
        } else if (input[i] == ')' && i + 1 < input.size() && isdigit(input[i+1])) {
            input_format = input_format + ")*";
        } else {
            input_format = input_format + input[i];
        }    
    }
            
    input = input_format;
    input_format.clear();

    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '-' && i > 0 && (input[i-1] == ')' || isdigit(input[i-1]))) {
            input_format = input_format + "+(-1)*";
        } else if (input[i] == '-') {
            input_format = input_format + "(-1)*";
        } else{
            input_format = input_format + input[i];
        }
    }

    return '(' + input_format + ')';
}    

void Formater::formater(std::string &input) {
    input = processLogic(input);
}
