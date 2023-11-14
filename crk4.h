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
            
            
            void rungeKutta4(std::vector<double>& y0, double h, double t_target) {

                while (xLeft < xRight) {
                    for (int i = 0; i < ButcherTableau4.size(); ++i) {
                        odeFunction(xLeft + ButcherTableau4[i][0] * xStep, y, dydx);
                        std::vector<double> k;
                        multiplyVectorByScalar(k, dydx, xStep);

                        for (int j = 0; j < y.size(); ++j) {
                            y[j] += ButcherTableau4[i][i + 1] * k[j];
                        }
                    }

                    xLeft += xStep;
                }
                // Временный вывод для проверок
                for(auto i : y){
                    std::cout << i << ' ';
                }
                 
            }
            double xLeft = 0.0;
            double xRight = 1.0;
            double xStep = 0.1;

        private:
            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> odeFunction;
            std::vector<double> y, dydx;
            std::vector<double> yStart;

            std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)> createFunction(const std::string& expression) {
            // TODO дописать функцию-парсер
            
                return [expression](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {
                    // int n = 3;
                    // for(int i = 0; i < n; i++) {
                    //     dydx[i] = y[i];
                    // }
                    
                    // временная функция для проверок
                    dydx[0] = y[1];
                    dydx[1] = y[2];
                    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
                };
            }
            
            void multiplyVectorByScalar(std::vector<double>& newVec, std::vector<double>& vec, double scalar) {
                std::transform(vec.begin(), vec.end(), newVec.begin(),
                            [scalar](double element) { return element * scalar; });
            }

            const std::vector<std::vector<double>> ButcherTableau4 = {
                {0},
                {1 / 2, 1 / 2},
                {1 / 2, 0, 1 / 2},
                {1, 0, 0, 1},
                    {1 / 6, 1 / 3, 1 / 3, 1 / 6}
            };
            
            const std::vector<std::vector<double>> ButcherTableau8 = {
                {0},
                {1 / 5, 1 / 5},
                {3 / 10, 3 / 40, 9 / 40},
                {4 / 5, 44 / 45, -56 / 15, 32 / 9},
                {8 / 9, 19372 / 6561, -31760 / 2187, 10448 / 6561, 0, 25360 / 2187},
                {1, 9017 / 3168, -355 / 33, 46732 / 5247, 49 / 176, -5103 / 18656, 0, 5 / 143},
                {1 / 2, 35 / 384, 0, 500 / 1113, 125 / 192, -2187 / 6784, 11 / 84, 0},
                    {0, 35 / 384, 0, 500 / 1113, 125 / 192, -2187 / 6784, 11 / 84, 0}
            };
    };
};