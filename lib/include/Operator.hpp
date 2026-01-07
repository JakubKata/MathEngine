#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>

class Operator {
public:
    virtual ~Operator() {}
    virtual double calculate(double left, double right) const = 0;
    virtual int getPriority() const = 0;
    virtual bool isLeftToRight() const = 0;
};

class AddOperator : public Operator {
public:
    double calculate(double left, double right) const override {
        return left + right;
    }
    int getPriority() const override {
        return 1;
    }
    bool isLeftToRight() const override {
        return true;
    }
};

class MultiplyOperator : public Operator {
public:
    double calculate(double left, double right) const override {
        return left * right;
    }  
    int getPriority() const override {
        return 2;
    }
    bool isLeftToRight() const override {
        return true;
    }
};

class DivideOperator : public Operator {
public:
    double calculate(double left, double right) const override {
        if (right == 0) return 0.0; 
        return left / right;
    }
    int getPriority() const override {
        return 2;
    }
    bool isLeftToRight() const override {
        return true;
    }
};

class PowerOperator : public Operator {
public:
    double calculate(double left, double right) const override {
        return std::pow(left, right);
    }
    int getPriority() const override {
        return 3;
    }
    bool isLeftToRight() const override {
        return false;
    }
};

class OperatorFactory {
public:
    static bool isOperator(std::string token) {
        if (token == "+" || token == "*" || token == "/" || token == "^") {
            return true;
        } else {
            return false;
        }
    }
    static bool isNumber(std::string token) {
        for (int i = 0; i < token.size(); ++i) {
            if ((token[i] == '-' && i == 0) || isdigit(token[i]) || token[i] == '.') {
                return true;
            } 
        }
        return false;
    }    
    static Operator* getOperator(std::string token) {
        if (token == "+") {
            static AddOperator op; 
            return &op;          
        } else if (token == "*") {
            static MultiplyOperator op;
            return &op;
        } else if (token == "/") {
            static DivideOperator op;
            return &op;
        } else if (token == "^") {
            static PowerOperator op;
            return &op;
        }
    }
};