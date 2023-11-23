#include <catch2/catch_test_macros.hpp>
#include "../caso.h"
#include <vector>
#include <cmath>

std::vector<double> dydx;
std::vector<double> y;

double y, xl, xr, xs, x;

class functions {
    void function1(std::vector<double>& dydx, std::vector<double>& y, double &x){
        dydx[0] = -3 * y[0]; 
    }
}

TEST_CASE("All tests passed", "[caso]") {
    SECTION()
}