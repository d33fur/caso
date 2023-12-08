#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <numeric>

namespace caso {

    /**
     * @brief Class for solving ordinary differential equations.
     */
    class ODE {

        /**
         * @brief Type of ODE system of equations.
         * @param dydx Derivative variables
         * @param y Derivatives
         * @param x x-value
         */
        using odeS = std::function<void(std::vector<double>& dydx, std::vector<double>& y, double x)>;
        /**
         * @brief Function pointer for numerical methods.
         */
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

        /**
         * @brief Sets ODE system.
         * @param inputOdeSystem ODE system function
         */
        void setEquationWithSystem(const odeS& inputOdeSystem) {
            odeSystem = inputOdeSystem;
        }

        /**
         * @brief Sets start values for ODE.
         * @param yStartValues Start values of the derivatives
         * @param inputXLeft Left border of X interval
         * @param inputXRight Right border of X interval
         * @param step Step or h for X
         */
        void setStartValuesAndBorders(std::vector<double> yStartValues, double inputXLeft, double inputXRight, double step) {
            yStart = yStartValues;
            xLeft = inputXLeft;
            xRight = inputXRight;
            xStep = step;
        }

        /**
         * @brief Solves ODE with Runge-Kutta method.
         * @param step Step or h for X
         */
        std::vector<double> rungeKutta4(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKutta4Iteration;
            currentButcherTableau = butcherTablesMap[RungeKutta4];
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Runge-Kutta method.
         * @param step Step or h for X
         */
        std::vector<double> rungeKuttaDormandPrince8(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKuttaDormandPrince8Iteration;
            currentButcherTableau = butcherTablesMap[RungeKuttaDormandPrince8];
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Runge-Kutta method.
         * @param step Step or h for X
         */
        std::vector<double> heun2(double step = -1.0) {
            currentIterFunction = &caso::ODE::heun2Iteration;
            currentButcherTableau = butcherTablesMap[Heun2];
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Runge-Kutta method.
         * @param step Step or h for X
         */
        std::vector<double> rungeKuttaFehlberg6(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKuttaFehlberg6Iteration;
            currentButcherTableau = butcherTablesMap[RungeKuttaFehlberg6];
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Runge-Kutta method.
         * @param step Step or h for X
         */
        std::vector<double> rungeKuttaBogackiShampine4(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKuttaBogackiShampine4Iteration;
            currentButcherTableau = butcherTablesMap[RungeKuttaBogackiShampine4];
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with forward Euler method.
         * @param step Step or h for X
         */
        std::vector<double> forwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::forwardEulerIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Backward Euler method.
         * @param step Step or h for X
         */
        std::vector<double> backwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::backwardEulerIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with midpoint method.
         * @param step Step or h for X
         */
        std::vector<double> midpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::midpointMethodIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with implicit midpoint method.
         * @param step Step or h for X
         */
        std::vector<double> implicitMidpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::implicitMidpointMethodIteration;
            return solveMethod(step);
        }



    private:
        /**
         * @brief Causes fields to be checked. Starts a loop over the interval X and on every iteration Calls 
         * a method depending on currentIterFunction. Returns solution vector.
         * @param step Step or h for X
         */
        std::vector<double> solveMethod(double step) {
            if(step > 0) {
                xStep = step;
            }

            validateParameters();
            currentY = yStart;
            size_t n = 0;
            while(xLeft < xRight) {
                std::cout << "n : " << n << "   x : " << xLeft << "   y : " << currentY[0] << "   step : " << xStep << std::endl;
                // if(n%100 == 0)
                //     std::cout << "n : " << n << "   x : " << xLeft << "   y : " << currentY[0] << std::endl;
                (this->*currentIterFunction)();
                xLeft += xStep;
                n++;
            }
            std::cout << "n : " << n << "   x : " << xLeft << "   y : " << currentY[0] << "   step : " << xStep << std::endl;
            return currentY;
        }

        /**
         * @brief Implementation of the universal Runge-Kutta algorithm iteration. If necessary, calls the adaptive step function.
         */
        void rungeKutta4Iteration() {
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 1, std::vector<double>(currentY.size()));
            std::vector<double> temp = currentY;
            odeSystem(k[0], currentY, xLeft);
            addVectorByScalar(temp, k[0], xStep / 2.);
            odeSystem(k[1], temp, xLeft + xStep / 2.);
            temp = currentY;
            addVectorByScalar(temp, k[1], xStep / 2.);
            odeSystem(k[2], temp, xLeft + xStep / 2.);
            temp = currentY;
            addVectorByScalar(temp, k[2], xStep);
            odeSystem(k[3], temp, xLeft + xStep);

            std::fill(temp.begin(), temp.end(), 0);
            addVectorByScalar(temp, k[0], 1. / 6.);
            addVectorByScalar(temp, k[1], 1. / 3.);
            addVectorByScalar(temp, k[2], 1. / 3.);
            addVectorByScalar(temp, k[3], 1. / 6.);
            addVectorByScalar(currentY, temp, xStep);
        }

        /**
         * @brief Implementation of the universal Runge-Kutta algorithm iteration. If necessary, calls the adaptive step function.
         */
        void rungeKuttaDormandPrince8Iteration() {
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 1, std::vector<double>(currentY.size()));
            std::vector<double> temp(currentY.size());
            computeK(k);

            for(size_t i = 0; i < currentButcherTableau.back().size(); i++) {

                addVectorByScalar(temp, k[i], currentButcherTableau[currentButcherTableau.size() - 2][i]);
            }
            addVectorByScalar(currentY, temp, xStep);

            changeStep(k);
            // double midY = std::accumulate(temp.begin(), temp.end(), 0) / temp.size();
            // double midZ = 0;//std::accumulate(temp1.begin(), temp1.end(), 0) / temp1.size();
            // double error = std::abs(midY - midZ);
            // double eps = 1e-4;
            // xStep *= 0.9 * std::pow(((eps * xStep) / (2 * error)), 1. / 5); 
        }

        /**
         * @brief Implementation of the universal Runge-Kutta algorithm iteration. If necessary, calls the adaptive step function.
         */
        void heun2Iteration() {
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 1, std::vector<double>(currentY.size()));
            std::vector<double> temp = currentY;
            odeSystem(k[0], currentY, xLeft);
            addVectorByScalar(temp, k[0], xStep * 2. / 3.);
            odeSystem(k[1], temp, xLeft + xStep * 2. / 3.);

            std::fill(temp.begin(), temp.end(), 0);
            addVectorByScalar(temp, k[0], 1. / 4.);
            addVectorByScalar(temp, k[1], 3. / 4.);
            addVectorByScalar(currentY, temp, xStep);
        }

        /**
         * @brief Implementation of the universal Runge-Kutta algorithm iteration. If necessary, calls the adaptive step function.
         */
        void rungeKuttaFehlberg6Iteration() {
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 1, std::vector<double>(currentY.size()));
            std::vector<double> temp(currentY.size());
            computeK(k);

            for(size_t i = 0; i < currentButcherTableau.back().size(); i++) {

                addVectorByScalar(temp, k[i], currentButcherTableau[currentButcherTableau.size() - 2][i]);
            }
            addVectorByScalar(currentY, temp, xStep);

            changeStep(k);
        }

