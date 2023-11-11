#pragma once

namespace derivatives {
    class DerivativeCalculator {
    public:
        DerivativeCalculator();
        double calculate(const double x, const double h);
    };
}
