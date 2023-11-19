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
        DormanPrince8,
    };

    class ODE {
        using odeS = std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)>;
        using IterFunction = void (caso::ODE::*)();

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
            if (butcherTablesMap.find(tableName) != butcherTablesMap.end()) {
                currentButcherTableau = butcherTablesMap[tableName];
            } else {
                throw std::invalid_argument("Invalid Butcher table name.");
            }
        }

        std::vector<double> rungeKutta(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKuttaIteration;
            return solveMethod(step);
        }

        std::vector<double> euler(double step = -1.0) {
            currentIterFunction = &caso::ODE::eulerIteration;
            setButcherTableau(caso::RungeKutta4);
            return solveMethod(step);
        }
        // добавить эйлера бэквардс и эйлера форвард, карч проверить метод эйлера который щас тут и к как он полностью называется
        // hein's method 
        // midpoint method
        // bogacki-shampine method
        // переделать дорман принс метод то есть переделать рунге кутту
        // implicit midpoint method



    private:
        std::vector<double> solveMethod(double step) {
            if (step > 0) {
                xStep = step;
            }

            validateParameters();
            y = yStart;

            while (xLeft < xRight) {
                (this->*currentIterFunction)();
                xLeft += xStep;
            }

            return y;
        }

        void rungeKuttaIteration() {
            std::vector<double> temp(y.size());
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 1, std::vector<double>(y.size()));
            computeK(k, temp);

            for (size_t i = 0; i < y.size(); i++) {
                y[i] += computeWeightedSum(k, i);
            }
        }

        void eulerIteration() {
            std::vector<double> temp(y.size());
            odeSystem(xLeft, y, temp);

            for (size_t i = 0; i < y.size(); i++) {
                y[i] += xStep * temp[i];
            }
        }

        // void computePreviousY(std::vector<std::vector<double>>& prevY, std::vector<double>& temp) {
        //     for (size_t i = 0; i < prevY.size(); i++) {
        //         temp = y;
        //         addVectorByScalar(temp, prevY[i], currentButcherTableau[i][0] * xStep);
        //         computeK(prevY, temp);
        //     }
        // }

        void computeK(std::vector<std::vector<double>>& kContainer, std::vector<double>& temp) {
            for (size_t i = 0; i < kContainer.size(); i++) {
                temp = y;
                addVectorByScalar(temp, kContainer[i], currentButcherTableau[i][0] * xStep);
                odeSystem(xLeft + currentButcherTableau[i][0] * xStep, temp, kContainer[i]);
            }
        }

        double computeWeightedSum(const std::vector<std::vector<double>>& k, size_t index) const {
            double sum = 0.0;
            for (size_t j = 0; j < k.size(); j++) {
                sum += currentButcherTableau.back()[j] * k[j][index];
            }
            return xStep * sum;
        }

        void addVectorByScalar(std::vector<double>& result, const std::vector<double>& vec, double scalar) {
            std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                           [scalar](double i1, double i2) { return i1 + i2 * scalar; });
        }

        void validateParameters() const {
            if (!odeSystem) {
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


        odeS odeSystem;
        IterFunction currentIterFunction = nullptr;
        std::vector<std::vector<double>> currentButcherTableau;
        std::vector<double> yStart, y;
        double xLeft = NAN;
        double xRight = NAN;
        double xStep = NAN;

        std::unordered_map<ButcherTableau, std::vector<std::vector<double>>> butcherTablesMap = {
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