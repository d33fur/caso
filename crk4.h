#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>

namespace crk4 {
    class ODE {
        public:
            ODE() : xLeft(0.0), xRight(1.0), step(0.01) {}
            ODE(const std::string& equationString) {odeFunction = createFunction(equationString); }; 
            
            
            double rungeKutta(const double step) {
                // TODO дописать реализацию алгоритма рунге кутты
                // step тут нужен для того, что можно было если что поменять шаг, тем самым увеличить точность
                double x;
                odeFunction(x, y, dydx);
                return 0;
            }
            
            double xLeft;
            double xRight;
            double step;

            //double x, std::vector<double>& y, std::vector<double>& dydx
        private:
            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> createFunction(const std::string& expression) {
            // TODO дописать функцию-парсер
            return [expression](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {};
            }

            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> odeFunction;
            std::vector<double> y, dydx;
    };
};
