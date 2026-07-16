#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <limits>
#include "MathEngine.hpp"

using namespace MathEngine;

const double EPSILON = 0.001;
const bool SHOW_ONLY_FAILURES = false;

struct TestCase {
    std::string category;
    std::string expr;
    double expected;
    bool expectError = false; 
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
    
    tests.push_back({"UnaryRefactor", "--3", 3.0});
    tests.push_back({"UnaryRefactor", "---3", -3.0});
    tests.push_back({"UnaryRefactor", "-(2+3)^2", -25.0});
    tests.push_back({"UnaryRefactor", "2*-(3+1)", -8.0});
    tests.push_back({"UnaryRefactor", "(-3)^2", 9.0});
    tests.push_back({"UnaryRefactor", "-2*-3", 6.0});
    tests.push_back({"UnaryRefactor", "--2^2", 4.0});
    tests.push_back({"UnaryRefactor", "-(-(-2))", -2.0});
    tests.push_back({"UnaryRefactor", "2^--3", 8.0});
    tests.push_back({"UnaryRefactor", "-(1)", -1.0});
    tests.push_back({"UnaryRefactor", "-(-(1))", 1.0});
    tests.push_back({"UnaryRefactor", "3*-(2^2)", -12.0});
    tests.push_back({"UnaryRefactor", "((2+3)*(4+1) - 6/3)^2", 529.0});

