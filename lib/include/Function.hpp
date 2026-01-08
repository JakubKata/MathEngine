#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>

class Function{
public:
    virtual ~Function() {}
    virtual double calculate(double token) const = 0;
};

class FunctionSqrt : public Function {
public:
    double calculate(double token) const override {
        if (token < 0) return 0.0; 
        return sqrt(token);
    }

};

class FunctionSin : public Function {
public:
    double calculate(double token) const override {
        return sin(token);
    }

};
class FunctionCos : public Function {
public:
    double calculate(double token) const override {
        return cos(token);
    }

};

class FunctionTan : public Function {
public:
    double calculate(double token) const override {
        return tan(token);
    }

};

class FunctionFactory {
public:
    static bool formatterFunction(std::string &input, std::string &input_formate, int &i) {
        if (i + 4 < input.size() && input[i] == 's' && input[i+1] == 'q' && input[i+2] == 'r' && input[i+3] == 't' && input[i+4] == '(') {
            input_formate = input_formate + "(S";
            i = i + 4;
            return true;
        } else if (i + 3 < input.size() && input[i] == 's' && input[i+1] == 'i' && input[i+2] == 'n' && input[i+3] == '(') {
            input_formate = input_formate + "(I";
            i = i + 3;
            return true;
        } else if (i + 3 < input.size() && input[i] == 'c' && input[i+1] == 'o' && input[i+2] == 's' && input[i+3] == '(') {
            input_formate = input_formate + "(C";
            i = i + 3;
            return true;
        } else if (i + 3 < input.size() && input[i] == 't' && input[i+1] == 'a' && input[i+2] == 'n' && input[i+3] == '(') {
            input_formate = input_formate + "(T";
            i = i + 3;
            return true;
        } 
        return false;        
    }

    static bool isFunction(std::string token) {
        if (token == "S" || token == "I" || token == "C" || token == "T") {
            return true;
        } else {
            return false;
        }
    }
   
    static Function* getFunction(std::string token) {
        if (token == "S") {
            static FunctionSqrt op; 
            return &op;          
        } else if (token == "I") {
            static FunctionSin op;
            return &op;
        } else if (token == "C") {
            static FunctionCos op;
            return &op;
        } else if (token == "T") {
            static FunctionTan op;
            return &op;
        }
    }
};