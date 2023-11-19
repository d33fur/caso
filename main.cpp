#include <iostream>
#include <iomanip>
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

void f2(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = -2 * x * y[0];
}

int main() {
    std::vector<double> y = {2};
    double xl = 1, xr = 3, xs = 0.25;
    caso::ODE a(f2, y, xl, xr, xs);
    a.setButcherTableau(caso::RungeKutta4);
    std::vector<double> ansa = a.rungeKutta();
    for(auto i : ansa) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }
    
    std::cout << std::endl;

    caso::ODE c(f2, y, xl, xr, xs);
    c.setButcherTableau(caso::DormanPrince8);
    std::vector<double> ansc = c.rungeKutta();
    for(auto i : ansc) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl;

    caso::ODE g(f2, y, xl, xr, xs);
    std::vector<double> ansg = g.euler();
    for(auto i : ansg) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }
    return 0;
}