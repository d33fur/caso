#include <iostream>
#include <iomanip>
#include "caso.h"

void f(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
}

int main() {
    caso::ODE a(f, {-1, 2, 3}, 0, 1, 0.01);
    std::vector<double> answer = a.rungeKutta4();
    
    for(auto i : answer) {
        std::cout << std::fixed << std::setprecision(10) << i << ' ';
    }

    return 0;
}