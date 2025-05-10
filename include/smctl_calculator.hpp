#pragma once

#include "exprtk.hpp"
#include "termcolor.hpp" // For colored output
#include "fmt/core.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>      // For std::abs, std::fmod, std::cbrt, NAN, INFINITY
#include <iomanip>    // For std::setprecision, std::fixed
#include <limits>     // For std::numeric_limits
#include <algorithm>  // For std::min, std::max (though direct comparison is often used)

// Using namespaces within the .hpp for brevity as it's a self-contained example.
// In larger projects, prefer 'std::' and 'termcolor::' prefixes or 'using' declarations in .cpp files / specific scopes.
using namespace std;
using namespace termcolor;

class Calculator {
public:
    Calculator() : m_x_val(0), m_graphPlotDensityFactor(1) {
        setupSymbolTable(); // Initialize the symbol table once
    }

    // Main entry point to run the calculator
    void run() {
        cout << red << bold << underline << "Welcome to SMCTL's sci-calc! version " << APP_VERSION << reset << endl;
        char mode_choice;
        while (true) {
            cout << green << bold << "\nPlease enter operation: [s]cientific, [g]raphing, [e]xtra (NYI), [q]uit: " << reset;
            cin >> mode_choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline

            switch (mode_choice) {
                case 's':
                    showScientificCalculator();
                    break;
                case 'g':
                    showGraphingTool();
                    break;
                case 'e':
                    cout << yellow << "Extra features are not yet implemented." << reset << endl;
                    break;
                case 'q':
                    cout << bright_blue << "Exiting calculator. Goodbye!" << reset << endl;
                    return;
                default:
                    cout << yellow << "Invalid option. Please try again." << reset << endl;
                    break;
            }
        }
    }

private:
    // --- Constants ---
    static constexpr const char* APP_VERSION = "2.0 - Refactored";
    static constexpr double PI_CONST = 3.14159265358979323846;
    static constexpr double E_CONST  = 2.71828182845904523536;

    // --- Member Variables ---
    double m_x_val; // Value for the 'x' variable in expressions
    int m_graphPlotDensityFactor; // Controls how many points are evaluated for graphing relative to width

    // exprtk objects
    exprtk::symbol_table<double> m_symbolTable;
    exprtk::expression<double> m_expression;
    exprtk::parser<double> m_parser;
    std::string m_currentExpressionStr; // Stores the last successfully compiled expression string

    // --- exprtk Setup ---
    // Custom function for cbrt to be registered with exprtk
    static double exprtk_cbrt_impl(double val) {
        return std::cbrt(val);
    }

    void setupSymbolTable() {
        m_symbolTable.add_variable("x", m_x_val);
        m_symbolTable.add_constant("pi", PI_CONST);
        m_symbolTable.add_constant("e", E_CONST);
        m_symbolTable.add_function("cbrt", exprtk_cbrt_impl);
        // exprtk typically registers standard math functions (sin, cos, log, etc.) by default.
        // If not, they can be added:
        // m_symbolTable.add_package(exprtk::common::numeric::package);
        // m_symbolTable.add_package(exprtk::common::trigonometry::package);
        // etc.
        m_expression.register_symbol_table(m_symbolTable);
    }

    // Compiles the given expression string. Returns true on success, false on error.
    // Stores the compiled expression in m_expression and the string in m_currentExpressionStr.
    bool compileExpression(const std::string& expressionStr) {
        if (!m_parser.compile(expressionStr, m_expression)) {
            cerr << red << "Error parsing expression: " << m_parser.error() << reset << endl;
            m_currentExpressionStr.clear(); // Clear invalid expression
            return false;
        }
        m_currentExpressionStr = expressionStr;
        return true;
    }

    // Evaluates the currently compiled expression.
    // Assumes m_expression is valid and compiled.
    // For expressions involving 'x', m_x_val should be set before calling.
    double evaluateCurrentlyCompiledExpression() {
        return m_expression.value();
    }
    
    // Evaluates a new expression string. Compiles it first.
    // This is for one-off evaluations. For loops, compile once then update m_x_val.
    double evaluateNewExpression(const std::string& expressionStr) {
        if (compileExpression(expressionStr)) {
            return evaluateCurrentlyCompiledExpression();
        }
        return NAN; // Return NaN on error
    }


