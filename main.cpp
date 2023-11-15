#include <iostream>
#include "crk4.h"
void f(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
}

int main() {
    //crk4::ODE a("a", 0.1);
    crk4::ODE b(f, {-1, 2, 3}, 0, 1, 0.01);
    b.rungeKutta4();
    //crk4::ODE a("a", 2.0);
    //std::cout << a.xStep;
    return 0;
}