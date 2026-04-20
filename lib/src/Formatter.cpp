#include "Formatter.hpp"
#include <cctype>
#include <stdexcept>
#include "Function.hpp"
#include "Operator.hpp"

namespace MathEngine {

namespace {

static std::string processLogicImpl(std::string input);

static bool containsDigit(const std::string &input) {
    for (int i = 0; i < input.size(); ++i) {
        if (isdigit(input[i])) {
            return true;
        }
    }
    return false;
}

static void normalizeParentheses(std::string &input) {
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
}

static void validateEmptyParenthesisGroups(const std::string &input) {
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
}

static int consumeBalancedParentheses(const std::string &input, int start_index) {
    int open_parentheses = 0;

    do {
        if (input[start_index] == '(') {
            open_parentheses = open_parentheses + 1;
        }
        if (input[start_index] == ')') {
            open_parentheses = open_parentheses - 1;
        }
        start_index = start_index + 1;
    } while (start_index < input.size() && open_parentheses > 0);

    return start_index;
}

static int consumeExponentOperand(const std::string &input, int start_index) {
    int current_index = start_index;

    if (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
        while (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
            current_index = current_index + 1;
        }
    } else if (current_index < input.size() && isalpha(input[current_index])) {
        while (current_index < input.size() && isalpha(input[current_index])) {
            current_index = current_index + 1;
        }
        if (current_index < input.size() && input[current_index] == '(') {
            current_index = consumeBalancedParentheses(input, current_index);
        }
    } else if (current_index < input.size() && input[current_index] == '(') {
        current_index = consumeBalancedParentheses(input, current_index);
    }

    return current_index;
}

static std::string extractNegativeExponentSubstring(const std::string &input, int start_index, int &current_index) {
    current_index = start_index;
    bool continuing_exponent = true;

    while (continuing_exponent) {
        current_index = consumeExponentOperand(input, current_index);

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
    for (int i = start_index; i < current_index; ++i) {
        exponent_substring = exponent_substring + input[i];
    }

    return exponent_substring;
}

static std::string formatNegativeExponents(const std::string &input) {
    std::string input_format;

    for (int i = 0; i < input.size(); ++i) {
        if (i + 1 < input.size() && input[i] == '^' && input[i + 1] == '-') {
            int current_index = 0;
            std::string exponent_substring = extractNegativeExponentSubstring(input, i + 2, current_index);

            if (!exponent_substring.empty()) {
                std::string formatted_exponent = processLogicImpl(exponent_substring);
                input_format = input_format + "^(-" + formatted_exponent + ")";
                i = current_index - 1;
            } else {
                input_format = input_format + "^-";
                i = i + 1;
            }
        } else {
            input_format = input_format + input[i];
        }
    }

    return input_format;
}

static std::string expandFunctions(std::string input) {
    std::string input_format;

    for (int i = 0; i < input.size(); ++i) {
        if (FunctionFactory::formatterFunction(input, input_format, i)) {
            continue;
        }
        input_format = input_format + input[i];
    }

    return input_format;
}

static std::string insertExplicitMultiplication(const std::string &input) {
    std::string input_format;

    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(' && i > 0 && (input[i - 1] == ')' || isdigit(input[i - 1]))) {
            input_format = input_format + "*(";
        } else if (input[i] == ')' && i + 1 < input.size() && isdigit(input[i + 1])) {
            input_format = input_format + ")*";
        } else {
            input_format = input_format + input[i];
        }
    }

    return input_format;
}

static std::string rewriteUnaryMinus(const std::string &input) {
    std::string input_format;

    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '-' && i > 0 && (input[i - 1] == ')' || isdigit(input[i - 1]))) {
            input_format = input_format + "+(-1)*";
        } else if (input[i] == '-') {
            if (i + 1 < input.size() && (isdigit(input[i + 1]) || input[i + 1] == '.')) {
                int j = i + 1;
                while (j < input.size() && (isdigit(input[j]) || input[j] == '.')) {
                    j = j + 1;
                }
                if (j < input.size() && input[j] == '^') {
                    input_format = input_format + "(-1)*";
                } else {
                    input_format = input_format + '-';
                }
            } else if (i + 1 < input.size() && input[i + 1] == '(') {
                int current_index = consumeBalancedParentheses(input, i + 1);

                std::string inner_content;
                for (int k = i + 1; k < current_index; ++k) {
                    inner_content = inner_content + input[k];
                }

                std::string processed_inner = processLogicImpl(inner_content);

                if (current_index < input.size() && input[current_index] == '^') {
                    input_format = input_format + "(-1)*" + processed_inner;
                } else {
                    input_format = input_format + "((-1)*" + processed_inner + ")";
                }
                i = current_index - 1;
            } else {
                input_format = input_format + "(-1)*";
            }
        } else {
            input_format = input_format + input[i];
        }
    }

    return input_format;
}

static int consumePowerTail(const std::string &input, int current_index) {
    while (current_index < input.size() && input[current_index] == '^') {
        current_index = current_index + 1;
        if (current_index < input.size() && input[current_index] == '(') {
            current_index = consumeBalancedParentheses(input, current_index);
        } else {
            if (current_index < input.size() && input[current_index] == '-') {
                current_index = current_index + 1;
            }
            while (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.')) {
                current_index = current_index + 1;
            }
        }
    }

    return current_index;
}

static int consumeOperandAfterOperator(const std::string &input, int current_index) {
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
            current_index = consumeBalancedParentheses(input, current_index);
        } else {
            if (current_index < input.size() && input[current_index] == '-') {
                current_index = current_index + 1;
            }
            while (current_index < input.size() && (isdigit(input[current_index]) || input[current_index] == '.' || isalpha(input[current_index]))) {
                current_index = current_index + 1;
            }
            if (current_index < input.size() && input[current_index] == '(') {
                current_index = consumeBalancedParentheses(input, current_index);
            }
        }
    }

    return consumePowerTail(input, current_index);
}

static std::string wrapOperatorOperands(const std::string &input) {
    std::string output = input;

    for (int i = 0; i < output.size(); ++i) {
        if (OperatorFactory::isOperator(std::string(1, output[i]))) {
            if (OperatorFactory::getOperator(std::string(1, output[i]))->getPriority() == 2) {
                int current_index = consumeOperandAfterOperator(output, i + 1);
                output.insert(current_index, ")");
                output.insert(i + 1, "(");
            }
        }
    }

    return output;
}

static std::string processLogicImpl(std::string input) {
    if (input.empty()) {
        throw std::invalid_argument("Input cannot be empty");
    }

    if (!containsDigit(input)) {
        throw std::invalid_argument("Input must contain at least one digit");
    }

    normalizeParentheses(input);
    validateEmptyParenthesisGroups(input);
    input = formatNegativeExponents(input);
    input = expandFunctions(input);
    input = insertExplicitMultiplication(input);
    input = rewriteUnaryMinus(input);
    input = wrapOperatorOperands(input);

    return '(' + input + ')';
}

} 

std::string Formatter::processLogic(std::string &input) {
    return processLogicImpl(input);
}

void Formatter::formatter(std::string &input) {
    input = processLogic(input);
}

}
