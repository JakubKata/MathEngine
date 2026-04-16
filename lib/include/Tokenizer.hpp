#pragma once
#include <string>
#include <vector>

namespace MathEngine {

class Tokenizer {
private:
    static std::vector<std::string> processLogic(const std::string &input) ;
public:
    Tokenizer() = delete;
    static void tokenizer(std::string &input, std::vector<std::string> &tokenized_input);
};

}
