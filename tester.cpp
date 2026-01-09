#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <limits>
#include <functional>
#include "MathEngine.hpp" 

const double EPSILON = 0.001;
const bool SHOW_ONLY_FAILURES = false;

struct TestCase {
    std::string category;
    std::string expr;
    double expected;
};

bool isCorrect(double result, double expected) {
    if (std::isnan(expected)) return std::isnan(result);
    if (std::isinf(expected)) return std::isinf(result) && ((expected > 0) == (result > 0));
    if (std::isinf(expected) && result == 0.0) return true; 
    
    return std::abs(result - expected) < EPSILON;
}

int main() {
    std::vector<TestCase> tests;

    std::cout << "Generowanie testow..." << std::endl;
    tests.push_back({"Basic", ".2+2", 2.2});
    tests.push_back({"Basic", "2.5+2.5", 5.0});
    tests.push_back({"Basic", "10--20", 30.0});
    tests.push_back({"Basic", "0.1+0.2", 0.3});
    tests.push_back({"Order", "2+2*2", 6.0});
    tests.push_back({"Order", "(2+2)*2", 8.0});
    tests.push_back({"Order", "2*2^3", 16.0});
    tests.push_back({"Order", "2^3*2", 16.0});
    tests.push_back({"Order", "10/2*5", 25.0});
    tests.push_back({"Order", "2+3*4-5/2", 11.5});
    tests.push_back({"Unary", "-5+2", -3.0});
    tests.push_back({"Unary", "2+-5", -3.0});
    tests.push_back({"Unary", "2*-5", -10.0});
    tests.push_back({"Unary", "-2^2", -4.0});
    tests.push_back({"Unary", "(-2)^2", 4.0});
    tests.push_back({"Unary", "-(-5)", 5.0});
    tests.push_back({"Unary", "5--5", 10.0}); 
    tests.push_back({"Unary", "-sin(0)", 0.0});
    tests.push_back({"Unary", "10/-2", -5.0});
    tests.push_back({"Power", "2^3", 8.0});
    tests.push_back({"Power", "2^-1", 0.5});
    tests.push_back({"Power", "2^-2", 0.25});
    tests.push_back({"Power", "4^0.5", 2.0});
    tests.push_back({"Power", "2^(1+1)", 4.0});
    tests.push_back({"Power", "2^-(1+1)", 0.25});
    tests.push_back({"Power", "2^(-1)", 0.5});
    tests.push_back({"Power", "2^3^2", 512.0});
    tests.push_back({"Power", "(2^3)^2", 64.0});
    tests.push_back({"PowerHard", "2^-sin(0)", 1.0});
    tests.push_back({"PowerHard", "2^-sqrt(4)", 0.25});
    tests.push_back({"PowerHard", "10^-((1))", 0.1}); 
    tests.push_back({"Func", "sin(0)", 0.0});
    tests.push_back({"Func", "cos(0)", 1.0});
    tests.push_back({"Func", "sqrt(16)", 4.0});
    tests.push_back({"Func", "sqrt(2+2)", 2.0});
    tests.push_back({"Func", "sin(sqrt(0))", 0.0});
    tests.push_back({"Func", "sin(cos(0)+sin(0)-1)", 0.0});
    tests.push_back({"Func", "sqrt(3^2+4^2)", 5.0});
    tests.push_back({"Implicit", "2(3)", 6.0});
    tests.push_back({"Implicit", "(2)(3)", 6.0});
    tests.push_back({"Implicit", "2sqrt(16)", 8.0});
    tests.push_back({"Implicit", "(2+2)2", 8.0});
    tests.push_back({"Implicit", "2(2(2))", 8.0});
    tests.push_back({"Implicit", "2/-2^-sin(2)^-2^-3^2", -4.00051497579159});
    tests.push_back({"Implicit", "-2^-2^-2^2", -0.95760328069857});
    tests.push_back({"Implicit", "2/-2^2", -0.5});
    tests.push_back({"Implicit", "1/-sin(25)", 7.555609});
    tests.push_back({"Implicit", "-2^2", -4.0});
    tests.push_back({"Implicit", "-2/1----1", -1.0});
    tests.push_back({"Implicit", "-2/1-----3", -5.0});
    std::string sum_chain = "0";
    for(int i=0; i<300; ++i) sum_chain += "+1";
    tests.push_back({"Stress-Add", sum_chain, 300.0});
    std::string deep_open = "";
    std::string deep_close = "";
    int depth = 100; 
    for(int i=0; i<depth; ++i) { deep_open += "("; deep_close += ")"; }
    tests.push_back({"Stress-Parens", deep_open + "123" + deep_close, 123.0});
    std::string alt_chain = "0";
    for(int i=0; i<100; ++i) alt_chain += "+10-10";
    tests.push_back({"Stress-Alt", alt_chain, 0.0});
    tests.push_back({"DeepFunc", "sqrt(sqrt(sqrt(sqrt(256))))", 1.414214});    
    std::string sin_chain = "0";
    for(int i=0; i<50; ++i) sin_chain = "sin(" + sin_chain + ")";
    tests.push_back({"DeepFunc-Sin", sin_chain, 0.0});
    tests.push_back({"Edge", "", 0.0});
    tests.push_back({"Edge", "()", 0.0});
    tests.push_back({"Edge", "((()))", 0.0});
    tests.push_back({"Edge", "1/0", 0.0});
    tests.push_back({"Edge", "0/0", 0.0}); 
    tests.push_back({"Edge", "007", 7.0});
    tests.push_back({"Autofix", "((2+2", 4.0});
    tests.push_back({"Autofix", "2+2))", 4.0});

    std::cout << "Uruchamianie " << tests.size() << " testow..." << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(15) << "CATEGORY" 
              << std::setw(40) << "EXPRESSION (truncated)" 
              << std::setw(10) << "EXPECTED" 
              << std::setw(10) << "GOT" 
              << "STATUS" << std::endl;
    std::cout << "========================================================================" << std::endl;

    int passed = 0;
    int failed = 0;
    int crashed = 0;
    int testId = 1;

    for (const auto& t : tests) {
        double result = 0.0;
        bool hasException = false;
        
        try {
            result = MathEngine::mathengine(t.expr);
        } catch (const std::exception& e) {
            hasException = true;
        } catch (...) {
            hasException = true;
        }

        bool ok = !hasException && isCorrect(result, t.expected);

        if (ok) {
            passed++;
        } else {
            failed++;
            if (hasException) crashed++;
        }

        if (!ok || !SHOW_ONLY_FAILURES) {
             std::string dispExpr = t.expr;
             
             if (dispExpr.length() > 37) dispExpr = dispExpr.substr(0, 34) + "...";
             
             std::cout << std::left << std::setw(6) << testId 
                       << std::setw(15) << t.category 
                       << std::setw(40) << dispExpr
                       << std::setw(10) << t.expected 
                       << std::setw(10) << (hasException ? "ERR" : std::to_string(result))
                       << (ok ? "[ OK ]" : (hasException ? "[CRASH]" : "[FAIL]")) 
                       << std::endl;
        }
        testId++;
    }

    std::cout << "========================================================================" << std::endl;
    std::cout << "SUMMARY:" << std::endl;
    std::cout << "TOTAL:   " << tests.size() << std::endl;
    std::cout << "PASSED:  " << passed << std::endl;
    std::cout << "FAILED:  " << failed << std::endl;
    std::cout << "CRASHES: " << crashed << " (Exception thrown)" << std::endl;
    
    if (failed == 0 && crashed == 0) {
        std::cout << "\nCONGRATULATIONS! ENGINE IS ROBUST!" << std::endl;
    } else {
        std::cout << "\nFIX BUGS BEFORE RELEASE!" << std::endl;
    }

    return 0;
}