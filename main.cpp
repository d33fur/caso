#include <iostream>
#include "crk4.h"
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
    //crk4::ODE b(f, {-1, 2, 3}, 0, 1, 0.01);

    std::string str = "y'+3*y=0, y(0)=-1, x[0;1], h = 0.01 "; //
    crk4::ODE b(str, 'y');
    std::vector<double> ans = b.rungeKutta4();

    for(auto i : ans) {
        std::cout << i << ' ';
    }
    return 0;
}