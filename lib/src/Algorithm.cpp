#include"Algorithm.hpp"

void Algorithm::parenthesis_index(const std::vector<std::string> &tokenized_input, int &parenthesis_open_index, int &parenthesis_close_index) {
    int parenthesis_current_open_index = -1;
    parenthesis_open_index = -1;
    parenthesis_close_index = -1;

    for (int i = 0; i < tokenized_input.size(); ++i) {
        if (tokenized_input[i] == "(") {
            parenthesis_current_open_index = i;
        } else if (tokenized_input[i] == ")") {
            if (parenthesis_current_open_index == -1) {
                throw std::invalid_argument("Syntax error: unmatched closing parenthesis");
            }
            parenthesis_open_index = parenthesis_current_open_index;
            parenthesis_close_index = i;
            break;
        }
    }

    if (parenthesis_open_index == -1 || parenthesis_close_index == -1) {
        throw std::invalid_argument("Syntax error: missing parenthesis pair");
    }
}

double Algorithm::processLogic(std::vector<std::string> &tokenized_input) {
    int parenthesis_open_index;
    int parenthesis_close_index;
    while(tokenized_input.size() > 1) {
        parenthesis_index(tokenized_input, parenthesis_open_index, parenthesis_close_index);
        std::cout << "[DBG] alg size=" << tokenized_input.size() << " open=" << parenthesis_open_index << " close=" << parenthesis_close_index << std::endl;
        std::vector<std::string> part_input;
        for (int i = parenthesis_open_index + 1; i < parenthesis_close_index; ++i) {
            part_input.push_back(tokenized_input[i]);
        }
        std::cout << "[DBG] part size=" << part_input.size() << std::endl;
        if (part_input.empty()) {
            throw std::invalid_argument("Syntax error: empty parenthesis group");
        }
        double solution = Calculate::calculate(part_input);
        std::cout << "[DBG] erase range [" << parenthesis_open_index << ", " << (parenthesis_close_index + 1) << ")" << std::endl;
        tokenized_input.erase(tokenized_input.begin() + parenthesis_open_index, tokenized_input.begin() + parenthesis_close_index + 1);
        tokenized_input.insert(tokenized_input.begin() + parenthesis_open_index, std::to_string(solution));
    }
    return std::stod(tokenized_input[0]);       
}    

void Algorithm::algorithm(std::vector<std::string> &tokenized_input, double &solution) {
    solution = processLogic(tokenized_input);
}
