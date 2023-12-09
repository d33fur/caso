#include <iostream>
#include <iomanip>
#include "caso.h"
void f(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
}

void f0(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = -3 * y[0];
}

void f1(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = 3 * x * x * y[0] + x * x * std::pow(M_E, std::pow(x, 3));
}

void f2(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = -2 * x * y[0];
}

void f3(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = -2 * y[0] + x;
}

void f4(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = -2 * y[0] * y[0];
}

void f5(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = y[1];
    dydx[1] = - y[0] - y[0] * (3 * std::cos(2 * x) + 5);
}

int main() {
    std::cout << std::endl << "RungeKutta4" << std::endl;

    std::vector<double> y = {2}; //f3 or f4
    double xl = 0, xr = 3, xs = 0.25;
    y = {2, 0}; //f5
    
    caso::ODE a(f5, y, 0, xr, xs);
    std::vector<double> ansa = a.rungeKutta4();
    for(auto i : ansa) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "DormanPrince8" << std::endl;

    caso::ODE c(f5, y, xl, xr, xs);
    std::vector<double> ansc = c.rungeKuttaDormandPrince8();
    for(auto i : ansc) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "Heun2" << std::endl;

    caso::ODE c1(f5, y, xl, xr, xs);
    std::vector<double> ansc1 = c1.heun2();
    for(auto i : ansc1) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "Fehlberg6" << std::endl;

    caso::ODE c2(f5, y, xl, xr, xs);
    std::vector<double> ansc2 = c2.rungeKuttaFehlberg6();
    for(auto i : ansc2) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "BogackiShampine4" << std::endl;

    caso::ODE c3(f5, y, xl, xr, xs);
    std::vector<double> ansc3 = c3.rungeKuttaBogackiShampine4();
    for(auto i : ansc3) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "forwardEuler" << std::endl;

    caso::ODE g(f5, y, xl, xr, xs);
    std::vector<double> ansg = g.forwardEuler();
    for(auto i : ansg) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "backwardEuler" << std::endl;

    caso::ODE j(f5, y, xl, xr, xs);
    std::vector<double> ansj = j.backwardEuler();
    for(auto i : ansj) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "midpoint" << std::endl;

    caso::ODE k(f5, y, xl, xr, xs);
    std::vector<double> ansk = k.midpoint();
    for(auto i : ansk) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    std::cout << std::endl << "implicitMidpoint" << std::endl;

    caso::ODE l(f5, y, xl, xr, xs);
    std::vector<double> ansl = l.implicitMidpoint();
    for(auto i : ansl) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }
    return 0;
}