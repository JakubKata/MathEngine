#include "Formatter.hpp"
#include <cctype>
#include <stdexcept>
#include "Function.hpp"
#include "Operator.hpp"

namespace MathEngine {

std::string Formater::processLogic(std::string &input) {
    if (input.empty()) {
        throw std::invalid_argument("Input cannot be empty");
    }

    bool one_index_is_digit = false;
    for (int i = 0; i < input.size(); ++i) {
        if (isdigit(input[i])) {
            one_index_is_digit = true;
        }
    }
    if (!one_index_is_digit){
        throw std::invalid_argument("Input must contain at least one digit");
    }

    int parenthesis_balance = 0;
    int needed_open_parenthesis = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            parenthesis_balance = parenthesis_balance + 1;
        } else if (input[i] == ')') {
            if (parenthesis_balance == 0) {
                needed_open_parenthesis = needed_open_parenthesis + 1;
            } else {
                parenthesis_balance = parenthesis_balance - 1;
            }
        }
    }

    if (needed_open_parenthesis > 0) {
        input = std::string(needed_open_parenthesis, '(') + input;
    }
    if (parenthesis_balance > 0) {
        input = input + std::string(parenthesis_balance, ')');
    }

    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            int next_non_space = i + 1;
            while (next_non_space < input.size() && isspace(input[next_non_space])) {
                next_non_space = next_non_space + 1;
            }
            if (next_non_space < input.size() && input[next_non_space] == ')') {
                throw std::invalid_argument("Syntax error: empty parenthesis group");
            }
        }
    }
    
    std::string input_format;

