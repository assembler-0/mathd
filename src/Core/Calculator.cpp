
#include "Core/Calculator.hpp"
#include <cmath>
#include <limits>

// Custom function for cbrt to be registered with exprtk
static double exprtk_cbrt_impl(const double val) {
    return std::cbrt(val);
}

Calculator::Calculator() : m_XVal(0) {
    SetupSymbolTable();
}

void Calculator::SetupSymbolTable() {
    m_SymbolTable.add_variable("x", m_XVal);
    m_SymbolTable.add_constant("pi", M_PI);
    m_SymbolTable.add_constant("e", M_E);
    m_SymbolTable.add_function("cbrt", exprtk_cbrt_impl);
    m_Expression.register_symbol_table(m_SymbolTable);
}

double Calculator::EvaluateExpression(const std::string& expression) {
    if (m_Parser.compile(expression, m_Expression)) {
        return m_Expression.value();
    }
    return std::numeric_limits<double>::quiet_NaN();
}

long long Calculator::CalculateFactorial(const int n) {
    if (n < 0) return 0;
    if (n > 20) { // Prevent overflow for long long
        return -1; // Indicate error
    }
    long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double Calculator::CalculateGcd(double a, double b) {
    a = std::abs(a);
    b = std::abs(b);
    while (std::abs(b) > 1e-9) {
        double temp = b;
        b = std::fmod(a, b);
        a = temp;
    }
    return a;
}

double Calculator::CalculateLcm(double a, double b) {
    if (std::abs(a) < 1e-9 || std::abs(b) < 1e-9) return 0;
    const double gcd_val = CalculateGcd(a, b);
    if (std::abs(gcd_val) < 1e-9) return std::numeric_limits<double>::quiet_NaN();
    return std::abs(a * b) / gcd_val;
}

double Calculator::CalculateProductSeries(const std::string& expr, const int start, const int end) {
    if (!m_Parser.compile(expr, m_Expression)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double totalProduct = 1.0;
    for (int i = start; i <= end; ++i) {
        m_XVal = static_cast<double>(i);
        totalProduct *= m_Expression.value();
        if (std::isnan(totalProduct) || std::isinf(totalProduct)) break;
    }
    return totalProduct;
}

double Calculator::CalculateSumSeries(const std::string& expr, const int start, const int end) {
    if (!m_Parser.compile(expr, m_Expression)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double totalSum = 0.0;
    for (int i = start; i <= end; ++i) {
        m_XVal = static_cast<double>(i);
        totalSum += m_Expression.value();
        if (std::isnan(totalSum) && !(i == start && std::isnan(m_Expression.value()))) break;
    }
    return totalSum;
}

void Calculator::CalculateMinMaxY(const std::string& expr, const double xMin, const double xMax, const int numSamples, double& outMinY, double& outMaxY) {
    outMinY = std::numeric_limits<double>::infinity();
    outMaxY = -std::numeric_limits<double>::infinity();

    if (xMin >= xMax || numSamples <= 0) {
        outMinY = std::numeric_limits<double>::quiet_NaN();
        outMaxY = std::numeric_limits<double>::quiet_NaN();
        return;
    }
    if (!m_Parser.compile(expr, m_Expression)) {
        outMinY = std::numeric_limits<double>::quiet_NaN();
        outMaxY = std::numeric_limits<double>::quiet_NaN();
        return;
    }

    double step = (xMax - xMin) / std::max(1, numSamples - 1);
    for (int i = 0; i < numSamples; ++i) {
        m_XVal = xMin + i * step;
        double y = m_Expression.value();
        if (!std::isnan(y)) {
            if (y < outMinY) outMinY = y;
            if (y > outMaxY) outMaxY = y;
        }
    }
    if (std::isinf(outMinY) || std::isinf(outMaxY)) {
        if (std::isinf(outMinY) && !std::isinf(outMaxY)) outMinY = outMaxY - 1;
        else if (!std::isinf(outMinY) && std::isinf(outMaxY)) outMaxY = outMinY + 1;
        else {
            outMinY = -1.0;
            outMaxY = 1.0;
        }
    }
}

void Calculator::SetX(double x) {
    m_XVal = x;
}