    // --- Core Calculation Functions ---
    long long calculateFactorial(int n_val) {
        if (n_val < 0) return 0; // Factorial not defined for negative numbers
        if (n_val > 20) { // Prevent overflow for long long
            cerr << yellow << "Factorial for n > 20 might overflow." << reset << endl;
            // Or return a specific error code / throw exception
        }
        long long result = 1;
        for (int i = 1; i <= n_val; ++i) {
            result *= i;
        }
        return result;
    }

    double calculateGcd(double a_val, double b_val) {
        // Using std::fmod for doubles. Note: GCD for non-integers can be tricky.
        // This implementation assumes integer-like behavior.
        a_val = std::abs(a_val);
        b_val = std::abs(b_val);
        while (std::abs(b_val) > 1e-9) { // Tolerance for double comparison
            double temp = b_val;
            b_val = std::fmod(a_val, b_val);
            a_val = temp;
        }
        return a_val;
    }

    double calculateLcm(double a_val, double b_val) {
        if (std::abs(a_val) < 1e-9 || std::abs(b_val) < 1e-9) return 0; // LCM with zero is zero
        double gcd_val = calculateGcd(a_val, b_val);
        if (std::abs(gcd_val) < 1e-9) return NAN; // Should not happen if a or b is non-zero
        return std::abs(a_val * b_val) / gcd_val;
    }

    // Calculates product of f(x) from start_x to end_x (integer steps)
    double calculateProductSeries(const std::string& exprStr, int start_x, int end_x) {
        if (!compileExpression(exprStr)) {
            return NAN;
        }
        double totalProduct = 1.0;
        for (int i = start_x; i <= end_x; ++i) {
            m_x_val = static_cast<double>(i);
            totalProduct *= evaluateCurrentlyCompiledExpression();
            if (std::isnan(totalProduct) || std::isinf(totalProduct)) break; // Stop on invalid result
        }
        return totalProduct;
    }

    // Calculates sum of f(x) from start_x to end_x (integer steps)
    double calculateSumSeries(const std::string& exprStr, int start_x, int end_x) {
        if (!compileExpression(exprStr)) {
            return NAN;
        }
        double totalSum = 0.0;
        for (int i = start_x; i <= end_x; ++i) {
            m_x_val = static_cast<double>(i);
            totalSum += evaluateCurrentlyCompiledExpression();
            if (std::isnan(totalSum) && !(i==start_x && std::isnan(m_expression.value())) ) break; // Stop on invalid result unless first term is NaN
        }
        return totalSum;
    }

    // Calculates Min and Max Y values for an expression over an X range
    void calculateMinMaxY(const std::string& exprStr, double xMin, double xMax, int numSamples, double& outMinY, double& outMaxY) {
        outMinY = std::numeric_limits<double>::infinity();
        outMaxY = -std::numeric_limits<double>::infinity();

        if (xMin >= xMax || numSamples <= 0) {
             cerr << yellow << "Invalid range or zero samples for MinMaxY calculation." << reset << endl;
            outMinY = NAN; outMaxY = NAN;
            return;
        }
        if (!compileExpression(exprStr)) { // Compile the expression once
            outMinY = NAN; outMaxY = NAN;
            return;
        }

        double step = (xMax - xMin) / std::max(1, numSamples - 1);
        for (int i = 0; i < numSamples; ++i) {
            m_x_val = xMin + i * step;
            double y = evaluateCurrentlyCompiledExpression();
            if (!std::isnan(y)) {
                if (y < outMinY) outMinY = y;
                if (y > outMaxY) outMaxY = y;
            }
        }
         if (std::isinf(outMinY) || std::isinf(outMaxY)) {
            // If only one bound was found, set the other to something reasonable or indicate error
            if (std::isinf(outMinY) && !std::isinf(outMaxY)) outMinY = outMaxY -1; // Default if only max found
            else if (!std::isinf(outMinY) && std::isinf(outMaxY)) outMaxY = outMinY +1; // Default if only min found
            else { // both inf, means no valid points found
                 outMinY = -1.0; outMaxY = 1.0; // Default range if no points found
                 cout << yellow << "Warning: No valid points found in the given range for MinMaxY. Using default Y range." << reset << endl;
            }
        }
    }

