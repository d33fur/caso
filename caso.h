#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace caso {

    enum ButcherTableau {
        RungeKutta4,
        RosenbrockGilbert4,
        DormanPrince8,
    };


    class ODE {
        using odeS = std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)>;

    public:
        ODE () = default;
        ODE(const odeS& odeSystem)
                : odeSystem(odeSystem) {}
        ODE(const odeS& odeSystem, std::vector<double> yStartValues, double xLeft, double xRight)
                : odeSystem(odeSystem), yStart(yStartValues), xLeft(xLeft), xRight(xRight) {}
        ODE(const odeS& odeSystem, std::vector<double> yStartValues, double xLeft, double xRight, double step)
                : odeSystem(odeSystem), yStart(yStartValues), xLeft(xLeft), xRight(xRight), xStep(step) {}

        ~ODE() = default;


        void setEquationWithSystem(const odeS& inputOdeSystem) {
            odeSystem = inputOdeSystem;
        }

        void setStartValuesAndBorders(std::vector<double> yStartValues, double inputXLeft, double inputXRight, double step) {
            yStart = yStartValues;
            xLeft = inputXLeft;
            xRight = inputXRight;
            xStep = step;
        }

        void setButcherTableau(ButcherTableau tableName) {
            if (butcherTables.find(tableName) != butcherTables.end()) {
                currentButcherTableau = butcherTables[tableName];
            } else {
                throw std::invalid_argument("Invalid Butcher table name.");
            }
        }

        std::vector<double> solve(double step = -1.0) {
            if (step > 0) {
                xStep = step;
            }

            validateParameters();
            y = yStart;

            while (xLeft < xRight) {
                rungeKutta4Iteration();
                xLeft += xStep;
            }

            return y;
        }



    private:
        void rungeKutta4Iteration() {
            std::vector<double> temp(y.size());
            std::vector<std::vector<double>> k(4, std::vector<double>(y.size()));

            for (size_t i = 0; i < k.size(); i++) {
                temp = y;
                addVectorByScalar(temp, k[i], currentButcherTableau[i][0] * xStep);
                odeSystem(xLeft + currentButcherTableau[i][0] * xStep, temp, k[i]);
            }

            for (size_t i = 0; i < y.size(); ++i) {
                y[i] += (xStep / 6.0) * (k[0][i] + 2 * k[1][i] + 2 * k[2][i] + k[3][i]);
            }
        }

        void addVectorByScalar(std::vector<double>& result, std::vector<double>& vec, double scalar) {
            std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                           [scalar](double i1, double i2) { return i1 + i2 * scalar; });
        }

        void validateParameters() const { // добавить проверку на наличие функции-системы
            if (odeSystem) {
                throw std::invalid_argument("Function for system of equations is not set.");
            }

            if (yStart.empty()) {
                throw std::invalid_argument("Initial values for y are not set.");
            }

            if (std::isnan(xStep)) {
                throw std::invalid_argument("Value for xStep is not set.");
            }

            if (std::isnan(xLeft)) {
                throw std::invalid_argument("Value for xLeft is not set.");
            }

            if (std::isnan(xRight)) {
                throw std::invalid_argument("Value for xRight is not set.");
            }

            if (xLeft >= xRight) {
                throw std::invalid_argument("Invalid values for xLeft and xRight. xLeft must be less than xRight.");
            }

            if (xStep <= 0) {
                throw std::invalid_argument("Invalid value for xStep. It must be greater than 0.");
            }

            if (currentButcherTableau.empty()) {
                throw std::invalid_argument("Butcher table are not set.");
            }
        }


        std::vector<std::vector<double>> currentButcherTableau;
        std::vector<double> yStart, y;
        double xLeft = NAN;
        double xRight = NAN;
        double xStep = NAN;
        odeS odeSystem;

        std::unordered_map<ButcherTableau, std::vector<std::vector<double>>> butcherTables = {
                {
                        RungeKutta4, {
                                             {0.},
                                             {1. / 2., 1. / 2.},
                                             {1. / 2., 0., 1. / 2.},
                                             {1., 0., 0., 1.},
                                             {1. / 6., 1. / 3., 1. / 3., 1. / 6.}
                                     }
                },
                {
                        RosenbrockGilbert4, {
                                             {0.},
                                             {1. / 4., 1. / 4.},
                                             {1. / 4., 1. / 2., 1. / 4.},
                                             {1. / 2., 0., 1. / 2., 1. / 4.},
                                             {0., 1. / 6., 1. / 3., 1. / 3., 1. / 6.}
                                     }
                },
                {
                        DormanPrince8, {
                                             {0.},
                                             {1. / 5., 1. / 5.},
                                             {3. / 10., 3. / 40., 9. / 40.},
                                             {4. / 5., 44. / 45., -56. / 15., 32. / 9.},
                                             {8. / 9., 19372. / 6561., -31760. / 2187., 10448. / 6561., 0., 25360. / 2187.},
                                             {1., 9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176., -5103. / 18656., 0., 5. / 143.},
                                             {1. / 2., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.},
                                             {0., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.}
                                     }
                }
        };
    };
};