    tests.push_back({"UnaryRefactor", "(((12+34)*(56-78))/2)+((90*12)-(34/2))", 557.0});
    tests.push_back({"UnaryRefactor", "((1+2)*(3+4)*(5+6)*(7+8))/(1+1+1+1)", 866.25});
    tests.push_back({"UnaryRefactor", "((100-(20*3))+(40/(5-3)))-(10*(2+1))", 30.0});
    tests.push_back({"UnaryRefactor", "((((5+5)*5)-5)*5)-5", 220.0});
    tests.push_back({"UnaryRefactor", "1+(2+(3+(4+(5+(6+(7+(8+(9+10))))))))", 55.0});
    tests.push_back({"UnaryRefactor", "((10-2)*3-(5+4)*2)*(8/2+1)", 30.0});
    tests.push_back({"UnaryRefactor", "((2^3)*(3^2)-(4^2))*2 - 100", 12.0});
    tests.push_back({"UnaryRefactor", "(100/2/2/2)-(50/5/5)", 10.5});
    tests.push_back({"UnaryRefactor", "((4+8)*(12-4))/((6+2)*(5-3))", 6.0});
    tests.push_back({"UnaryRefactor", "((15+5)*(20-10))/((25-5)*(30-20))", 1.0});
    tests.push_back({"UnaryRefactor", "((9+3)*(8-2)-(7+1)*(6-3))^2", 2304.0});
    tests.push_back({"UnaryRefactor", "(((2+2)^2)^2)-250", 6.0});
    tests.push_back({"UnaryRefactor", "(1+2+3+4+5)*(1-2+3-4+5)", 45.0});
    tests.push_back({"UnaryRefactor", "((50-10*4)+20)/((30/3)-5)", 6.0});
    tests.push_back({"UnaryRefactor", "((100+200)*3)/((400-100)/2)", 6.0});
    tests.push_back({"UnaryRefactor", "((3*4)+(5*6)-(7*8)) * (2+2)", -56.0});
    tests.push_back({"UnaryRefactor", "((1.5+2.5)*(4.5-0.5))^2", 256.0});
    tests.push_back({"UnaryRefactor", "((((10-2)*2)-4)*2)-8", 16.0});
    tests.push_back({"UnaryRefactor", "(120/2)/(60/4)*(10-2)", 32.0});
    tests.push_back({"UnaryRefactor", "((9^2)-(8^2))*((7^2)-(6^2))", 221.0});
    tests.push_back({"UnaryRefactor", "(((3+1)^2)+4)/((2^2)+1)", 4.0});
    tests.push_back({"UnaryRefactor", "((10*10)-(9*9))*((8*8)-(7*7))", 285.0});
    tests.push_back({"UnaryRefactor", "((25/5)+(36/6))*((49/7)-(16/4))", 33.0});
    tests.push_back({"UnaryRefactor", "((5*5*5)-(4*4*4))/(2+1)", 20.333333333});
    tests.push_back({"UnaryRefactor", "((10-5)*(20-10)*(30-15))/125", 6.0});
    tests.push_back({"UnaryRefactor", "(((2+3)^2)-(4-1)^2)^2", 256.0});
    tests.push_back({"UnaryRefactor", "(100-(50-(25-(10-5))))", 70.0});
    tests.push_back({"UnaryRefactor", "((12+18)/5)*((40-16)/4)", 36.0});
    tests.push_back({"UnaryRefactor", "((((2*2)*2)*2)*2)-30", 2.0});
    tests.push_back({"UnaryRefactor", "2+3*4-5/1+2^3", 17.0});
    tests.push_back({"UnaryRefactor", "100-20*4+50/2^2", 32.5});
    tests.push_back({"UnaryRefactor", "5*5+4*4-3*3+2*2^3", 48.0});
    tests.push_back({"UnaryRefactor", "1+2*3^2*4+5", 78.0});
    tests.push_back({"UnaryRefactor", "10-2-3-4-5*0", 1.0});
    tests.push_back({"UnaryRefactor", "2^4/4^2*5*5-20", 5.0});
    tests.push_back({"UnaryRefactor", "0.5*4+1.5*2-3^2/3", 2.0});
    tests.push_back({"UnaryRefactor", "100/5/5*2^3-10", 22.0});
    tests.push_back({"UnaryRefactor", "3*4^2/2+5-1", 28.0});
    tests.push_back({"UnaryRefactor", "7+7/7+7*7-7", 50.0});
    tests.push_back({"UnaryRefactor", "2*3+4*5-6/2+2^2", 27.0});
    tests.push_back({"UnaryRefactor", "10-10*0+10/10", 11.0});
    tests.push_back({"UnaryRefactor", "5^2-4^2+3^2-2^2+1^2", 15.0});
    tests.push_back({"UnaryRefactor", "2.5*4+10/0.5-5^2", 5.0});
    tests.push_back({"UnaryRefactor", "1+1+1+1*0+1+1", 5.0});
    tests.push_back({"UnaryRefactor", "-5+-5", -10.0});
    tests.push_back({"UnaryRefactor", "-(-5)", 5.0});
    tests.push_back({"UnaryRefactor", "-2^2", -4.0});
    tests.push_back({"UnaryRefactor", "(-2)^2", 4.0});
    tests.push_back({"UnaryRefactor", "-10*-2/-5", -4.0});
    tests.push_back({"UnaryRefactor", "-((2+3)*-2)", 10.0});
    tests.push_back({"UnaryRefactor", "-1-1-1-1", -4.0});
    tests.push_back({"UnaryRefactor", "5*-2--3", -7.0});
    tests.push_back({"UnaryRefactor", "-(10-(5--2))", -3.0});
    tests.push_back({"UnaryRefactor", "-0.5*-4.0", 2.0});
    tests.push_back({"UnaryRefactor", "-2^-3", -0.125});
    tests.push_back({"UnaryRefactor", "(-3)^-2", 0.111111111});
    tests.push_back({"UnaryRefactor", "-100/(-2)/(-2)", -25.0});
    tests.push_back({"UnaryRefactor", "-((-10+5)*-2)", -10.0});
    tests.push_back({"UnaryRefactor", "-5^2-(-5)^2", -50.0});
    tests.push_back({"Complex", "10+2*(5-3)^3/-4--sin(0)", 6.0});
    tests.push_back({"Complex", "(((1+2)*3)-4)^2/5", 5.0});
    tests.push_back({"Complex", "2*(3+4*(5-2))-10/2", 25.0});
    tests.push_back({"Complex", "(1.5*2+3.5*2-10)^2/0.01", 0.0});
    tests.push_back({"WeirdMix", "-(2^2)^-1*-8", 2.0});
    tests.push_back({"WeirdMix", "-sqrt(16)^-1*-4", 1.0});
    tests.push_back({"WeirdMix", "--2^---3", 0.125});        
    tests.push_back({"WeirdMix", "-(-2)^3", 8.0});           
    tests.push_back({"WeirdMix", "2/(-2)/(-2)", 0.5});       
    tests.push_back({"WeirdMix", "-2^2^3", -256.0});
    tests.push_back({"ImplicitHard", "2(3+4)(5-1)", 56.0});  
    tests.push_back({"ImplicitHard", "-(2)-(-3)(2)", 4.0});  
    tests.push_back({"ImplicitHard", "2sqrt(9)cos(0)", 6.0}); 
    tests.push_back({"ImplicitHard", "((2)3)4", 24.0});      
    tests.push_back({"ImplicitHard", "-2(3)^2", -18.0});
    tests.push_back({"Monster", "sqrt(10^2-6^2)+((2^3)^2/4)-sqrt(25)*-2", 34.0}); 
    tests.push_back({"Monster", "2^3*3^2/2^2*4^0.5-10", 26.0});                  
    tests.push_back({"Monster", "sin(0)+cos(0)*sqrt(16)/2^2*(-1+2)", 1.0});      
    tests.push_back({"Monster", "100/10/2/0.5*3", 30.0});                
    tests.push_back({"Monster", "2+3-4*5/6^2+7", 11.4444444444});
    tests.push_back({"Syntax-Err", "2+*3", 0.0, true});
    tests.push_back({"Syntax-Err", "/5", 0.0, true});
    tests.push_back({"Syntax-Err", "()2", 0.0, true});
    tests.push_back({"Syntax-Err", "sin(2+)", 0.0, true});
    tests.push_back({"Syntax-Err", "3.14.15", 0.0, true});

