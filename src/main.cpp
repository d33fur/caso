#include <iostream>
#include "derivative.h"

int main() {
    derivatives::DerivativeCalculator calculator;
    double x = 2.0;
    double h = 0.0001;
    double result = calculator.calculate(x, h);

    std::cout << "Derivative at x = " << x << ": " << result << std::endl;

    return 0;
}