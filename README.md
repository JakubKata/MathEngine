# MathEngine

MathEngine is a lightweight C++17 expression engine.
It parses and evaluates math expressions such as:

- `2+2*2`
- `sqrt(16)+sin(0)`
- `2^3^2`
- `2(3+4)` (implicit multiplication)
- `((2+3)*(4+1)-6/3)^2`
- `sqrt((3+4)^2 + (4-4)^2)`

The project builds:

- `MathCore` (static library)
- `MathTester` (test executable)

---

## Features

- Operator precedence and parentheses
- Exponentiation (`^`) with right associativity
- Built-in functions: `sqrt`, `sin`, `cos`, `tan`
- Floating-point numbers and unary operators
- Implicit multiplication, for example `2(3)` and `2sqrt(16)`
- Safe API with structured error reporting (`calculateSafe`)
- Expanded regression coverage for nested parentheses, chained unary operators, exponent precedence, and mixed function/operator expressions

---

## Requirements

- CMake `>= 3.10`
- A C++17 compiler:
  - Windows: MSVC (Visual Studio)
  - Linux/macOS: GCC or Clang

---

## Build

### Configure

```bash
cmake -S . -B build
```

### Compile

Windows:

```powershell
cmake --build build --config Debug
```

Linux/macOS:

```bash
cmake --build build
```

Alternative (if you are already in the `build` directory):

```bash
cmake ..
cmake --build .
```

### Run tests

Windows:

```powershell
.\build\tests\Debug\MathTester.exe
```

Linux/macOS:

```bash
./build/tests/MathTester
```

The test runner exercises a broad set of cases, including:

- basic arithmetic and precedence
- unary operator chains such as `--2^2`
- right-associative exponentiation like `2^3^2`
- implicit multiplication such as `2(3+4)`
- nested expressions like `sqrt((3+4)^2 + (4-4)^2)`
- error cases for invalid syntax and invalid math operations

---

## Usage Example

```cpp
#include <iostream>
#include "MathEngine.hpp"

int main() {
    using namespace MathEngine;

    // Exception-throwing API
    double value = Math::calculate("2+2*2");
    std::cout << value << "\n"; // 6

    // Safe API
    CalculationResult res = Math::calculateSafe("sqrt(16)+cos(0)");
    if (res.isSuccess) {
        std::cout << res.value << "\n";
    } else {
        std::cerr << "Error: " << res.errorMessage << "\n";
    }

    // More complex expression
    std::cout << Math::calculate("((2+3)*(4+1)-6/3)^2") << "\n";
}
```

Public headers are available in `lib/include`.

---

## Project Structure

```text
MathEngine/
├── CMakeLists.txt
├── README.md
├── lib/
│   ├── include/
│   └── src/
└── tests/
    ├── CMakeLists.txt
    └── tester.cpp
```

---

## License

MIT

