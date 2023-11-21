# 1. About caso (CAuchy SOlver)
This is header-only C++ library for solving Cauchy problem using numerical methods.

There are 3 methods: ```Runge-Kutta```, ```Adams-Bashforth```, ```Euler```.
# 2. Usage
Let's take $$y''' + 3y'' + 3y' + y = 0, \quad y(0) = -1, \quad y'(0) = 2, \quad y''(0) = 3, \quad x \in [0, 1], \quad h = 0.01$$ Cauchy problem and solve it.

Add ```#include "caso.h"``` into your project.

## 2.1 
First of all you need to initialize an instance of the ODE class.
You can do this with function for system of equations:
```Cpp
void f(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0];
}
```
And here are a few ways to set initial conditions of Cauchy problem:
```Cpp
crk4::ODE myEquation(f, {-1, 2, 3}, 0, 1);
```
```Cpp
crk4::ODE myEquation(f, {-1, 2, 3}, 0, 1, 0.01);
```
```Cpp
crk4::ODE myEquation(f);
myEquation.setStartValuesAndBorders({-1, 2, 3}, 0, 1, 0.01);
```
```Cpp
crk4::ODE myEquation;
myEquation.setEquationWithSystem(f);
myEquation.setStartValuesAndBorders({-1, 2, 3}, 0, 1, 0.01);
```

## 2.2 Set the Butcher tableau for your method (if it is Runge-Kutta)
There are 3 Butcher tableaus for Runge-Kutta method:
```RungeKutta4```,
```DormanPrince8```,
```Heun2```,
```Fehlberg6```,
```BogackiShampine4```
```Cpp
myEquation.setButcherTableau(caso::RungeKutta4);
```

## 2.3 Call the method
It will return std::vector<double> = { $y^{(n)}, y^{(n-1)},..., y'', y'$ }
```Cpp
std::vector<double> answer = myEquation.rungeKutta();
```
```Cpp
std::vector<double> answer = myEquation.forwardEuler();
```
```Cpp
std::vector<double> answer = myEquation.backwardEuler();
```
```Cpp
std::vector<double> answer = myEquation.midpoint();
```
```Cpp
std::vector<double> answer = myEquation.implicitMidpoint();
```
you can also set a step if you didn't set it during initialization or want to try another one for a solution.
```Cpp
std::vector<double> answer = myEquation.rungeKutta(0.01);
```