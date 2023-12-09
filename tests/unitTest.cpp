#include <catch2/catch_test_macros.hpp>
#include "../caso.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cpr/cpr.h>
#include "nlohmann/json.hpp"
#include "env.cpp"
#include <cmath>

using json = nlohmann::json;

std::vector<double> answer;
std::vector<double> dydx;
std::vector<double> y {2};

// std::string equation = "3x^2*y+x^2*e^(x^3)";
// std::string equation = "2x+y";
std::string equation;
std::string method;

double xl = 0.0, xr = 3.0, xs = 0.25, x = 0.0, comparableAnswer = 0.0;

void returnValuesToDefault(){
    xl = 0.0;
    xr = 3.0;
    xs = 0.25;
    x = 0.0;
}

std::string getCurrentInput(std::string methodName, std::string& equation, double &xl, double &xr, double &xs) {
    std::string currentValueReturn = methodName + " method, dy/dx = " + equation + ", y(0) = 2, from " + std::to_string(xl) + " to " + std::to_string(xr) + ", h = " + std::to_string(xs);
    return currentValueReturn;
}

json getWolframData(std::string methodName){
    std::string wolframAlphaAppId = wolframApiKey;
    std::string apiUrl = "http://api.wolframalpha.com/v2/query";

    std::string input = getCurrentInput(methodName, equation, xl, xr, xs);

    std::string fullUrl = apiUrl + "?input=" + cpr::util::urlEncode(input) + "&format=plaintext&output=JSON&appid=" + wolframAlphaAppId;

    auto response = cpr::Get(cpr::Url{fullUrl});
    
    std::string answer = response.text;
    json returningData = json::parse(answer);

    return returningData;
}

double getWolframAnswer(std::string response){
    size_t prevIndex = 0, pipeAmount = 0, beginIndex = 0;
    double apiAsnwer = 0.0, degree = 0;
    std::string answerSubStr;
    std::string apiAsnwerString;

    for (int i = 0; i<response.length(); i++){
        if (response[i] == '\n'){
            prevIndex = i + 1;
        }
        if (i == response.length() - 2){
            answerSubStr = response.substr(prevIndex, response.length() - prevIndex);
        }
    }

    for (int i = 0; i < answerSubStr.length(); i++){
        if (answerSubStr[i] == '|'){
            pipeAmount++;

            if (pipeAmount == 2){
                beginIndex = i + 1;
            }
            if (pipeAmount == 3){
                apiAsnwerString = answerSubStr.substr(beginIndex, i - beginIndex);
            }
        }
    }

    size_t pos = apiAsnwerString.find('^');

    if (pos != std::string::npos) {
        degree = std::stod(apiAsnwerString.substr(pos+1, apiAsnwerString.length()-pos-2));
    }

    pos = apiAsnwerString.find("×");

    if (pos != std::string::npos) {
        apiAsnwerString.erase(pos, apiAsnwerString.length()-pos-1);
    }
    
    apiAsnwer = std::stod(apiAsnwerString) * pow(10, degree);
    return apiAsnwer;
}

// void function1(std::vector<double>& dydx, std::vector<double>& y, double x) {
//     dydx[0] = 3 * x * x * y[0] + x * x * std::pow(M_E, std::pow(x, 3));
// }

double getComparableAnswer(std::string methodName){
    json data = getWolframData(methodName);
    std::string stepwiseResults = data["queryresult"]["pods"][2]["subpods"][0]["plaintext"];
    return getWolframAnswer(stepwiseResults);
}


void function1(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = (-2) * x * y[0];
}

void function2(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = 2 * x + y[0];
}

void function3(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = std::pow(M_E, x) - y[0];
}

void function4(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = 2 * y[0];
}

void function5(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = (-1) * y[0] + x;
}

