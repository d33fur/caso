#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>

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
         * @brief Pointer to a function for numerical methods.
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
         * @param step Step for X
         */
        void setStartValuesAndBorders(std::vector<double> yStartValues, double inputXLeft, double inputXRight, double step) {
            yStart = yStartValues;
            xLeft = inputXLeft;
            xRight = inputXRight;
            xStep = step;
        }

        /**
         * @brief Solves ODE with Runge-Kutta 4-order method.
         * @param step Step for X
         */
        std::vector<double> rungeKutta4(double step = -1.0) {
            currentIterFunction = &caso::ODE::rungeKutta4Iteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Heun 2-order method.
         * @param step Step for X
         */
        std::vector<double> heun2(double step = -1.0) {
            currentIterFunction = &caso::ODE::heun2Iteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with forward Euler method.
         * @param step Step for X
         */
        std::vector<double> forwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::forwardEulerIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with Backward Euler method.
         * @param step Step for X
         */
        std::vector<double> backwardEuler(double step = -1.0) {
            currentIterFunction = &caso::ODE::backwardEulerIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with midpoint method.
         * @param step Step for X
         */
        std::vector<double> midpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::midpointIteration;
            return solveMethod(step);
        }

        /**
         * @brief Solves ODE with implicit midpoint method.
         * @param step Step for X
         */
        std::vector<double> implicitMidpoint(double step = -1.0) {
            currentIterFunction = &caso::ODE::implicitMidpointIteration;
            return solveMethod(step);
        }



    private:
        /**
         * @brief Calls validateParameters(). 
         * Starts a loop over the interval X and on every iteration Calls 
         * a method depending on currentIterFunction. 
         * Returns solution vector.
         * @param step Step for X
         */
        std::vector<double> solveMethod(double step) {
            if(step > 0) {
                xStep = step;
            }

            validateParameters();
            currentY = yStart;

            while(xLeft < xRight) {
                (this->*currentIterFunction)();
                xLeft += xStep;
            }
            return currentY;
        }

        /**
         * @brief Implementation of the Runge-Kutta 4-order algorithm iteration.
         */
        void rungeKutta4Iteration() {
            std::vector<std::vector<double>> k(4, std::vector<double>(currentY.size()));
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
         * @brief Implementation of the Heun 2-order algorithm iteration.
         */
        void heun2Iteration() {
            std::vector<std::vector<double>> k(2, std::vector<double>(currentY.size()));
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
        void midpointIteration() {
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
        void implicitMidpointIteration() {
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



        //! ODE system function.
        odeS odeSystem;
        //! Pointer to a current numerical method iteration.
        IterFunction currentIterFunction = nullptr;
        //! Start Y values.
        std::vector<double> yStart;
        //! Current Y values.
        std::vector<double> currentY;
        //! Left border of X interval.
        double xLeft = NAN;
        //! Right border of X interval.
        double xRight = NAN;
        //! Current step for X.
        double xStep = NAN;
    };
};
