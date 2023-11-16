#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

namespace crk4 {
    class ODE {
        using odeS = std::function<void(double x, std::vector<double>& y, std::vector<double>& dydx)>;

        public:
            ODE() {}
            ODE(const std::string& equation, char symbolOfVariable)
                : odeSystem(createSystem(equation)), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, double step, char symbolOfVariable) 
                : odeSystem(createSystem(equation)), xStep(step), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, std::vector<double> y0, double xLeft, double xRight, char symbolOfVariable) 
                : odeSystem(createSystem(equation)), y0(y0), xLeft(xLeft), xRight(xRight), symbol(symbolOfVariable) {}
            ODE(const std::string& equation, std::vector<double> y0, double xLeft, double xRight, double step, char symbolOfVariable) 
                : odeSystem(createSystem(equation)), y0(y0), xLeft(xLeft), xRight(xRight), xStep(step), symbol(symbolOfVariable) {}

            ODE(const odeS& odeSystem)
                : odeSystem(odeSystem) {}
            ODE(const odeS& odeSystem, double step)
                : odeSystem(odeSystem), xStep(step) {}
            ODE(const odeS& odeSystem, std::vector<double> y0, double xLeft, double xRight) 
                : odeSystem(odeSystem), y0(y0), xLeft(xLeft), xRight(xRight) {}
            ODE(const odeS& odeSystem, std::vector<double> y0, double xLeft, double xRight, double step) 
                : odeSystem(odeSystem), y0(y0), xLeft(xLeft), xRight(xRight), xStep(step) {}

            ~ODE(){}
            


            void setEquationWithString(const std::string& equation, const char symbolOfVariable) {
                symbol = symbolOfVariable;
                odeSystem = createSystem(equation);
            }
            
            void setEquationWithSystem(const odeS& inputOdeFunction) {
                odeSystem = inputOdeFunction;
            }

            std::vector<double> rungeKutta4(double xStep = 0.01) {
                y = y0;
                while (xLeft < xRight) {
                    rungeKutta4Iteration();
                    xLeft += xStep;
                }
                return y;
            }

            

            char symbol;
            double xLeft;
            double xRight;
            double xStep;
            std::vector<double> y0;

        protected:
            std::set<std::string> symbolsOfEquation;

        private:
            odeS createSystem(const std::string& expression) {
                std::string tempExpr = expression, equation = tempExpr;

                tempExpr.erase(std::remove_if(tempExpr.begin(), tempExpr.end(), ::isspace), tempExpr.end());
                findStartData(tempExpr, equation);
                
                return parseSystemFromString(equation);
            }

            void findStartData(std::string tempExpr, std::string& equation) {
                size_t foundComma = tempExpr.find(',');

                if (foundComma != std::string::npos) {
                    equation = tempExpr.substr(0, foundComma);
                    std::string startData = tempExpr.substr(foundComma + 1, tempExpr.length() - foundComma - 1);
                    getStartDataFromString(startData);
                }
            }

            void getStartDataFromString(std::string startData) {
                size_t pos = 0;
                                    
                if((startData.find(symbol, pos) == std::string::npos) && startData.length() > 0) {
                    throw std::invalid_argument("Invalid variable symbol");
                }

                while ((pos = startData.find(symbol, pos)) != std::string::npos) {
                    pos = startData.find(symbol, pos);
                    //обернуть тут каждую штуку в функцию
                    size_t openParenthesis = startData.find('(', pos);
                    size_t closePaParenthesis = startData.find(')', pos);
                    symbolsOfEquation.insert(startData.substr(pos, openParenthesis - pos));

                    try {
                        size_t equally = startData.find('=', openParenthesis);
                        size_t comma = startData.find(',', equally);
                        y0.push_back(std::stod(startData.substr(equally + 1, comma - equally - 1)));
                    } catch(const std::exception e) {
                        //std::cerr << e.what() << std::endl;
                    }
                    
                    try {
                        size_t openSqrBracket = startData.find('[', pos);
                        size_t closeSqrBracket = startData.find(']', openSqrBracket);
                        size_t semicolon = startData.find(';', openSqrBracket);
                        xLeft = std::stod(startData.substr(openSqrBracket + 1, semicolon - openSqrBracket - 1));
                        xRight = std::stod(startData.substr(semicolon + 1, closeSqrBracket - semicolon - 1));
                    } catch(const std::exception e) {
                        //std::cerr << e.what() << std::endl;
                    }

                    try {
                        size_t hSymbol = startData.find('h', pos) + 1;
                        xStep = std::stod(startData.substr(hSymbol + 1, startData.length() - hSymbol - 1));
                    } catch(const std::exception e) {
                        //std::cerr << e.what() << std::endl;
                    }

                    pos++;
                }
            }

            odeS parseSystemFromString(std::string equation) {
                return [equation](double x, std::vector<double>& y, std::vector<double>& dydx) -> void {
                    std::map<int, int> C; //степень производной - ключ, коэффициент - значение
                    int result = 0, derivate = 0, coefficient, end_iter=0;
                    for (int i = equation.length()-1; i >= 0; i--){
                        if (equation[i]=='='){ //нахождение правой части уравнения
                            std::string answer = equation.substr(i+1, equation.length()-1);
                            result = std::stod(answer);
                        }
                        if (equation[i] == '\''){
                            derivate++;
                        }
                        if (equation[i] == 'y'){
                            if (equation[i-1]== '+' || equation[i-1] == '-' || equation[i-1] == '/' || i == 0){
                                C[derivate] = 1;
                                derivate = 0;
                            }
                            else if (equation[i-1] == '*'){
                                end_iter = i-2;
                            }
                            else{
                                end_iter = i - 1;
                            }
                        }
                        if ((equation[i]== '+' || equation[i] == '-' || equation[i] == '/' || equation[i] == '*') && equation[i+1] != 'y'){
                            std::string coef_string = equation.substr(i + 1, end_iter - i);
                            coefficient = std::stod(coef_string);
                            C[derivate] = coefficient;
                            derivate = 0;
                            coefficient = 0;
                            end_iter = 0;
                        }
                    }
                    //dydx[y.size()-1] = -3 * y[2] - 3 * y[1] - y[0];

                    for (int i = 0; i < y.size()-1; i++){
                        dydx[i]=y[i+1];
                    }
                    int iterator = 0;
                    for (auto it = C.begin(); it != std::prev(C.end()); it++){
                        dydx[y.size()-1] += (-1)*y[iterator]*it->second;
                        iterator++;
                    }
                    auto itl = C.end();
                    dydx[C.size()-1]/=itl->second;
                    for (int i = 0; i < dydx.size(); i++){
                        //std::cout<<dydx[i]<<std::endl;
                    }
                };
            }

            void rungeKutta4Iteration() {
                    std::vector<double> temp(y.size());
                    std::vector<std::vector<double>> k(4, std::vector<double>(y.size()));

                    for (int i = 0; i < k.size(); i++) {
                        temp = y;
                        addVectorByScalar(temp, k[i], butcherTableau4[i][0] * xStep);
                        odeSystem(xLeft + butcherTableau4[i][0] * xStep, temp, k[i]);
                    }

                    for (int i = 0; i < y.size(); ++i) {
                        y[i] += (xStep / 6.0) * (k[0][i] + 2 * k[1][i] + 2 * k[2][i] + k[3][i]);
                    }
            }
            
            // void multiplyVectorByScalar(std::vector<double>& newVec, std::vector<double>& vec, double scalar) {
            //     std::transform(vec.begin(), vec.end(), newVec.begin(),
            //                 [scalar](double element) { return element * scalar; });
            // }

            void addVectorByScalar(std::vector<double>& result, std::vector<double>& vec, double scalar) {
                std::transform(result.begin(), result.end(), vec.begin(), result.begin(),
                            [scalar](double i1, double i2) { return i1 + i2 * scalar; });
            }

            odeS odeSystem; // функция или система уравнения
            std::vector<double> y; // вектор производных

            //Runge-Kutta
            const std::vector<std::vector<double>> butcherTableau4 = {
                    {0.},
                    {1. / 2., 1. / 2.},
                    {1. / 2., 0., 1. / 2.},
                    {1., 0., 0., 1.},
                    {1. / 6., 1. / 3., 1. / 3., 1. / 6.}
            };

            //Rosenbrock-Gilbert
            std::vector<std::vector<double>> butcherTableau4RG = {
                    {0.},
                    {1. / 4., 1. / 4.},
                    {1. / 4., 1. / 2., 1. / 4.},
                    {1. / 2., 0., 1. / 2., 1. / 4.},
                    {0., 1. / 6., 1. / 3., 1. / 3., 1. / 6.}
            };

            //Dorman-Prince
            const std::vector<std::vector<double>> ButcherTableau8DP = {
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