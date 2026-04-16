#pragma once
#include <string>
#include <vector>

namespace MathEngine {

class Algorithm {
private:
    static void parenthesis_index(const std::vector<std::string> &tokenized_input, int &parenthesis_open_index, int &parenthesis_close_index);
    static double processLogic(std::vector<std::string> &tokenized_input);
public:
    Algorithm() = delete;
    static void algorithm(std::vector<std::string> &tokenized_input, double &solution);
};

}