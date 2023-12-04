//#include <catch2/catch_test_macros.hpp>
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

std::vector<double> dydx;
std::vector<double> y {2};

// std::string equation = "3x^2*y+x^2*e^(x^3)";
std::string equation = "-2xy";

double xl = 0.0, xr = 3.0, xs = 0.25, x = 0.0;

void returnValuesToDefault(){
    xl = 0.0;
    xr = 3.0;
    xs = 0.25;
    x = 0.0;
}

std::string getCurrentInput(std::string& equation, double &xl, double &xr, double &xs) {
    std::string currentValueReturn = "backward Euler method, dy/dx = " + equation + ", y(0) = 2, from " + std::to_string(xl) + " to " + std::to_string(xr) + ", h = " + std::to_string(xs);
    return currentValueReturn;
}

json getWolframData(){
    std::string wolframAlphaAppId = wolframApiKey;
    std::string apiUrl = "http://api.wolframalpha.com/v2/query";

    std::string input = getCurrentInput(equation, xl, xr, xs);

    std::string fullUrl = apiUrl + "?input=" + cpr::util::urlEncode(input) + "&format=plaintext&output=JSON&appid=" + wolframAlphaAppId;

    auto response = cpr::Get(cpr::Url{fullUrl});
    
    std::string answer = response.text;
    json returningData = json::parse(answer);

    return returningData;
}

double getWolframAnswer(std::string response){
    size_t answerEnd = 0, answerBegin = 0, pipeAmount = 0;
    double apiAsnwer = 0.0, degree = 0;

    for (int i = response.length(); i >= 0; i--){
        if (response[i] == '|'){
            pipeAmount++;

            if (pipeAmount == 2){
                answerEnd = i;
            }

            if (pipeAmount == 3){
                answerBegin = i + 1;
                break;
            }
        }
    }
    std::string apiAsnwerString = response.substr(answerBegin, answerEnd - answerBegin);

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

void function1(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = 3 * x * x * y[0] + x * x * std::pow(M_E, std::pow(x, 3));
}

void function2(std::vector<double>& dydx, std::vector<double>& y, double x) {
    dydx[0] = -2 * x * y[0];
}

/*TEST_CASE("All tests passed", "[caso]") {
    SECTION( "Equal left and right borders" ) {
        json data = getWolframData();
        std::string stepwiseResults = data["queryresult"]["pods"][2]["subpods"][0]["plaintext"];
        double comparableAnswer = getWolframAnswer(stepwiseResults);

        caso::ODE testObject(function2, y, xl, xr, xs);
        std::vector<double> answer = testObject.rungeKutta4();
    }
}*/

int main(){
    //xl = xr - 1; //test case

    json data = getWolframData();
    std::string stepwiseResults = data["queryresult"]["pods"][2]["subpods"][0]["plaintext"];
    double comparableAnswer = getWolframAnswer(stepwiseResults);

    caso::ODE testObject(function2, y, xl, xr, xs);
    std::vector<double> answer = testObject.backwardEuler();
    
    // std::cout << stepwiseResults <<std::endl;
    // std::cout << comparableAnswer <<std::endl;
    

    // for (auto i : answer) {
    //     if (round(comparableAnswer / 1e9) == round(i / 1e9)){
    //         std::cout<<"Good!"<<std::endl;
    //     }
    //     else{
    //         std::cout<<"Bad!"<<std::endl;
    //     }
    // }

    for (auto i : answer) {
        std::cout<<i<<std::endl;
    }
}