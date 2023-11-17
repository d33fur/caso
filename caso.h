#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
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
            ODE(const std::string& equation, char symbolOfVariable)
                : odeSystem(createSystem(equation)), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, char symbolOfVariable, double step) 
                : odeSystem(createSystem(equation)), xStep(step), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, char symbolOfVariable, std::vector<double> yStartValues, double xLeft, double xRight) 
                : odeSystem(createSystem(equation)), yStart(yStartValues), xLeft(xLeft), xRight(xRight), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, char symbolOfVariable, std::vector<double> yStartValues, double xLeft, double xRight, double step) 
                : odeSystem(createSystem(equation)), yStart(yStartValues), xLeft(xLeft), xRight(xRight), xStep(step), symbol(symbolOfVariable) {}

            ODE(const odeS& odeSystem)
                : odeSystem(odeSystem) {}
            ODE(const odeS& odeSystem, std::vector<double> yStartValues, double xLeft, double xRight) 
                : odeSystem(odeSystem), yStart(yStartValues), xLeft(xLeft), xRight(xRight) {}
            ODE(const odeS& odeSystem, std::vector<double> yStartValues, double xLeft, double xRight, double step) 
                : odeSystem(odeSystem), yStart(yStartValues), xLeft(xLeft), xRight(xRight), xStep(step) {}

            ~ODE() = default;
            


            void setEquationWithString(const std::string& equation, const char symbolOfVariable) {
                symbol = symbolOfVariable;
                odeSystem = createSystem(equation);
            }
            
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
            
            

        protected:
            std::set<std::string> symbolsOfEquation;
            std::vector<std::vector<double>> currentButcherTableau;
            std::vector<double> yStart;
            char symbol;
            double xLeft = NAN;
            double xRight = NAN;
            double xStep = NAN;



        private:
            odeS createSystem(const std::string& expression) {
                std::string tempExpr = expression, equation = tempExpr;

                tempExpr.erase(std::remove_if(tempExpr.begin(), tempExpr.end(), ::isspace), tempExpr.end());
                findStartData(tempExpr, equation);
                
                return parseSystemFromString(equation);
            }

            void findStartData(std::string tempExpr, std::string& equation) {
                size_t foundComma = tempExpr.find(',');
                if((tempExpr.substr(0, foundComma).find(symbol, 0) == std::string::npos) && tempExpr.length() > 0) {
                    throw std::invalid_argument("Invalid variable symbol.");
                }

                if (foundComma != std::string::npos) {
                    equation = tempExpr.substr(0, foundComma);
                    std::string startData = tempExpr.substr(foundComma + 1, tempExpr.length() - foundComma - 1);
                    getStartDataFromString(startData);
                }
            }

            void getStartDataFromString(std::string startData) {
                size_t i = 0;

                while ((i = startData.find(symbol, i)) != std::string::npos) {
                    i = startData.find(symbol, i);
                    iterationStartData(i, startData);
                    i++;
                }
            }

            void iterationStartData(size_t& i, std::string& startData) {
                //обернуть тут каждую штуку в функцию
                size_t openParenthesis = startData.find('(', i);
                size_t closePaParenthesis = startData.find(')', i);
                symbolsOfEquation.insert(startData.substr(i, openParenthesis - i));

                try {
                    getYStart(openParenthesis, startData);
                } catch(const std::exception e) {
                    //std::cerr << e.what() << std::endl;
                }
                
                try {
                    getXLeftXRight(i, startData);
                } catch(const std::exception e) {
                    //std::cerr << e.what() << std::endl;
                }

                try {
                    getXStep(i, startData);
                } catch(const std::exception e) {
                    //std::cerr << e.what() << std::endl;
                }
            }

            void getYStart(size_t& openParenthesis, std::string& startData) {
                    size_t equally = startData.find('=', openParenthesis);
                    size_t comma = startData.find(',', equally);
                    yStart.push_back(std::stod(startData.substr(equally + 1, comma - equally - 1)));
            }

            void getXLeftXRight(size_t& i, std::string& startData) {
                    size_t openSqrBracket = startData.find('[', i);
                    size_t closeSqrBracket = startData.find(']', openSqrBracket);
                    size_t semicolon = startData.find(';', openSqrBracket);
                    xLeft = std::stod(startData.substr(openSqrBracket + 1, semicolon - openSqrBracket - 1));
                    xRight = std::stod(startData.substr(semicolon + 1, closeSqrBracket - semicolon - 1));
            }

            void getXStep(size_t& i, std::string& startData) {
                    size_t hSymbol = startData.find('h', i) + 1;
                    xStep = std::stod(startData.substr(hSymbol + 1, startData.length() - hSymbol - 1));
            }

            odeS parseSystemFromString(std::string equation) {
                return [equation](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {
                    std::map<size_t, int64_t> C; //степень производной - ключ, коэффициент - значение
                    int64_t result = 0, coefficient;
                    size_t derivate = 0, endIter = 0;
                    for (size_t i = equation.length()-1; i >= 0; i--){
                        if (equation[i] == '='){ //нахождение правой части уравнения
                            std::string answer = equation.substr(i + 1, equation.length() - 1);
                            result = std::stod(answer);
                        }
                        if (equation[i] == '\''){
                            derivate++;
                        }
                        if (equation[i] == 'y'){
                            if (equation[i - 1] == '+' || equation[i - 1] == '-' || equation[i - 1] == '/' || i == 0){
                                C[derivate] = 1;
                                derivate = 0;
                            } else if (equation[i-1] == '*'){
                                endIter = i - 2;
                            } else{
                                endIter = i - 1;
                            }
                        }
                        std::vector<char> operators = {'+', '-', '/', '*'};
                        if ((std::find(operators.begin(), operators.end(), equation[i]) != operators.end()) && equation[i + 1] != 'y'){
                            std::string coefString = equation.substr(i + 1, endIter - i);
                            coefficient = std::stod(coefString);
                            C[derivate] = coefficient;
                            derivate = 0;
                            coefficient = 0;
                            endIter = 0;
                        }
                    }
                    for (size_t i = 0; i < y.size()-1; i++){
                        dydx[i]=y[i + 1];
                    }
                    size_t iter = 0;
                    for (auto it = C.begin(); it != std::prev(C.end()); it++){
                        dydx[y.size()-1] += (-1)*y[iter]*it->second;
                        iter++;
                    }
                    auto itl = C.end();
                    dydx[C.size() - 1]/=itl->second;
                };
            }

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

            void validateParameters() const {
                if (yStart.empty()) {
                    throw std::invalid_argument("Initial values for y are not set.");
                }

                if (symbol == '\0') {
                    throw std::invalid_argument("Symbol of the variable is not set.");
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



            odeS odeSystem; // функция или система уравнения
            std::vector<double> y; // вектор производных

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