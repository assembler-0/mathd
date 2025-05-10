#pragma once 
//#include "../include/CLI11.hpp"
#include "exprtk.hpp"
#include "termcolor.hpp"
#include <iomanip>
#include <cmath>            //headers, includes
#include <iostream>
//#include <vector>
//#include <algorithm>
using namespace std;
using namespace termcolor;
const string version ("1.0 - CMake");
string expressionStr; //version
signed int precision;
double maxY, minY;
char mode, func_sci;
double a , b, n;
char option_advanced;
double pi (3.1415926535);
double e (2.7182818284);
double exprtk_cbrt(double x){
    return cbrt(x);
}
void factorial(void) {
    cout << bright_blue << bold << "Enter value: " << reset;
    cin >> n;
    long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    cout << red << underline << bold << fixed << setprecision(0) << "Returned value: " << result << reset << '\n';
    return;
}
double gcd_lcm(double a, double b){
    while (b != 0) {
        double temp = b;
        b = std::fmod(a, b);
        a = temp;
    }
    return a;
}
void gcd(void) {
    cout << bright_blue << bold << "Please enter a: " << reset;
    cin >> a;
    cout << bright_blue << bold << "Please enter b: " << reset;
    cin >> b;
    while (b != 0) {
        double temp = b;
        b = std::fmod(a, b);
        a = temp;
    }
    cout << red << bold << underline << "Returned value: " << a << reset;
    return;
}
void lcm(void){
    cout << bright_blue << bold << "Please enter a: " << reset;
    cin >> a;
    cout << bright_blue << bold << "Please enter b: " << reset;
    cin >> b;
    cout << red << bold << underline << "Returned value: " <<  std::abs(a * b) / gcd_lcm(a, b);
    return;
}
double evaluateExpression_graph(const std::string& expressionStr, double x) {
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x);
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);
    exprtk::parser<double> parser;
    if (!parser.compile(expressionStr, expression)) {
        cerr << "Error parsing expression: " << parser.error() << endl;
        return NAN;
    }

    return expression.value();
}
void cMaxMin(const string& expressionStr, double xMin, double xMax, double& maxY, double& minY) {
    const double step = 0.001; // Small step for accurate results
    maxY = -INFINITY;
    minY = INFINITY;

    for (double x = xMin; x <= xMax; x += step) {
        double y = evaluateExpression_graph(expressionStr, x);
        if (!std::isnan(y)) {
            if (y > maxY) maxY = y;
            if (y < minY) minY = y;
        }
    }
}