        /**
         * @brief Implementation of the universal Runge-Kutta algorithm iteration. If necessary, calls the adaptive step function.
         */
        void rungeKuttaBogackiShampine4Iteration() {
            std::vector<std::vector<double>> k(currentButcherTableau.size() - 2, std::vector<double>(currentY.size()));
            std::vector<double> temp = currentY, temp1 = currentY;
            odeSystem(k[0], currentY, xLeft);
            addVectorByScalar(temp, k[0], xStep / 2.);
            odeSystem(k[1], temp, xLeft + xStep / 2.);
            temp = currentY;
            addVectorByScalar(temp, k[1], xStep * 3. / 4.);
            odeSystem(k[2], temp, xLeft + xStep * 3. / 4.);

            temp = currentY;
            for(size_t i = 0; i < currentY.size(); i++) {
                temp[i] += 2. / 9. * xStep * k[0][i] + 1. / 3. * xStep * k[1][i] + 4. / 9. * xStep * k[2][i];
            }
            
            odeSystem(k[3], currentY, xLeft + xStep);

            temp1 = currentY;
            for(size_t i = 0; i < currentY.size(); i++) {
                temp1[i] += 7. / 24. * xStep * k[0][i] + 1. / 4. * xStep * k[1][i] + 1. / 3. * xStep * k[2][i] + 1. / 8. * xStep * k[3][i];
            }

            currentY = temp;
            //double error = computeError(k);

            changeStep(k);
            // double midY = std::accumulate(temp.begin(), temp.end(), 0) / temp.size();
            // double midZ = std::accumulate(temp1.begin(), temp1.end(), 0) / temp1.size();
            // double error = std::abs(midY - midZ);
            // const double tolerance = 1e-8;
            // //double error = std::abs(computeError(k));
            // //xStep = std::pow((tolerance * xStep / (2 * error)), 1. / (currentButcherTableau.back().size() - 2));
            // xStep *= 0.9 * std::min(std::max(std::pow(tolerance / (2. * error), 1. / (currentButcherTableau.back().size() - 2)), 0.3), 2.0);
            // if(error < tolerance) {
            //     currentY[0] += error;
            //     //xStep *= 0.9 * std::min(std::max(std::pow(tolerance * xStep / (2. * error), 1. / (currentButcherTableau.back().size() - 2)), 0.3), 2.0);
            //     //xStep--;
            // } 
            // double eps = 1e-5;
            // xStep *= std::pow(((eps * xStep) / (2 * error)), 1. / 5); 


            // double aTol = 1e-100;
            // double rTol = 1e-100;
            // double tolh = aTol + rTol * std::max(std::abs(midY), std::abs(midZ));
            // double En = std::abs(error / tolh);
            
            // if(std::abs(midY - midZ) < 1e-7) {
            //     xStep += xStep;
            // } else {
            //     xStep *= 0.5;
            // }



            //double err = doStep(xStep, derivFuncs, nDepVars, vars[0], &vars[2], 2, &vars[3], 2);
            //newStep1 = xStep * k_reduc * std::pow(tolerance/err, 1.0/((double)k_porder+1.0));
            //newStep2 = xStep * 10;
            //xStep = newstep1<newstep2 ? newStep1 : newStep2;
        }