    // --- UI and Interaction Functions ---
    void displayScientificMenu() {
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << setw(0) << bold << bright_green << "Trigonometric" << reset
             << setw(18) << bold << bright_green << "Hyperbolic" << reset
             << setw(18) << bold << bright_green << "Power/Root" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << bright_green << left << setw(15) << " sin(x)" << setw(15) << " sinh(x)" << setw(15) << " pow(b,exp)" << setw(15) << " sqrt(x)" << reset << '\n';
        cout << bright_green << left << setw(15) << " cos(x)" << setw(15) << " cosh(x)" << setw(15) << " cbrt(x)" << setw(15) << " log(x)" << reset << '\n';
        cout << bright_green << left << setw(15) << " tan(x)" << setw(15) << " tanh(x)" << setw(15) << " exp(x)" << setw(15) << " log10(x)" << reset << '\n';
        cout << bright_green << left << setw(15) << " asin(x)" << setw(15) << " asinh(x)" << reset << '\n';
        cout << bright_green << left << setw(15) << " acos(x)" << setw(15) << " acosh(x)" << reset << '\n';
        cout << bright_green << left << setw(15) << " atan(x)" << setw(15) << " atanh(x)" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << setw(0) << bold << bright_green << "Rounding" << reset
             << setw(20) << bold << bright_green << "Absolute/Min/Max" << reset
             << setw(15) << bold << bright_green << "Other" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << bright_green << left << setw(15) << " floor(x)" << setw(20) << " abs(x)" << setw(15) << " gcd(a,b) ('G')" << reset << '\n';
        cout << bright_green << left << setw(15) << " ceil(x)" << setw(20) << " min(a,b)" << setw(15) << " lcm(a,b) ('L')" << reset << '\n';
        cout << bright_green << left << setw(15) << " round(x)" << setw(20) << " max(a,b)" << setw(15) << " factorial(n) ('F')" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << setw(0) << bold << bright_green << "Series Operations (enter 'P' or 'S')" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << bright_green << left << setw(30) << " Product ('P'): PI[f(x)]" << setw(30) << " Sum ('S'): SIGMA[f(x)]" << reset << '\n';
        cout << bold << green << string(60, '-') << reset << '\n';
        cout << bright_green << "# NOTE: For general expressions, just type them e.g., 5+4*8-sin(pi/2)+pow(2,3)" << reset << '\n';
        cout << bright_green << "# Type 'm' for menu, 'q' to return to main menu." << reset << endl;
    }

    void showScientificCalculator() {
        string inputStr;
        displayScientificMenu();

        while (true) {
            cout << bold << bright_blue << "\nSciCalc > " << reset;
            getline(cin, inputStr);

            if (inputStr.empty()) continue;
            if (inputStr == "q" || inputStr == "Q") break;
            if (inputStr == "m" || inputStr == "M") {
                displayScientificMenu();
                continue;
            }

            char firstChar = inputStr[0];
            if (inputStr.length() == 1 && std::isalpha(firstChar)) {
                firstChar = std::tolower(firstChar);
                if (firstChar == 'f') { // Factorial
                    int n_val;
                    cout << bright_blue << "Enter integer value for factorial n: " << reset;
                    cin >> n_val;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << red << underline << bold << "Factorial(" << n_val << ") = " << calculateFactorial(n_val) << reset << endl;
                } else if (firstChar == 'g' || firstChar == 'l') { // GCD or LCM
                    double val_a, val_b;
                    cout << bright_blue << "Enter a: " << reset; cin >> val_a;
                    cout << bright_blue << "Enter b: " << reset; cin >> val_b;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (firstChar == 'g') {
                        cout << red << underline << bold << "GCD(" << val_a << ", " << val_b << ") = " << calculateGcd(val_a, val_b) << reset << endl;
                    } else {
                        cout << red << underline << bold << "LCM(" << val_a << ", " << val_b << ") = " << calculateLcm(val_a, val_b) << reset << endl;
                    }
                } else if (firstChar == 'p' || firstChar == 's') { // Product or Sum Series
                    string seriesExprStr;
                    int start_idx, end_idx;
                    cout << bright_blue << "Enter function f(x) for series: " << reset; getline(cin, seriesExprStr);
                    cout << bright_blue << "Enter integer start index for x: " << reset; cin >> start_idx;
                    cout << bright_blue << "Enter integer end index for x: " << reset; cin >> end_idx;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (firstChar == 'p') {
                        double result = calculateProductSeries(seriesExprStr, start_idx, end_idx);
                        cout << red << underline << bold << "Product Series Result: " << result << reset << endl;
                    } else {
                        double result = calculateSumSeries(seriesExprStr, start_idx, end_idx);
                        cout << red << underline << bold << "Sum Series Result: " << result << reset << endl;
                    }
                } else {
                     cout << yellow << "Unknown single-letter command. Try 'm' for menu." << reset << endl;
                }
            } else { // General expression
                double result = evaluateNewExpression(inputStr);
                if (!std::isnan(result)) {
                    cout << red << underline << bold << fixed << setprecision(10) << "Result: " << result << reset << endl;
                }
            }
        }
    }
    
