#include "Calculate.hpp"
#include <stdexcept>
#include "Function.hpp"
#include "Operator.hpp"

namespace MathEngine {

void Engine::OperatorSecurity(std::vector<std::string> &part_input){
    if (part_input.empty()) return;

    for (int i = 0; i < part_input.size(); i++) {
        bool is_op = OperatorFactory::isOperator(part_input[i]);

        if (i % 2 == 0) {
            if (is_op) {
                throw std::invalid_argument("Syntax error: Expected a number, but found an operator '" + part_input[i] + "' at position " + std::to_string(i));
            }
        } 
        else {
            if (!is_op) {
                throw std::invalid_argument("Structure error: Expected an operator, but found a number '" + part_input[i] + "' at position " + std::to_string(i));
            }
        }
    }

    if (part_input.size() % 2 == 0) {
        throw std::invalid_argument("Structure error: Incomplete expression (ends with an operator or missing a number)!");
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
    if (part_input.empty()){
        return 0;
    } 
    if (part_input.size() == 1) {
        return std::stod(part_input[0]);
    }   
    while (part_input.size() > 1) {
        int operator_index = operatorIndex(part_input);
        if (operator_index <= 0 || operator_index >= part_input.size() - 1) {
            throw std::invalid_argument("Malformed expression around operator position " + std::to_string(operator_index));
        }
        double solution = (OperatorFactory::getOperator(part_input[operator_index]))->calculate(std::stod(part_input[operator_index - 1]), std::stod(part_input[operator_index + 1]));
        int insert_pos = operator_index - 1;
        part_input.erase(part_input.begin() + insert_pos, part_input.begin() + insert_pos + 3);
        part_input.insert(part_input.begin() + insert_pos, std::to_string(solution));
    }
    return std::stod(part_input[0]);
}

double Engine::engine(std::vector<std::string> &part_input) {
    OperatorSecurity(part_input);
    return processLogic(part_input);
}



double Calculate::processLogic(std::vector<std::string> &part_input) {
    if (part_input.empty()){
        return 0;
    } 
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

}