        /**
         * @brief Implementation of the forward Euler algorithm iteration.
         */
        void forwardEulerIteration() {
            std::vector<double> newY(currentY.size()), k1(currentY.size()), k2(currentY.size());
            odeSystem(k1, currentY, xLeft);
            addVectorByScalar(currentY, k1, xStep);
        }

        /**
         * @brief Implementation of the backward Euler algorithm iteration.
         */
        void backwardEulerIteration() {
            std::vector<double> k1(currentY.size()), temp(currentY.size()), temp1(currentY.size());

            odeSystem(temp, currentY, xLeft);
            temp1 = currentY;
            addVectorByScalar(temp1, temp, xStep);

            const double tolerance = 1e-8;

            for(size_t i = 0; i < 100 && !checkConvergence(currentY, temp1, tolerance); i++) {
                odeSystem(temp, temp1, xLeft + xStep);
                temp1 = currentY;
                addVectorByScalar(temp1, temp, xStep);
            }

            odeSystem(k1, temp1, xLeft + xStep);
            addVectorByScalar(currentY, k1, xStep);
        }

        /**
         * @brief Implementation of the midpoint algorithm iteration.
         */
        void midpointMethodIteration() {
            std::vector<double> k1(currentY.size()), k2(currentY.size()), temp(currentY.size());

            odeSystem(k1, currentY, xLeft);
            temp = currentY;
            addVectorByScalar(temp, k1, xStep / 2.);

            odeSystem(k2, temp, xLeft + xStep / 2.);
            addVectorByScalar(currentY, k2, xStep);
        }

        /**
         * @brief Implementation of the implicit midpoint algorithm iteration.
         */
        void implicitMidpointMethodIteration() {
            std::vector<double> k1(currentY.size()), temp(currentY.size()), temp1(currentY.size());

            odeSystem(temp, currentY, xLeft + xStep / 2.);
            temp1 = currentY;
            addVectorByScalar(temp1, temp, xStep / 2.);

            const double tolerance = 1e-8;

            for(size_t i = 0; i < 100 && !checkConvergence(currentY, temp1, tolerance); i++) {
                odeSystem(temp, temp1, xLeft + xStep / 2.);
                temp1 = currentY;
                addVectorByScalar(temp1, temp, xStep / 2.);
            }

            odeSystem(k1, temp1, xLeft + xStep / 2.);
            addVectorByScalar(currentY, k1, xStep);
        }

        /**
         * @brief Calculates a new step value based on the magnitude of the error value.
         * @param k Matrix of coefficients
         */
        void changeStep(const std::vector<std::vector<double>>& k) {
            const double tolerance = 1e-4;
            double error = std::abs(computeError(k));
            xStep *= 0.9 * std::pow((tolerance * xStep / (2 * error)), 1. / (currentButcherTableau.back().size() - 2));

            //xStep *= 0.9 * std::min(std::max(std::pow(tolerance * xStep / (2. * error), 1. / (currentButcherTableau.back().size() - 2)), 0.3), 2.0);
            //xStep--;
            // if(error < tolerance) {
                
            //     xStep = 0.9 * std::min(std::max(std::pow(tolerance * xStep / (2. * error), 1. / (currentButcherTableau.back().size() - 2)), 0.3), 2.0);
            //     xStep--;
            // } 
            // else {
            //     xStep *= 2.0;
            // }
        }

        /**
         * @brief Calculates the error value using the last 2 lines of the Butcher tableau.
         * @param k Matrix of coefficients
         */
        double computeError(const std::vector<std::vector<double>>& k) {
            double error = 0.0;
            size_t n = currentButcherTableau.back().size();
            for(size_t i = 0; i < n; i++) {
                error += (currentButcherTableau[n-2][i] - currentButcherTableau[n-1][i]) * std::accumulate(k[i].begin(), k[i].end(), 0) / k[i].size();
            }
            return xStep * error;
        }

