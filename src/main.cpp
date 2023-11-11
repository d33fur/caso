#include <iostream>
#include "derivative.h"

int main() {
    std::string equationString = "8x";
    int orderOfDerivative = 1;
    double point = 1;
    std::string variableName = "x";
    auto result0 = der::derivative(variableName, equationString, orderOfDerivative, point);
    auto result1 = der::derivative(variableName, equationString, point);
    std::cout << result0 << ' ' << result1 << std::endl;
    return 0;
}