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
    caso::ODE a(f, {-1, 2, 3}, 0, 1, 0.01);
    a.setButcherTableau(caso::RungeKutta4);
    std::vector<double> ansa = a.rungeKutta();
    for(auto i : ansa) {
        std::cout << i << ' ';
    }

    std::cout << std::endl;

    caso::ODE b(f, {-1, 2, 3}, 0, 1, 0.01);
    b.setButcherTableau(caso::RosenbrockGilbert4);
    std::vector<double> ansb = b.rungeKutta();
    for(auto i : ansb) {
        std::cout << i << ' ';
    }
    
    std::cout << std::endl;

    caso::ODE c(f, {-1, 2, 3}, 0, 1, 0.01);
    c.setButcherTableau(caso::DormanPrince8);
    std::vector<double> ansc = c.rungeKutta();
    for(auto i : ansc) {
        std::cout << i << ' ';
    }

    std::cout << std::endl;

    caso::ODE d(f, {-1, 2, 3}, 0, 1, 0.01);
    std::vector<double> ansd = d.adamsBashforth();
    for(auto i : ansd) {
        std::cout << i << ' ';
    }

    std::cout << std::endl;

    caso::ODE g(f, {-1, 2, 3}, 0, 1, 0.01);
    std::vector<double> ansg = g.euler();
    for(auto i : ansg) {
        std::cout << i << ' ';
    }
    return 0;
}