    void plotAsciiGraph(const std::string& exprStr, int width, int height,
                        double xMin, double xMax, double yMinActual, double yMaxActual,
                        int plotDensityFactor) {
        if (width <= 0 || height <= 0) {
            cerr << red << "Error: Graph width and height must be positive." << reset << endl;
            return;
        }
        if (xMin >= xMax) {
             cerr << red << "Error: xMin must be less than xMax for graphing." << reset << endl;
            return;
        }
         if (yMinActual >= yMaxActual) { // If yMinActual == yMaxActual (e.g. constant function), create a small range.
            yMinActual -= 0.5;
            yMaxActual += 0.5;
            if (yMinActual >= yMaxActual) { // Still equal (e.g. if original was 0, now -0.5, 0.5)
                 yMinActual = yMaxActual -1.0; // Ensure different
            }
         }


        // Compile the expression once for the entire plotting process
        if (!compileExpression(exprStr)) {
            return; // Error message already printed by compileExpression
        }

        vector<string> canvas(height, string(width, ' '));

        // --- Axis Drawing ---
        // Determine Y position of X-axis
        int xAxisRow = -1;
        if (yMinActual <= 0 && yMaxActual >= 0) { // X-axis is visible
            xAxisRow = static_cast<int>((yMaxActual - 0) * (height - 1) / (yMaxActual - yMinActual));
        }
         // Ensure xAxisRow is within bounds
        if (xAxisRow < 0) xAxisRow = height -1; // If all y > 0, put axis at bottom
        if (xAxisRow >= height) xAxisRow = 0; // If all y < 0, put axis at top
        
        for (int w = 0; w < width; ++w) {
            if (xAxisRow >=0 && xAxisRow < height) canvas[xAxisRow][w] = '-';
        }

        // Determine X position of Y-axis
        int yAxisCol = -1;
        if (xMin <= 0 && xMax >= 0) { // Y-axis is visible
            yAxisCol = static_cast<int>((0 - xMin) * (width - 1) / (xMax - xMin));
        }
        // Ensure yAxisCol is within bounds
        if (yAxisCol < 0) yAxisCol = 0; // If all x > 0, put axis at left
        if (yAxisCol >= width) yAxisCol = width -1; // If all x < 0, put axis at right


        for (int h = 0; h < height; ++h) {
             if (yAxisCol >=0 && yAxisCol < width) canvas[h][yAxisCol] = '|';
        }

        // Origin character
        if (xAxisRow >= 0 && xAxisRow < height && yAxisCol >= 0 && yAxisCol < width) {
            canvas[xAxisRow][yAxisCol] = '+';
        }
        
        // --- Plotting Points ---
        // Number of points to evaluate based on width and density factor
        int numEvalPoints = std::max(width, width * plotDensityFactor); // Ensure at least 'width' points
        double xStep = (xMax - xMin) / std::max(1, numEvalPoints - 1);

        for (int i = 0; i < numEvalPoints; ++i) {
            m_x_val = xMin + i * xStep;
            double y = evaluateCurrentlyCompiledExpression();

            if (!std::isnan(y)) {
                // Map x to canvas column
                int plotX = static_cast<int>((m_x_val - xMin) * (width - 1) / (xMax - xMin));
                // Map y to canvas row (inverted: 0 at top for console)
                int plotY = static_cast<int>((yMaxActual - y) * (height - 1) / (yMaxActual - yMinActual));

                if (plotX >= 0 && plotX < width && plotY >= 0 && plotY < height) {
                    canvas[plotY][plotX] = '*';
                }
            }
        }

        // Print the canvas
        cout << bold << bright_cyan << "\n--- Graph of y = " << exprStr << " ---" << reset << endl;
        cout << bright_cyan << "X range: [" << xMin << ", " << xMax << "], Y range: [" << yMinActual << ", " << yMaxActual << "]" << reset << endl;
        for (int h = 0; h < height; ++h) {
            cout << bold << bright_green << canvas[h] << reset << endl;
        }
        cout << bold << bright_cyan << "--- End of Graph ---" << reset << endl;
    }


