
#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "exprtk.hpp"
#include <string>
#include <vector>

class Calculator {
public:
    Calculator();

    [[nodiscard]] double EvaluateExpression(const std::string& expression);
    [[nodiscard]] static long long CalculateFactorial(int n);
    [[nodiscard]] static double CalculateGcd(double a, double b);
    [[nodiscard]] static double CalculateLcm(double a, double b);
    [[nodiscard]] double CalculateProductSeries(const std::string& expr, int start, int end);
    [[nodiscard]] double CalculateSumSeries(const std::string& expr, int start, int end);
    void CalculateMinMaxY(const std::string& expr, double xMin, double xMax, int numSamples, double& yMin, double& yMax);
    void SetX(double x);

private:
    void SetupSymbolTable();

    exprtk::symbol_table<double> m_SymbolTable;
    exprtk::expression<double> m_Expression;
    exprtk::parser<double> m_Parser;
    double m_XVal;
};

#endif //CALCULATOR_HPP
