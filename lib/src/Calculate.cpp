#include "Calculate.hpp"

void Engine::dualOperatorSecurity(std::vector<std::string> &part_input){
    while (!(part_input.empty() || OperatorFactory::isNumber(part_input[0]))) {
        part_input.erase(part_input.begin(), part_input.begin() + 1);
    }

    for (int i = 1; i < part_input.size(); i++) {
        if(OperatorFactory::isOperator(part_input[i]) && OperatorFactory::isOperator(part_input[i-1])){
            part_input.erase(part_input.begin() + i, part_input.begin() + i + 1);
            i = i - 1;
        }
    }    
}

int Engine::operatorIndex(std::vector<std::string> &part_input) {
    int operator_index = -1;
    int highest_priority = -1;
    int current_priority = -1;
    for (int i = 0; i < part_input.size(); i++) {
        if (!OperatorFactory::isOperator(part_input[i])) {
            continue;
        }
        current_priority = OperatorFactory::getOperator(part_input[i])->getPriority();
        if (current_priority > highest_priority || (current_priority == highest_priority && !OperatorFactory::getOperator(part_input[i])->isLeftToRight())) {
            operator_index = i;
            highest_priority = current_priority;
        }
    }
    return operator_index;
}  

double Engine::processLogic(std::vector<std::string> &part_input) {
    if (part_input.size() == 1) {
        return std::stod(part_input[0]);
    if (part_input.empty()){
        return 0;
    }    
    }
    while (part_input.size() > 1) {
        int operator_index = operatorIndex(part_input);
        double solution = (OperatorFactory::getOperator(part_input[operator_index]))->calculate(std::stod(part_input[operator_index - 1]), std::stod(part_input[operator_index + 1]));
        part_input.erase(part_input.begin() + operator_index - 1, part_input.begin() + operator_index + 2);
        part_input.insert(part_input.begin() + operator_index - 1, std::to_string(solution));
    }
    return std::stod(part_input[0]);
}

double Engine::engine(std::vector<std::string> &part_input) {
    dualOperatorSecurity(part_input);
    return processLogic(part_input);
}



double Calculate::processLogic(std::vector<std::string> &part_input) {
    std::string function;
    if (FunctionFactory::isFunction(part_input[0])) {
        function = part_input[0];
        part_input.erase(part_input.begin(), part_input.begin() + 1);
    } 

    double solution = Engine::engine(part_input);

    if (function.empty()) {
        return solution;
    } 
    return (FunctionFactory::getFunction(function))->calculate(solution);        
}    



double Calculate::calculate(std::vector<std::string> &part_input) {
    return processLogic(part_input);
}
