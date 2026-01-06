#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>
#include "Operator.hpp"
#include "Function.hpp"

class Engine {
private:
    static int operatorIndex(std::vector<std::string> &part_input);
    static double processLogic(std::vector<std::string> &part_input);
public:
    Engine() = delete;
    static double engine(std::vector<std::string> &part_input);
};

class Calculate {
private:
    static double processLogic(std::vector<std::string> &part_input);
public:
    Calculate() = delete;
    static double calculate(std::vector<std::string> &part_input);
};