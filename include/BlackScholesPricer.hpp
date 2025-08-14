#pragma once
#include "Option.hpp"

class BlackScholesPricer
{
public:
    BlackScholesPricer(double spot, double rate, double volatility,
                       double dividendYield = 0.0);

    double price(const Option &option) const;

    // Greeks
    double delta(const Option &option) const;
    double gamma(const Option &option) const;
    double vega(const Option &option) const;
    double theta(const Option &option) const;
    double rho(const Option &option) const;

    // Implied volatility
    static double impliedVolatility(const Option &option,
                                    double spot,
                                    double rate,
                                    double marketPrice,
                                    double initialVol = 0.2,
                                    double tolerance = 1e-6,
                                    int maxIterations = 100,
                                    double divisionYield = 0.0);

private:
    double m_spot;
    double m_rate;
    double m_volatility;
    double m_dividendYield;
};
