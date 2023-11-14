#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>

namespace crk4 {
    class ODE {
        public:
            ODE() {}
            ODE(const std::string& equationString, double step) {odeFunction = createFunction(equationString); };
            
            
            double rungeKutta() { //const double step возможно на вход еще step для настройки точности
                // TODO дописать реализацию алгоритма рунге кутты
                while (xLeft < xRight) {
                    // я ничего не понял в каких случаях какую формулу использовать. время 6:22
                    // все что я понял, что нужно записать таблицу батчера и использовать ее для вычисления k



                    xLeft = xLeft + xStep;
                }
                return 0;
            }
            double xLeft = 0.0;
            double xRight = 1.0;
            double xStep = 0.1;

            //double x, std::vector<double>& y, std::vector<double>& dydx
        private:
            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> odeFunction;
            std::vector<double> y, dydx;
            std::vector<double> yStart;

            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> createFunction(const std::string& expression) {
            // TODO дописать функцию-парсер
            
                return [expression](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {
                    int n = 3;
                    for(int i = 0; i < n; i++) {
                        dydx[i] = y[i];
                    }
                };
            }
    };
};
