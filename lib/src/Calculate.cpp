#include "Calculate.hpp"
#include <stdexcept>
#include <vector>
#include "Function.hpp"
#include "Operator.hpp"

namespace MathEngine {

namespace {

static bool isValueToken(const std::string &token) {
    return !OperatorFactory::isOperator(token);
}

static void validateInfixStructure(const std::vector<std::string> &part_input) {
    if (part_input.empty()) {
        return;
    }

    bool expect_value = true;

    for (int i = 0; i < part_input.size(); ++i) {
        bool token_is_operator = OperatorFactory::isOperator(part_input[i]);

        if (expect_value) {
            if (token_is_operator) {
                Operator *op = OperatorFactory::getOperator(part_input[i]);
                if (op->isUnary()) {
                    continue;
                }
                throw std::invalid_argument("Syntax error: Expected a number, but found an operator '" + part_input[i] + "' at position " + std::to_string(i));
            }
            expect_value = false;
        } else {
            if (!token_is_operator) {
                throw std::invalid_argument("Structure error: Expected an operator, but found a number '" + part_input[i] + "' at position " + std::to_string(i));
            }
            Operator *op = OperatorFactory::getOperator(part_input[i]);
            if (op->isUnary()) {
                throw std::invalid_argument("Syntax error: Unary operator '" + part_input[i] + "' in binary position at " + std::to_string(i));
            }
            expect_value = true;
        }
    }

    if (expect_value) {
        throw std::invalid_argument("Structure error: Incomplete expression (ends with an operator or missing a number)!");
    }
}

static std::vector<std::string> convertToRpn(const std::vector<std::string> &part_input) {
    std::vector<std::string> output;
    std::vector<std::string> operator_stack;

    for (const std::string &token : part_input) {
        if (token == "(") {
            operator_stack.push_back(token);
            continue;
        }

        if (token == ")") {
            while (!operator_stack.empty() && operator_stack.back() != "(") {
                output.push_back(operator_stack.back());
                operator_stack.pop_back();
            }

            if (operator_stack.empty()) {
                throw std::invalid_argument("Syntax error: unmatched closing parenthesis");
            }

            operator_stack.pop_back();
            continue;
        }

        if (!OperatorFactory::isOperator(token)) {
            output.push_back(token);
            continue;
        }

        Operator *current_operator = OperatorFactory::getOperator(token);

        if (!current_operator->isUnary()) {
            while (!operator_stack.empty() && OperatorFactory::isOperator(operator_stack.back())) {
                Operator *top_operator = OperatorFactory::getOperator(operator_stack.back());
                bool should_pop = top_operator->getPriority() > current_operator->getPriority()
                    || (top_operator->getPriority() == current_operator->getPriority() && current_operator->isLeftToRight());

                if (!should_pop) {
                    break;
                }

                output.push_back(operator_stack.back());
                operator_stack.pop_back();
            }
        }

        operator_stack.push_back(token);
    }

    while (!operator_stack.empty()) {
        if (operator_stack.back() == "(") {
            throw std::invalid_argument("Syntax error: missing parenthesis pair");
        }

        output.push_back(operator_stack.back());
        operator_stack.pop_back();
    }

    return output;
}

static double evaluateRpn(const std::vector<std::string> &rpn_input) {
    std::vector<double> value_stack;

    for (const std::string &token : rpn_input) {
        if (OperatorFactory::isOperator(token)) {
            Operator *op = OperatorFactory::getOperator(token);

            if (op->isUnary()) {
                if (value_stack.size() < 1) {
                    throw std::invalid_argument("Malformed RPN expression around unary operator '" + token + "'");
                }

                double operand = value_stack.back();
                value_stack.pop_back();

                double solution = op->calculate(0, operand);
                value_stack.push_back(solution);
            } else {
                if (value_stack.size() < 2) {
                    throw std::invalid_argument("Malformed RPN expression around operator '" + token + "'");
                }

                double right = value_stack.back();
                value_stack.pop_back();
                double left = value_stack.back();
                value_stack.pop_back();

                double solution = op->calculate(left, right);
                value_stack.push_back(solution);
            }
            continue;
        }

        if (!isValueToken(token)) {
            throw std::invalid_argument("Invalid token in RPN expression: " + token);
        }

        value_stack.push_back(std::stod(token));
    }

    if (value_stack.size() != 1) {
        throw std::invalid_argument("Malformed RPN expression: too many operands");
    }

    return value_stack.back();
}

} 

double Engine::processLogic(std::vector<std::string> &part_input) {
    if (part_input.empty()) {
        return 0;
    }

    validateInfixStructure(part_input);
    std::vector<std::string> rpn_input = convertToRpn(part_input);
    return evaluateRpn(rpn_input);
}

double Engine::engine(std::vector<std::string> &part_input) {
    return processLogic(part_input);
}

double Calculate::processLogic(std::vector<std::string> &part_input) {
    if (part_input.empty()) {
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
