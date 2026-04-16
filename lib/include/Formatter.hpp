#pragma once
#include <string>

namespace MathEngine {

class Formatter {
private:
    static std::string processLogic(std::string &input);
public:
    Formatter() = delete;
    static void formatter(std::string &input);
};

}