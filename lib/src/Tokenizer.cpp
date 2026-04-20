#include "Tokenizer.hpp"
#include <cctype>
#include <stdexcept>
#include "Function.hpp"
#include "Operator.hpp"

namespace MathEngine {

namespace {

static bool isTokenBoundary(const std::string &token) {
    return OperatorFactory::isOperator(token) || FunctionFactory::isFunction(token) || token == "(" || token == ")";
}

static void flushDigitToken(std::vector<std::string> &tokenized_input, std::string &digit) {
    if (!digit.empty()) {
        tokenized_input.push_back(digit);
        digit.clear();
    }
}

static void validateToken(const std::string &token) {
    if (isTokenBoundary(token)) {
        return;
    }

    bool token_is_digit = false;
    for (int i = 0; i < token.size(); ++i) {
        if (isdigit(token[i])) {
            token_is_digit = true;
        }
    }

    if (!token_is_digit) {
        throw std::invalid_argument("Invalid token: " + token);
    }
}

static std::vector<std::string> tokenizeInput(const std::string &input) {
    std::vector<std::string> tokenized_input;
    std::string digit;
    bool point_in_digit = false;

    for (int i = 0; i < input.size(); ++i) {
        if (isspace(input[i])) {
            continue;
        }

        if (isTokenBoundary(std::string(1, input[i]))) {
            flushDigitToken(tokenized_input, digit);
            point_in_digit = false;
            tokenized_input.push_back(std::string(1, input[i]));
        } else if (isdigit(input[i]) || input[i] == '-') {
            digit = digit + input[i];
        } else if (input[i] == '.' && !point_in_digit) {
            digit = digit + input[i];
            point_in_digit = true;
        } else {
            throw std::invalid_argument(std::string("Invalid character in input: ") + input[i]);
        }
    }

    flushDigitToken(tokenized_input, digit);

    for (int i = 0; i < tokenized_input.size(); ++i) {
        validateToken(tokenized_input[i]);
    }

    return tokenized_input;
}

}

std::vector<std::string> Tokenizer::processLogic(const std::string &input) {
    return tokenizeInput(input);
}

void Tokenizer::tokenizer(std::string &input, std::vector<std::string> &tokenized_input) {
    tokenized_input = processLogic(input);
}

}

