#pragma once

namespace der {
    double derivative(const std::string variableName, const std::string equationString, int orderOfDerivative, double point);
    double derivative(const std::string variableName, const std::string equationString, double point);
}
