#include"Algorithm.hpp"

void Algorithm::parenthesis_index(const std::vector<std::string> &tokenized_input, int &parenthesis_open_index, int &parenthesis_close_index) {
    int parenthesis_current_open_index;
    for (int i = 0; i < tokenized_input.size(); ++i) {
        if (tokenized_input[i] == "(") {
            parenthesis_current_open_index = i;
        } else if (tokenized_input[i] == ")") {
            parenthesis_open_index = parenthesis_current_open_index;
            parenthesis_close_index = i;
            break;
        }
    }
}

double Algorithm::processLogic(std::vector<std::string> &tokenized_input) {
    int parenthesis_open_index;
    int parenthesis_close_index;
    while(tokenized_input.size() > 1) {
        parenthesis_index(tokenized_input, parenthesis_open_index, parenthesis_close_index);
        std::vector<std::string> part_input;
        for (int i = parenthesis_open_index + 1; i < parenthesis_close_index; ++i) {
            part_input.push_back(tokenized_input[i]);
        }
        double solution = Calculate::calculate(part_input);
        tokenized_input.erase(tokenized_input.begin() + parenthesis_open_index, tokenized_input.begin() + parenthesis_close_index + 1);
        tokenized_input.insert(tokenized_input.begin() + parenthesis_open_index, std::to_string(solution));
    }
    return std::stod(tokenized_input[0]);       
}    

void Algorithm::algorithm(std::vector<std::string> &tokenized_input, double &solution) {
    solution = processLogic(tokenized_input);
}
