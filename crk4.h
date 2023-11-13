#pragma once

#include <iostream>
#include <functional>
#include <cmath>
#include <vector>

namespace crk {
    template <typename T, typename R>
    class ODE {
        public:
            //ODE() {};
            ODE(const std::string& equation) {functionHolder.odeFunction = createFunction(equation); }; 
                //odeFunction = parseEquation(equation); 
            // template <typename F>
            // void solve(const std::string& equation, double initialValue, double startTime, double endTime, double stepSize, F method) {
            //     // Здесь нужно реализовать парсер для equation и создать функцию, представляющую уравнение
            //     std::function<double(double, double)> odeFunction = parseEquation(equation);

            //     double t = startTime;
            //     double y = initialValue;

            //     while (t < endTime) {
            //         // Используем переданный численный метод (method) для решения уравнения
            //         y = method(odeFunction, t, y, stepSize);
            //         t += stepSize;
            //     }

            //     // Вывод результатов или их сохранение
            //     std::cout << "At t=" << endTime << ", y=" << y << std::endl;
            // }

            double arrayOfY[0];
            double xLeft;
            double xRight;
            double step;


        private:
            template <typename T, typename R>
            std::function<R(T)> createFunction(const std::string& expression) {return parseEquation(expression); }
            // template <typename T, typename R>
            // std::function<R(T)> createFunction() {
            //     return [](T x) -> R {
            //         // Ваш код здесь
            //         // Пример: сложение двух значений и преобразование результата к типу R
            //         return static_cast<R>(x);
            //     };
            // }
            //template <typename T, typename R>
            //std::function<R(T)> odeFunc;

            template <typename U, typename V>
            class FunctionHolder {
                public:
                    std::function<U(V)> odeFunction;
            };

            FunctionHolder<T, R> functionHolder;


            class Parser {
                public:
                    Parser() {};
                    Parser(const std::string& equation) {};

                    std::function<double(double, double)> parseEquation(const std::string& equation) {
                        // Лямбда-функция для вычисления значения выражения
                        auto lambda = [expression](T x) -> R {
                            // Пример: простая реализация, использующая std::stringstream
                            std::stringstream ss(expression);
                            T value;
                            ss >> value;

                            // Пример: вычисление значения, например, квадрата числа
                            return static_cast<R>(value * value);
                        };

                        return lambda;
                        return [](double x, double y) { return std::sin(x); };
                };
            }; 
    };
};


    //double derivative(const std::string variableName, const std::string equationString, int orderOfDerivative, double point) {return 0;};
    //double derivative(const std::string variableName, const std::string equationString, double point) {return 0;};
