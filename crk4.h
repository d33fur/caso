#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>

namespace crk4 {
    class ODE {
        using odeF = std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)>;

        public:
            ODE() {}
            ODE(const std::string& equationString) 
                : odeFunction(createFunction(equationString)) {}
            ODE(const std::string& equationString, double xStep) 
                : odeFunction(createFunction(equationString)), xStep(xStep) {}
            ODE(const std::string& equationString, std::vector<double> y0, double xLeft, double xRight) 
                : odeFunction(createFunction(equationString)), y0(y0), xLeft(xLeft), xRight(xRight) {}
            ODE(const std::string& equationString, std::vector<double> y0, double xLeft, double xRight, double xStep) 
                : odeFunction(createFunction(equationString)), y0(y0), xLeft(xLeft), xRight(xRight), xStep(xStep) {}

            ODE(const odeF& odeFunction)
                : odeFunction(odeFunction) {}
            ODE(const odeF& odeFunction, double xStep)
                : odeFunction(odeFunction), xStep(xStep) {}
            ODE(const odeF& odeFunction, std::vector<double> y0, double xLeft, double xRight) 
                : odeFunction(odeFunction), y0(y0), xLeft(xLeft), xRight(xRight) {}
            ODE(const odeF& odeFunction, std::vector<double> y0, double xLeft, double xRight, double xStep) 
                : odeFunction(odeFunction), y0(y0), xLeft(xLeft), xRight(xRight), xStep(xStep) {}
                
            ~ODE(){}
            
            void setEquationWithString(const std::string& equationString) {
                odeFunction = createFunction(equationString);
            }
            
            void setEquationWithFunction(const odeF& inputOdeFunction) {
                odeFunction = inputOdeFunction;
            }

            void rungeKutta4(double xStep = 0.01) { // опционально xStep в аргумент
                y = y0;

                while (xLeft < xRight) {
                    std::vector<double> temp(y.size());
                    std::vector<std::vector<double>> k(4, std::vector<double>(y.size()));

                    for (int i = 0; i < k.size(); i++) {
                        temp = y;
                        addVectorByScalar(temp, k[i], ButcherTableau4[i][0] * xStep);
                        odeFunction(xLeft + ButcherTableau4[i][0] * xStep, temp, k[i]);
                    }

                    for (int i = 0; i < y.size(); ++i) { //вычисляем y с использованием k
                        y[i] += (xStep / 6.0) * (k[0][i] + 2 * k[1][i] + 2 * k[2][i] + k[3][i]);
                    }

                    xLeft += xStep;

                    // Вывод результатов
                    std::cout << "x = " << xLeft << ", y = " << y[0] << ", y' = " << y[1] << ", y'' = " << y[2] << std::endl;
                }
            }
            double xLeft; // левая граница x
            double xRight; // правая граница x
            double xStep; // шаг
            std::vector<double> y0; // начальные y
            

        private:
            odeF odeFunction; // функция или система уравнения
            std::vector<double> y, dydx; // вектор производных и замен этих же самых производных

            odeF createFunction(const std::string& expression) {
            // TODO дописать функцию-парсер
            
                return [expression](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {
                    // int n = 3;
                    // for(int i = 0; i < n; i++) {
                    //     dydx[i] = y[i];
                    // }
                };
            }
            
            void multiplyVectorByScalar(std::vector<double>& newVec, std::vector<double>& vec, double scalar) {
                std::transform(vec.begin(), vec.end(), newVec.begin(),
                            [scalar](double element) { return element * scalar; });
            }

            void addVectorByScalar(std::vector<double>& result, std::vector<double>& vec, double scalar) {
                std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                            [scalar](double i1, double i2) { return i1 + i2 * scalar; });
            }

            const std::vector<std::vector<double>> ButcherTableau4 = {
                {0.},
                {1. / 2., 1. / 2.},
                {1. / 2., 0., 1. / 2.},
                {1., 0., 0., 1.},
                    {1. / 6., 1. / 3., 1. / 3., 1. / 6.}
            };
            
            const std::vector<std::vector<double>> ButcherTableau8 = {
                {0.},
                {1. / 5., 1. / 5.},
                {3. / 10., 3. / 40., 9. / 40.},
                {4. / 5., 44. / 45., -56. / 15., 32. / 9.},
                {8. / 9., 19372. / 6561., -31760. / 2187., 10448. / 6561., 0., 25360. / 2187.},
                {1., 9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176., -5103. / 18656., 0., 5. / 143.},
                {1. / 2., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.},
                    {0., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.}
            };
    };
};