        /**
         * @brief Checks convergence using previous and current Y values.
         * @param previous Previous Y values
         * @param current Current Y values
         * @param tolerance Calculation accuracy
         */
        bool checkConvergence(const std::vector<double>& previous, const std::vector<double>& current, double tolerance) const {
            for(size_t i = 0; i < current.size(); ++i) {
                if(std::abs(current[i] - previous[i]) > tolerance) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Computes coefficients of Runge-Kutta method using currentButcherTableau.
         * @param k Matrix of coefficients
         */
        void computeK(std::vector<std::vector<double>>& k) {
            std::vector<double> temp;
            for(size_t i = 0; i < k.size(); i++) {
                temp = currentY;
                for(size_t j = 1; j < currentButcherTableau[i].size(); j++) {
                    addVectorByScalar(temp, k[j - 1], currentButcherTableau[i][j] * xStep);
                }
                odeSystem(k[i], temp, xLeft + currentButcherTableau[i][0] * xStep);
            }
        }

        /**
         * @brief Checks convergence using previous and current Y values.
         * @param result Vector to which vec will be added element-wise after multiplication
         * @param vec A vector that will be multiplied by a scalar
         * @param scalar Multiplication factor
         */
        void addVectorByScalar(std::vector<double>& result, const std::vector<double>& vec, double scalar) {
            std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                           [scalar](double i1, double i2) { return i1 + i2 * scalar; });
        }

        /**
         * @brief Causes fields to be checked.
         */
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
        }

        /**
         * @brief Enum type Butcher Tableaus for Runge-Kutta method.
         */
        enum ButcherTableau {
            RungeKutta4,
            RungeKuttaDormandPrince8,
            Heun2,
            RungeKuttaFehlberg6,
            RungeKuttaBogackiShampine4
        };

        //! ODE system function.
        odeS odeSystem;
        //! Pointer to a current numerical method iteration.
        IterFunction currentIterFunction = nullptr;
        //! Current Butcher tableau.
        std::vector<std::vector<double>> currentButcherTableau;
        //! Start Y values.
        std::vector<double> yStart;
        //! Final Y values.
        std::vector<double> currentY;
        //! Left border of X interval and current X.
        double xLeft = NAN;
        //! Right border of X interval.
        double xRight = NAN;
        //! Current step or h for X.
        double xStep = NAN;

        //! Map with Butcher tableaus.
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
                RungeKuttaDormandPrince8, {
                    {0.},
                    {1. / 5., 1. / 5.},
                    {3. / 10., 3. / 40., 9. / 40.},
                    {4. / 5., 44. / 45., -56. / 15., 32. / 9.},
                    {8. / 9., 19372. / 6561., -31760. / 2187., 10448. / 6561., 0., 25360. / 2187.},
                    {1., 9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176., -5103. / 18656., 0., 5. / 143.},
                    {1. / 2., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.},
                        {35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.},
                        //{71. / 57600., 0., -71. / 16695., 71. / 1920., -17253. / 339200., 22. / 525., -1. / 40}
                        {5179. / 57600., 0., 7571. / 16695., 396. / 640., -92097. / 339200., 187. / 2100., 1. / 40}
                }
            },
            {
                Heun2, {
                    {0.},
                    {2. / 3., 2. / 3.},
                        {1. / 4., 3. / 4.}
                }
            },
            {
                RungeKuttaFehlberg6, {
                    {0.},
                    {1. / 4., 1. / 4.},
                    {3. / 8., 3. / 32., 9. / 32.},
                    {12. / 13., 1932. / 2197., -7200. / 2197., 7296. / 2197.},
                    {1., 439. / 216., -8., 3680. / 513., -845. / 4104.},
                    {1. / 2., -8. / 27., 2., -3544. / 2565., 1859. / 4104., -11. / 40.},
                        // {25. / 216., 0., 1408. / 2565., 2197. / 4104., -1. / 5., 0.},
                        // {-1. / 360., 0., 128. / 4275., 2197. / 75240., -1. / 50., -2. / 55.}
                        {16. / 135., 0., 6656. / 12825., 2197. / 56430., -9. / 50., 2. / 55.},
                        {25. / 216., 0., 1408. / 2565., 2197. / 4104., -1. / 50., 0.}
                }
            },
            {
                RungeKuttaBogackiShampine4, {
                    {0.},
                    {1. / 2., 1. / 2.},
                    {3. / 4., 0., 3. / 4.},
                    {1., 2. / 9., 1. / 3., 4. / 9.},
                        {2. / 9., 1. / 3., 4. / 9., 0.},
                        //{-5. / 72., 1. / 12., 1. / 9., -1. / 8.}
                        {7. / 24., 1. / 4., 1. / 3., 1. / 8.}
                }
            }
        };
    };
};
