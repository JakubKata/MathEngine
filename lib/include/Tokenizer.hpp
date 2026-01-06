#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "Operator.hpp"
#include "Function.hpp"

class Tokenizer {
private:
    static std::vector<std::string> processLogic(const std::string &input) ;
public:
    Tokenizer() = delete;
    static void tokenizer(std::string &input, std::vector<std::string> &tokenized_input);
};
