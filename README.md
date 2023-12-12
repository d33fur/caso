- [1. About caso](#1-about-caso)
- [2. Usage](#2-usage)
- 	- [2.1. Start](#21-start)
	- [2.2. Call the method](#23-call-the-method)
- [3. Documentation](#3-documentation)
# 1. About caso
caso (CAuchy SOlver) - this is header-only C++ library for solving Cauchy problem using numerical methods.

There are 6 methods: ```rungeKutta4```, ```heun2```, ```forwardEuler```, ```backwardEuler```, ```midpoint```, ```implicitMidpoint```.
# 2. Usage
Let's take $$y''' + 3y'' + 3y' + y - x = 0, \quad y(0) = -1, \quad y'(0) = 2, \quad y''(0) = 3, \quad x \in [0, 1], \quad h = 0.01$$ Cauchy problem and solve it.

Add ```#include "caso.h"``` into your project.

## 2.1 Start
First of all you need to initialize an instance of the ODE class.
You can do this with function for system of equations:
$$y' = y_2$$
$$y'' = y_3$$
$$y''' = -3 * y_3 - 3 * y_2 - y_1 + x$$
```Cpp
void f(double x, std::vector<double>& y, std::vector<double>& dydx) {
    dydx[0] = y[1];
    dydx[1] = y[2];
    dydx[2] = -3 * y[2] - 3 * y[1] - y[0] + x;
}
```
And here are a few ways to set initial conditions of Cauchy problem:

```caso::ODE myEquation(your system of equations, vector of starting Y values, left x border, right x border, step);```
```Cpp
caso::ODE myEquation(f, {-1, 2, 3}, 0, 1);
```
```Cpp
caso::ODE myEquation(f, {-1, 2, 3}, 0, 1, 0.01);
```
```Cpp
caso::ODE myEquation(f);
myEquation.setStartValuesAndBorders({-1, 2, 3}, 0, 1, 0.01);
```
```Cpp
caso::ODE myEquation;
myEquation.setEquationWithSystem(f);
myEquation.setStartValuesAndBorders({-1, 2, 3}, 0, 1, 0.01);
```

## 2.2 Call the method
It will return std::vector<double> = { $y^{(n)}, y^{(n-1)},..., y'', y'$ }
```Cpp
std::vector<double> answer = myEquation.rungeKutta4();
```

you can also set a step if you didn't set it during initialization or want to try another one for a solution.
```Cpp
std::vector<double> answer = myEquation.rungeKutta4(0.01);
```
Look at [1. About caso](#1-about-caso) for all methods.

# 3. Documentation
[Documentation](https://d33fur.github.io/caso/index.html)