    void showGraphingTool() {
        string exprStr;
        int graphWidth = 80, graphHeight = 25; // Default dimensions
        double xMin = -10.0, xMax = 10.0;    // Default X range
        int densityFactor = 1;               // Default density

        cout << bold << bright_blue << "Enter expression in terms of x (e.g., x^2, sin(x)): " << reset;
        getline(cin, exprStr);
        if (exprStr.empty()) {
            cout << yellow << "No expression entered. Aborting graph." << reset << endl;
            return;
        }

        cout << bold << bright_blue << "Enter graph width (default " << graphWidth << "): " << reset;
        string tempInput;
        getline(cin, tempInput);
        if (!tempInput.empty()) graphWidth = std::stoi(tempInput);

        cout << bold << bright_blue << "Enter graph height (default " << graphHeight << "): " << reset;
        getline(cin, tempInput);
        if (!tempInput.empty()) graphHeight = std::stoi(tempInput);
        
        cout << bold << bright_blue << "Enter X-min (default " << xMin << "): " << reset;
        getline(cin, tempInput);
        if (!tempInput.empty()) xMin = std::stod(tempInput);

        cout << bold << bright_blue << "Enter X-max (default " << xMax << "): " << reset;
        getline(cin, tempInput);
        if (!tempInput.empty()) xMax = std::stod(tempInput);

        cout << bold << bright_blue << "Enter plot density factor (integer, default " << densityFactor << ", higher is more detailed): " << reset;
        getline(cin, tempInput);
        if (!tempInput.empty()) densityFactor = std::stoi(tempInput);
        
        m_graphPlotDensityFactor = std::max(1, densityFactor); // Ensure it's at least 1

        if (graphWidth <= 0 || graphHeight <=0 || xMin >= xMax) {
            cerr << red << "Invalid graph parameters. Aborting." << reset << endl;
            return;
        }

        double actualMinY, actualMaxY;
        // Calculate Min/Max Y. Sample more points than densityFactor for better accuracy.
        int samplesForMinMax = std::max(graphWidth * m_graphPlotDensityFactor * 2, 500); // e.g. twice the plot points or at least 500
        cout << yellow << "Calculating Y range for the expression..." << reset << endl;
        calculateMinMaxY(exprStr, xMin, xMax, samplesForMinMax, actualMinY, actualMaxY);

        if (std::isnan(actualMinY) || std::isnan(actualMaxY)) {
            cerr << red << "Could not determine Y range for the expression. Aborting graph." << reset << endl;
            // This can happen if compileExpression failed inside calculateMinMaxY or all points were NaN
            return;
        }
        
        cout << green << "Calculated Y range: [" << actualMinY << ", " << actualMaxY << "]" << reset << endl;

        plotAsciiGraph(exprStr, graphWidth, graphHeight, xMin, xMax, actualMinY, actualMaxY, m_graphPlotDensityFactor);
    }
};