    tests.push_back({"Advanced", "((2+3)*(4+1) - 6/3)^2", 529.0});
    tests.push_back({"Advanced", "2*(3+4)^2 - 3^2*(4-1)", 71.0});
    tests.push_back({"Advanced", "2^(2+1) + 3^(2-1)", 11.0});
    tests.push_back({"Advanced", "3*(2+sqrt(16)) - 2^3", 10.0});
    tests.push_back({"Advanced", "sqrt((3+4)^2 + (4-4)^2)", 7.0});
    tests.push_back({"Advanced", "2^3^2^1", 512.0});
    tests.push_back({"Advanced", "(-2)^3^2", -512.0});
    tests.push_back({"Advanced", "2(3+4) + 5(1+1)", 24.0});
    tests.push_back({"Advanced", "((8/2)/2)^2", 4.0});
    tests.push_back({"Advanced", "-(2^3) + (4^2)", 8.0});
    tests.push_back({"Advanced", "sin(0)^2 + cos(0)^2", 1.0});

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
    
    tests.push_back({"Edge", "007", 7.0});

    tests.push_back({"Edge-Err", "", 0.0, true});
    tests.push_back({"Edge-Err", "()", 0.0, true});
    tests.push_back({"Edge-Err", "((()))", 0.0, true});
    tests.push_back({"Math-Err", "1/0", 0.0, true});
    tests.push_back({"Math-Err", "0/0", 0.0, true}); 
    tests.push_back({"Math-Err", "sqrt(-16)", 0.0, true}); 
    
    tests.push_back({"Auto-Parens", "((2+2", 4.0, false}); 
    tests.push_back({"Auto-Parens", "2+2))", 4.0, false});
    
    tests.push_back({"Syntax-Err", "2**3", 0.0, true});
    tests.push_back({"Syntax-Err", "*5+2", 0.0, true});
    tests.push_back({"Syntax-Err", "2+x", 0.0, true});


    std::cout << "Uruchamianie " << tests.size() << " testow..." << std::endl;
    std::cout << "==========================================================================================" << std::endl;
    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(15) << "CATEGORY" 
              << std::setw(30) << "EXPRESSION" 
              << std::setw(10) << "EXPECTED" 
              << std::setw(10) << "GOT" 
              << "STATUS" << std::endl;
    std::cout << "==========================================================================================" << std::endl;

    int passed = 0;
    int failed = 0;
    int testId = 1;

    for (const auto& t : tests) {
        CalculationResult res = Math::calculateSafe(t.expr);
        bool ok = false;
        
        if (t.expectError) {
            ok = !res.isSuccess;
        } else {
            ok = res.isSuccess && isCorrect(res.value, t.expected);
        }

        if (ok) {
            passed++;
        } else {
            failed++;
        }

        if (!ok || !SHOW_ONLY_FAILURES) {
             std::string dispExpr = t.expr;
             if (dispExpr.length() > 27) dispExpr = dispExpr.substr(0, 24) + "...";
             
             std::string status = ok ? "[ OK ]" : "[FAIL]";
             
             std::string expStr = t.expectError ? "ERROR" : std::to_string(t.expected);
             if (!t.expectError) { 
                 expStr.erase(expStr.find_last_not_of('0') + 1, std::string::npos);
                 if (expStr.back() == '.') expStr.pop_back();
             }

             std::string gotStr;
             if (res.isSuccess) {
                 gotStr = std::to_string(res.value);
                 gotStr.erase(gotStr.find_last_not_of('0') + 1, std::string::npos);
                 if (gotStr.back() == '.') gotStr.pop_back();
             } else {
                 gotStr = "ERR";
             }

             std::cout << std::left << std::setw(6) << testId 
                       << std::setw(15) << t.category 
                       << std::setw(30) << dispExpr
                       << std::setw(10) << expStr 
                       << std::setw(10) << gotStr
                       << status;
                       
             if (!res.isSuccess) {
                 std::cout << " -> " << res.errorMessage;
             }
             std::cout << std::endl;
        }
        testId++;
    }

    std::cout << "==========================================================================================" << std::endl;
    std::cout << "SUMMARY:" << std::endl;
    std::cout << "TOTAL:   " << tests.size() << std::endl;
    std::cout << "PASSED:  " << passed << std::endl;
    std::cout << "FAILED:  " << failed << std::endl;
    
    if (failed == 0) {
        std::cout << "\nCONGRATULATIONS! ENGINE IS ROBUST AND SAFE!" << std::endl;
    } else {
        std::cout << "\nFIX BUGS BEFORE RELEASE!" << std::endl;
    }

    return 0;
}