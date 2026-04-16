#pragma once
#include <string>
#include <vector>

namespace MathEngine {

class Engine {
private:
    static void OperatorSecurity(std::vector<std::string> &part_input);
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

}