#include <iostream>
#include "crk4.h"

int main() {
    crk4::ODE a("a");
    return 0;
}


// #include <iostream>
// #include <cmath>

// // функция заданного уравнения
// double f(double x, double y) {
//     return 3 * std::pow(x, 2) * y + std::pow(x, 2) * std::exp(std::pow(x, 3));
// }

// // Реализация метода Рунге-Кутты 4-го порядка
// double rungeKutta(double x0, double y0, double h, double x_target) {
//     while (x0 < x_target) {
//         double k1 = h * f(x0, y0);
//         double k2 = h * f(x0 + h / 2, y0 + k1 / 2);
//         double k3 = h * f(x0 + h / 2, y0 + k2 / 2);
//         double k4 = h * f(x0 + h, y0 + k3);

//         y0 = y0 + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
//         x0 = x0 + h;
//     }

//     return y0;
// }

// int main() {
//     double x0 = 0.0;
//     double y0 = 0.0;
//     double h = 0.001;
//     double x_target = 1.0;

//     double result = rungeKutta(x0, y0, h, x_target);

//     std::cout << "При x = " << x_target << ", значение y = " << result << std::endl;

//     return 0;
// }

// #include <iostream>
// #include <cmath>

// // Функция, задающая систему уравнений
// void f(double x, const double y[], double result[]) {
//     result[0] = y[1];
//     result[1] = y[2];
//     result[2] = -3 * y[2] - 3 * y[1] - y[0];
// }

// // Реализация метода Рунге-Кутты 4-го порядка для системы уравнений
// void rungeKutta(double x0, double y0[], double h, double x_target) {
//     double k1[3], k2[3], k3[3], k4[3];

//     while (x0 < x_target) {
//         // Вычисление коэффициентов k1
//         f(x0, y0, k1);

//         // Вычисление коэффициентов k2
//         double y_temp[3];
//         for (int i = 0; i < 3; ++i) {
//             y_temp[i] = y0[i] + h / 2 * k1[i];
//         }
//         f(x0 + h / 2, y_temp, k2);

//         // Вычисление коэффициентов k3
//         for (int i = 0; i < 3; ++i) {
//             y_temp[i] = y0[i] + h / 2 * k2[i];
//         }
//         f(x0 + h / 2, y_temp, k3);

//         // Вычисление коэффициентов k4
//         for (int i = 0; i < 3; ++i) {
//             y_temp[i] = y0[i] + h * k3[i];
//         }
//         f(x0 + h, y_temp, k4);

//         // Обновление значений
//         for (int i = 0; i < 3; ++i) {
//             y0[i] = y0[i] + h * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6;
//         }

//         x0 = x0 + h;
//     }

//     std::cout << "При x = " << x_target << ", значения y, y', y'' = " << y0[0] << ", " << y0[1] << ", " << y0[2] << std::endl;
// }

// int main() {
//     double x0 = 0.0;
//     double y0[] = {-1.0, 2.0, 3.0};
//     double h = 0.01;
//     double x_target = 1.0;

//     rungeKutta(x0, y0, h, x_target);

//     return 0;
// }
