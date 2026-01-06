#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "Operator.hpp"
#include "Function.hpp"

class Formater {
private:
    static std::string processLogic(std::string &input);
public:
    Formater() = delete;
    static void formater(std::string &input);
};