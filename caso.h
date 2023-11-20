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
        Heun2,
        Fehlberg6,
        BogackiShampine4
    };

    class ODE {
        using odeS = std::function<void(std::vector<double>& dydx, std::vector<double>& y, double x)>;
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
            if(butcherTablesMap.find(tableName) != butcherTablesMap.end()) {
                currentButcherTableau = butcherTablesMap[tableName];
            } else {
                throw std::invalid_argument("Invalid Butcher table name.");
            }
        }

        std::vector<double> rungeKutta(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKuttaIteration;
            return solveMethod(step);
        }

        std::vector<double> forwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::forwardEulerIteration;
            setButcherTableau(caso::RungeKutta4);
            return solveMethod(step);
        }

        std::vector<double> backwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::backwardEulerIteration;
            setButcherTableau(caso::RungeKutta4);
            return solveMethod(step);
        }

        std::vector<double> midpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::midpointMethodIteration;
            setButcherTableau(caso::RungeKutta4);
            return solveMethod(step);
        }

        std::vector<double> implicitMidpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::implicitMidpointMethodIteration;
            setButcherTableau(caso::RungeKutta4);
            return solveMethod(step);
        }
        // эйлер форвард алгоритм правильный
        // эйлер бэквардс алгоритм правильный
        // midpoint method алгоритм правильный
        // проверить hein's method 
        // проверить bogacki-shampine method
        // переделать дорман принс метод то есть переделать рунге кутту
        // implicit midpoint method алгоритм правильный



    private:
        std::vector<double> solveMethod(double step) {
            if(step > 0) {
                xStep = step;
            }

            validateParameters();
            currentY = yStart;

            while(xLeft < xRight) {
                std::cout << xLeft << " " << currentY[0] << std::endl;
                (this->*currentIterFunction)();
                
                xLeft += xStep;
            }

            return currentY;
        }

        void rungeKuttaIteration() {
            std::vector<double> temp(currentY.size());
            std::vector<std::vector<double>> k(7, std::vector<double>(currentY.size())); //currentButcherTableau.size() - 1
            computeK(k, temp);

            for(size_t i = 0; i < currentY.size(); i++) {
                currentY[i] += computeWeightedSum(k, i);
            }
        }

        void forwardEulerIteration() {
            std::vector<double> newY(currentY.size()), k1(currentY.size()), k2(currentY.size());
            odeSystem(k1, currentY, xLeft);
            addVectorByScalar(currentY, k1, xStep);
        }

        void backwardEulerIteration() {
            std::vector<double> k1(currentY.size()), temp(currentY.size()), temp1(currentY.size());

            odeSystem(temp, currentY, xLeft); // f(x, y)
            temp1 = currentY; // y
            addVectorByScalar(temp1, temp, xStep); // y + h * f(x + h, y)

            for(size_t i = 0; i < 100; i++) {
                odeSystem(temp, temp1, xLeft + xStep);
                temp1 = currentY;
                addVectorByScalar(temp1, temp, xStep);
            }

            odeSystem(k1, temp1, xLeft + xStep);
            addVectorByScalar(currentY, k1, xStep);
        }

        void midpointMethodIteration() {
            std::vector<double> k1(currentY.size()), k2(currentY.size()), temp(currentY.size());

            odeSystem(k1, currentY, xLeft);
            temp = currentY;
            addVectorByScalar(temp, k1, xStep / 2.);

            odeSystem(k2, temp, xLeft + xStep / 2.);
            addVectorByScalar(currentY, k2, xStep);
        }

        void implicitMidpointMethodIteration() {
            std::vector<double> k1(currentY.size()), temp(currentY.size()), temp1(currentY.size());

            odeSystem(temp, currentY, xLeft + xStep / 2.); // f(x, y)
            temp1 = currentY; // y
            addVectorByScalar(temp1, temp, xStep / 2.); // y + h * f(x + h, y)

            for(size_t i = 0; i < 100; i++) {
                odeSystem(temp, temp1, xLeft + xStep / 2.);
                temp1 = currentY;
                addVectorByScalar(temp1, temp, xStep / 2.);
            }

            odeSystem(k1, temp1, xLeft + xStep / 2.);
            addVectorByScalar(currentY, k1, xStep);
        }

        void computeK(std::vector<std::vector<double>>& kContainer, std::vector<double>& temp) {
            for(size_t i = 0; i < kContainer.size(); i++) {
                temp = currentY;
                for(size_t j = 1; j < currentButcherTableau[i].size(); j++) {
                    addVectorByScalar(temp, kContainer[j - 1], currentButcherTableau[i][j] * xStep);
                }
                odeSystem(kContainer[i], temp, xLeft + currentButcherTableau[i][0] * xStep);
            }
        }

        double computeWeightedSum(const std::vector<std::vector<double>>& k, size_t index) const {
            double sum = 0.0;
            for(size_t j = 0; j < k.size(); j++) {
                sum += currentButcherTableau.back()[j] * k[j][index];
            }
            return xStep * sum;
        }

        void addVectorByScalar(std::vector<double>& result, const std::vector<double>& vec, double scalar) {
            std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                           [scalar](double i1, double i2) { return i1 + i2 * scalar; });
        }

        void validateParameters() const {
            if(!odeSystem) {
                throw std::invalid_argument("Function for system of equations is not set.");
            }

            if(yStart.empty()) {
                throw std::invalid_argument("Initial values for y are not set.");
            }

            if(std::isnan(xStep)) {
                throw std::invalid_argument("Value for xStep is not set.");
            }

            if(std::isnan(xLeft)) {
                throw std::invalid_argument("Value for xLeft is not set.");
            }

            if(std::isnan(xRight)) {
                throw std::invalid_argument("Value for xRight is not set.");
            }

            if(xLeft >= xRight) {
                throw std::invalid_argument("Invalid values for xLeft and xRight. xLeft must be less than xRight.");
            }

            if(xStep <= 0) {
                throw std::invalid_argument("Invalid value for xStep. It must be greater than 0.");
            }

            if(currentButcherTableau.empty()) {
                throw std::invalid_argument("Butcher table are not set.");
            }
        }


        odeS odeSystem;
        IterFunction currentIterFunction = nullptr;
        std::vector<std::vector<double>> currentButcherTableau;
        std::vector<double> yStart, currentY;
        double xLeft = NAN;
        double xRight = NAN;
        double xStep = NAN;

        std::unordered_map<ButcherTableau, const std::vector<std::vector<double>>> butcherTablesMap = {
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
                        {35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.},
                        //{71. / 57600., 0., -71. / 16695., 71. / 1920., -17253. / 339200., 22. / 525., -1. / 40}
                }
            },
            {
                Heun2, {
                    {0.},
                    {2. / 3., 2. / 3.},
                        {1. / 4., 1. / 4.}
                }
            },
            {
                Fehlberg6, {
                    {0.},
                    {1. / 4., 1. / 4.},
                    {3. / 8., 3. / 32., 9. / 32.},
                    {12. / 13., 1932. / 2197., -7200. / 2197., 7296. / 2197.},
                    {1., 439. / 216., -8., 3680. / 513., -845. / 4104.},
                    {1. / 2.0, -8. / 27., 2., -3544. / 2565., 1859. / 4104., -11. / 40.},
                        {25. / 216., 0., 1408. / 2565., 2197. / 4104., -1. / 5., 0.},
                        //{-1. / 360., 0., 128. / 4275., 2197. / 75240., -1. / 50., -2.0 / 55.0}
                }
            },
            {
                BogackiShampine4, {
                    {0.},
                    {1. / 2., 1. / 2.},
                    {3. / 4., 0., 3. / 4.},
                    {1., 2. / 9., 1. / 3., 4. / 9.},
                        {2. / 9.0, 1. / 3., 4. / 9., 0.},
                        {-7. / 72., 1. / 12., 1. / 9., -1. / 8.}
                }
            }
        };
    };
};