TEST_CASE("All tests passed", "[caso]") {
    SECTION( "Answer equality" ) {
        equation = "-2xy";
        method = "runge kutta";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectRungeKutta(function1, y, xl, xr, xs);
        answer = testObjectRungeKutta.rungeKutta4();
        
        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }

        equation = "2x+y";
        method = "backward euler";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectBackwardEuler(function2, y, xl, xr, xs);
        answer = testObjectBackwardEuler.backwardEuler();

        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }

        // equation = "exp(x)-y";
        // equation = "2y";
        // method = "Dormand Prince";
        // comparableAnswer = getComparableAnswer(method);

        // caso::ODE testObjectDormandPrince(function4, y, xl, xr, xs);
        // answer = testObjectDormandPrince.rungeKuttaDormandPrince8();

        // for (auto i : answer){
        //     REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        // }

        equation = "2y";
        method = "Heun";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectHeun(function4, y, xl, xr, xs);
        answer = testObjectHeun.heun2();

        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }


        equation = "-y+x";
        method = "Runge Kutta Fehlberg";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectRungeKuttaFehlberg(function5, y, xl, xr, xs);
        answer = testObjectRungeKuttaFehlberg.rungeKuttaFehlberg6();

        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }

        // equation = "-2xy";
        // method = "Bogacki Shampine";
        // comparableAnswer = getComparableAnswer(method);

        // caso::ODE testObjectBogackiShampine(function1, y, xl, xr, xs);
        // answer = testObjectBogackiShampine.rungeKuttaBogackiShampine4();

        // for (auto i : answer){
        //     REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        // }

        equation = "2x+y";
        method = "midpoint";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectMidpoint(function2, y, xl, xr, xs);
        answer = testObjectMidpoint.midpoint();

        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }

        equation = "2y";
        method = "implicit midpoint";
        comparableAnswer = getComparableAnswer(method);

        caso::ODE testObjectImplicitMidpoint(function4, y, xl, xr, xs);
        answer = testObjectImplicitMidpoint.implicitMidpoint();

        for (auto i : answer){
            REQUIRE(round(comparableAnswer * 1e9 / 1e9) == round(i * 1e9 / 1e9));
        }
    }
}

// int main(){
//     // xl = xr - 1; //test case

//     equation = "-2xy";
//     method = "runge kutta";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectRungeKutta(function1, y, xl, xr, xs);
//     answer = testObjectRungeKutta.rungeKutta4();

//     equation = "2x+y";
//     method = "backward euler";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectBackwardEuler(function2, y, xl, xr, xs);
//     answer = testObjectBackwardEuler.backwardEuler();

//     // equation = "exp(x)-y";
//     equation = "2y";
//     method = "Dormand Prince";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectDormandPrince(function4, y, xl, xr, xs);
//     answer = testObjectDormandPrince.rungeKuttaDormandPrince8();
    
//     // std::cout << data <<std::endl;
//     // std::cout << stepwiseResults <<std::endl;
//     // std::cout << comparableAnswer <<std::endl;
    
//     equation = "2y";
//     method = "Heun";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectHeun(function4, y, xl, xr, xs);
//     answer = testObjectHeun.heun2();

//     equation = "2x+y";
//     method = "midpoint";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectMidpoint(function2, y, xl, xr, xs);
//     answer = testObjectMidpoint.midpoint();

//     equation = "2y";
//     method = "implicit midpoint";
//     comparableAnswer = getComparableAnswer(method);

//     caso::ODE testObjectImplicitMidpoint(function4, y, xl, xr, xs);
//     answer = testObjectImplicitMidpoint.implicitMidpoint();

//     // equation = "-2xy";
//     // method = "Bogacki Shampine";
//     // comparableAnswer = getComparableAnswer(method);

//     // caso::ODE testObjectBogackiShampine(function1, y, xl, xr, xs);
//     // answer = testObjectBogackiShampine.rungeKuttaBogackiShampine4();
    
//     std::cout<<comparableAnswer * 1e9 / 1e9<<std::endl;
//     for (auto i : answer) {
//         std::cout<<i * 1e9 / 1e9<<std::endl;
//     }

//     // for (auto i : answer) {
//     //     if (comparableAnswer == i){
//     //         std::cout<<"Good!"<<std::endl;
//     //     }
//     //     else{
//     //         std::cout<<"Bad!"<<std::endl;
//     //     }
//     // }
// }