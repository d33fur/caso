#include <iostream>
#include "caso.h"
void f(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
}

void f0(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = -3 * y[0];
}

void f1(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = 3 * x * x * y[0] + x * x * std::pow(M_E, std::pow(x, 3));
}

int main() {
    caso::ODE b(f, {-1, 2, 3}, 0, 1);
    b.setButcherTableau(caso::RungeKutta4);
    //b.setStartValuesAndBorders({-1, 2, 3}, 0, 1, 0.01);
    std::vector<double> ansb = b.solve(0.01);
    for(auto i : ansb) {
        std::cout << i << ' ';
    }
    return 0;
}