for (int i = 0; i < input.size(); ++i) {            
        if (i + 1 < input.size() && input[i] == '^' && input[i+1] == '-') {
            int start_index = i + 2;
            int current_index = start_index;
            bool continuing_exponent = true;
            while (continuing_exponent) {
                if (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
                    while(current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
                        current_index = current_index + 1;
                    }
                } else if (current_index < input.size() && isalpha(input[current_index])) {
                    while(current_index < input.size() && isalpha(input[current_index])) {
                        current_index = current_index + 1;
                    }
                    if (current_index < input.size() && input[current_index] == '(') {
                        int open_parentheses = 0;
                        do {
                            if (input[current_index] == '(') {
                                open_parentheses = open_parentheses + 1;
                            }
                            if (input[current_index] == ')') {
                                open_parentheses = open_parentheses - 1 ;
                            }
                            current_index = current_index + 1;
                        } while (current_index < input.size() && open_parentheses > 0);
                    }
                } else if (current_index < input.size() && input[current_index] == '(') {
                    int open_parentheses = 0;
                    do {
                        if (input[current_index] == '(') {
                            open_parentheses = open_parentheses + 1;
                        }
                        if (input[current_index] == ')') {
                            open_parentheses = open_parentheses - 1;
                        }
                        current_index = current_index + 1;
                    } while (current_index < input.size() && open_parentheses > 0);
                }
                if (current_index < input.size() && input[current_index] == '^') {
                    current_index = current_index + 1;
                    if (current_index < input.size() && input[current_index] == '-') {
                        current_index = current_index + 1; 
                    }
                } else {
                    continuing_exponent = false;
                }
            }
            std::string exponent_substring;
            for (int j = start_index; j < current_index; ++j) {
                exponent_substring = exponent_substring + input[j];
            }
            
            if (!exponent_substring.empty()) {
                std::string formatted_exponent = processLogic(exponent_substring);
                input_format = input_format + "^(-" + formatted_exponent + ")";
                i = current_index - 1;
            } else {
                input_format = input_format + "^-";
                i = i + 1;
            }
        } 
        else {
            input_format = input_format + input[i];
        }
    }

    input = input_format;
    input_format.clear();

    for (int i = 0; i < input.size(); ++i) {            
        if (FunctionFactory::formatterFunction(input, input_format, i)) {
            continue;
        } else {
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
        } 
        else if (input[i] == '-') {         
            if ((i + 1 < input.size() && (isdigit(input[i+1]) || input[i+1] == '.'))) {
                int j = i + 1;
                while (j < input.size() && (isdigit(input[j]) || input[j] == '.')) {
                    j = j + 1;
                }
                if (j < input.size() && input[j] == '^') {
                    input_format = input_format + "(-1)*";
                } else {           
                input_format = input_format + '-';
                }
            } 
            else if (i + 1 < input.size() && input[i+1] == '(') {
                int start_index = i + 1;
                int current_index = start_index;
                int open_parenthesis = 0;
                do {  
                    if (input[current_index] == '(') {
                        open_parenthesis = open_parenthesis + 1;
                    }    
                    if (input[current_index] == ')') {
                         open_parenthesis = open_parenthesis - 1;
                    }
                    current_index = current_index + 1;
                } while (current_index < input.size() && open_parenthesis > 0);                
                
                std::string inner_content;
                for (int k = start_index; k < current_index; ++k) {
                    inner_content = inner_content + input[k];
                }
                
                std::string processed_inner = processLogic(inner_content);

                if (current_index < input.size() && input[current_index] == '^') {
                    input_format = input_format + "(-1)*" + processed_inner;
                } else {
                    input_format = input_format + "((-1)*" + processed_inner + ")";
                }
                i = current_index - 1; 
            }
            else {
                input_format = input_format + "(-1)*";
            }
        } 
        else {
            input_format = input_format + input[i];
        }
    }
        input = input_format;

    for (int i = 0; i < input.size(); ++i) {
        if (OperatorFactory::isOperator(std::string(1, input[i]))) {
            if (OperatorFactory::getOperator(std::string(1, input[i]))->getPriority() == 2) {
                
                int current_index = i + 1;

                while (current_index + 5 <= input.size() && 
                       input[current_index] == '(' &&
                       input[current_index + 1] == '-' &&
                       input[current_index + 2] == '1' &&
                       input[current_index + 3] == ')' &&
                       input[current_index + 4] == '*') {                    
                    current_index = current_index + 5;
                }

                if (current_index < input.size()) {
                    if (input[current_index] == '(') {
                        int open_parenthesis = 0;
                        do {
                            if (input[current_index] == '(') {
                                open_parenthesis = open_parenthesis + 1;
                            }
                            if (input[current_index] == ')') {
                                open_parenthesis = open_parenthesis - 1;
                            }
                            current_index = current_index + 1;
                        } while (current_index < input.size() && open_parenthesis > 0);
                    } else {
                        if (current_index < input.size() && input[current_index] == '-') {
                            current_index = current_index + 1;
                        }
                        while (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.' || isalpha(input[current_index]))) {
                            current_index = current_index + 1;
                        }
                        if (current_index < input.size() && input[current_index] == '(') {
                             int open_parenthesis = 0;
                             do {
                                if (input[current_index] == '(') {
                                    open_parenthesis = open_parenthesis + 1;
                                }
                                if (input[current_index] == ')') {
                                    open_parenthesis = open_parenthesis - 1;
                                }
                                current_index = current_index + 1;
                             } while (current_index < input.size() && open_parenthesis > 0);
                        }
                    }
                }

                while (current_index < input.size() && input[current_index] == '^') {
                    current_index = current_index + 1;
                    if (current_index < input.size() && input[current_index] == '(') {
                         int open_parenthesis = 0;
                         do {
                            if (input[current_index] == '(') {
                                open_parenthesis = open_parenthesis + 1;
                            }
                            if (input[current_index] == ')') {
                                open_parenthesis = open_parenthesis - 1;
                            }
                            current_index = current_index + 1;
                         } while (current_index < input.size() && open_parenthesis > 0);
                    } else {
                         if (current_index < input.size() && input[current_index] == '-') {
                            current_index = current_index + 1;
                         }
                         while (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
                            current_index = current_index + 1;
                         }
                    }
                }
                input.insert(current_index, ")");
                input.insert(i + 1, "(");
            }
        }
    }
    return '(' + input + ')'; 
}    

void Formater::formater(std::string &input) {
    input = processLogic(input);
}

}