void plotGraph(const string& expressionStr, int width, int height) {
    std::vector<string> canvas(height, string(width, ' '));

    const double xMin = -10.0, xMax = 10.0; // Range of x
    const double yMin = -1.5, yMax = 1.5;   // Range of y

    const double xStep = (xMax - xMin) / (width*precision);
    const double yStep = (yMax - yMin) / (height*precision);

    // Draw axes
    int xAxis = height / 2;
    int yAxis = width / 2;
    for (int x = 0; x < width; ++x) canvas[xAxis][x] = '-';
    for (int y = 0; y < height; ++y) canvas[y][yAxis] = '|';
    canvas[xAxis][yAxis] = '+'; // Origin

    // Plot points
    for (double x = xMin; x <= xMax; x += xStep) {
        double y = evaluateExpression_graph(expressionStr, x);
        if (!std::isnan(y)) {
            int plotX = static_cast<int>((x - xMin) * (width - 1) / (xMax - xMin));
            int plotY = static_cast<int>((yMax - y) * (height - 1) / (yMax - yMin));
            if (plotX >= 0 && plotX < width && plotY >= 0 && plotY < height) {
                canvas[plotY][plotX] = '*';
            }
        }
    }

    // Print the canvas
    for (const auto& row : canvas) {
        cout << bold << bright_green << row << endl << reset;
    }
}
void product(void){
    double mul (1);
    double x_value (1);
    cout << blue << bold << "Please enter function: " << reset;
    cin >> expressionStr;
    cout << blue << bold << "x goes from (do not enter 0): " << reset;
    cin >> a;
    cout << blue << bold << "to: " << reset;
    cin >> b;
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x_value); 
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);
    exprtk::parser<double> parser;
    if (!parser.compile(expressionStr, expression)) {
        std::cerr << "Error parsing the expression!" << std::endl;
        return;
    }
    for (int i = a; i <= b; ++i) {
        x_value = i;  // Set the value of x
        mul *= expression.value();  // Evaluate the expression
    }
     cout << red << setprecision(0) << fixed << bold << underline << "Product of f(x) from " << a << " to " << b << " is: " << mul << '\n' << reset;
    return;
}
void sigma_s(void){
    double sum (0);
    double x_value (0);
    cout << blue << bold << "Please enter function: " << reset;
    cin >> expressionStr;
    cout << blue << bold << "x goes from : " << reset;
    cin >> a;
    cout << blue << bold << "to: " << reset;
    cin >> b;
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x_value); 
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);
    exprtk::parser<double> parser;
    if (!parser.compile(expressionStr, expression)) {
        std::cerr << "Error parsing the expression!" << std::endl;
        return;
    }
    for (int i = a; i <= b; ++i) {
        x_value = i;  // Set the value of x
        sum += expression.value();  // Evaluate the expression
    }
    cout << red << bold << fixed << setprecision(0) << underline << "Sum of f(x) from " << a << " to " << b << " is: " << sum << '\n' << reset;
    return;
}
void scientific(void){
    cout << bold << green << string(55, '-') << reset << '\n'; 
    cout<< setw(0) <<bold << bright_green << "Trigonometric" << reset
        << setw(14) << bold << bright_green << "Hyperbolic" << reset
        << setw(21) << bold << bright_green << "Power" << reset << '\n';
    cout << bold << bright_green << string(55, '-') << reset << '\n';
    cout << setw(3) << bold << bright_green << "cos()"<< setw(25) << "cosh" << setw(26) << "pow()\n" << reset;
    cout << setw(3) << bold << bright_green << "sin()"<< setw(25) << "sinh" << setw(26) <<"sqrt()\n" << reset;
    cout << setw(3) << bold << bright_green << "tan()"<< setw(25) << "tanh" << setw(26) <<"cbrt()\n" << reset;
    cout << setw(3) << bold << bright_green << "acos()"<< setw(27) << "acosh()\n"<< reset;
    cout << setw(3) << bold << bright_green << "asin()" << setw(27) <<"asinh()\n"<< reset;
    cout << setw(3) << bold << bright_green << "atan()"<<setw(27)<<"atanh()\n"<< reset;
    cout << bold << green << string(55, '-') << reset << '\n';
    cout<< setw(0) <<bold << bright_green << "Exp/Log" << reset
        << setw(22) << bold << bright_green << "Rounding" << reset
        << setw(20) << bold << bright_green << "Abs+Others" << reset << '\n';
    cout << bold << bright_green << string(55, '-') << reset << '\n';
    cout << bold << bright_green << setw(3) << "exp()" << setw(27) << "floor()" << setw(25) << "abs()\n" << reset;
    cout << bold << bright_green << setw(3) << "log()" << setw(26) << "ceil()" << setw(26) << "min()\n" << reset;
    cout << bold << bright_green << setw(3) << "log10()" << setw(25) << "round()" << setw(25) << "max()\n" << reset;
    cout << bold << bright_green << setw(57) << "min()\n" << reset;
    cout << bold << green << string(55, '-') << reset << '\n';
    cout << bold << bright_green << setw(0) << "Other expresions\n" << reset;
    cout << bold << green << string(55, '-') << reset << '\n';
    cout << bold << bright_green << setw(0) << "gcd('g') lcm('l') factorial('f)' sum('s') product('p')\n" << reset; 
    cout << bold << green << string(55, '-') << reset << '\n';
    cout << bright_green <<"#NOTE: if you want to calculate normal expression\njust type like: 5+4*8-sin(90)+pow(2,3)\n" << reset;
    exprtk::symbol_table<double> symbol_table; 
    symbol_table.add_function("cbrt", exprtk_cbrt);
    cout << bold << bright_blue << "Pleases enter operation you want to perform: " << reset;
    cin >> expressionStr;
    if(expressionStr.length() == 1){
        func_sci = expressionStr[0];
        switch(func_sci){
            case('q'): cout << "exited\n"; break;
            case('p'): {product(); break;}
            case('s'): {sigma_s(); break;}
            case('f'): {factorial(); break;}
            case('l'): {lcm(); break;}
            case('g'): {gcd(); break;}
            default: break;
        }
        return;
    } else{
        exprtk::symbol_table<double> symbol_table;
        symbol_table.add_variable("pi", pi);
        symbol_table.add_variable("e", e);
        exprtk::expression<double> expr;
        expr.register_symbol_table(symbol_table);
        exprtk::parser<double> parser;
        if (!parser.compile(expressionStr, expr)) {
            std::cerr << "Error: " << parser.error() << "\n";
            return; //if cannot parse
        }
        double result = expr.value();
        cout << bold << underline << red << fixed << setprecision(13) << "Returned value: "<< result << endl << reset;
        return;
    }
    return;
}
void extra(void){
    return;
}
void graph(void){
    cout << bold << bright_blue <<"Enter expression (x, x^2, sin(x), ...): " << reset;
    cin >> expressionStr;
    int width = 85;
    int height = 30;
    cout <<bold << bright_blue << "Enter width and height follow this format: WIDTH[space]HEIGHT (85 27 recomended): " << reset;
    cin >> width >> height;
    cout << bold << bright_blue << "Please enter precision (beta, 1 for default): " << reset;
    cin >> precision;
    plotGraph(expressionStr, width, height);
    cMaxMin(expressionStr, -10.0, 10.0, maxY, minY);
    cout << bold << red << underline <<"Max Y: " << maxY << endl;
    cout << "Min Y: " << minY << endl << reset;
    return;
}
void init(){
    cout << red << bold <<underline << "Welcome to SMCTL's sci-calc! version" << version << reset << endl;
    cout << green << bold <<"Please enter operation s for scientific, g for graphing and e for extra (comming soon!): " << reset;
    cin >> mode;
    switch(mode){
        case('s'): scientific(); break;
        case('g'): graph(); break;
        case('e'): extra(); break;
        default: break;
    } 
    return;
}
