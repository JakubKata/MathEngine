#pragma once
#include <string>

namespace MathEngine {

class Formater {
private:
    static std::string processLogic(std::string &input);
public:
    Formater() = delete;
    static void formater(std::string &input);
};

}