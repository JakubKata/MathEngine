cmake_minimum_required(VERSION 3.10)

project(MathEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W3) 
else()
    add_compile_options(-Wall -Wextra) 
endif()

add_library(MathCore STATIC
    lib/src/Algorithm.cpp
    lib/src/Calculate.cpp
    lib/src/Formatter.cpp
    lib/src/MathEngine.cpp
    lib/src/Tokenizer.cpp
     
    lib/include/Algorithm.hpp
    lib/include/Calculate.hpp
    lib/include/Formatter.hpp
    lib/include/Function.hpp
    lib/include/MathEngine.hpp
    lib/include/Operator.hpp
    lib/include/Tokenizer.hpp
)

target_include_directories(MathCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)

add_executable(MathTester tester.cpp)

target_link_libraries(MathTester PRIVATE MathCore)