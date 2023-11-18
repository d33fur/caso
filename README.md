# 1. About caso (CAuchy SOlver)
This is header-only C++ library for solving Cauchy problem using numerical methods.

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

## 2.2 Set the Butcher tableau for your method
There are 3 Butcher tableaus:
```RungeKutta4```,
```RosenbrockGilbert4```,
```DormanPrince8```
```Cpp
myEquation.setButcherTableau(caso::RungeKutta4);
```
## 2.3 Call the solve() method
It will return std::vector<double> = { $$y^{(n)}, y^{(n-1)},..., y'', y'$$ }
```Cpp
std::vector<double> answer = myEquation.solve();
```
or you can set new step
```Cpp
std::vector<double> answer = myEquation.solve(0.01);
```
