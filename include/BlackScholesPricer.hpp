#pragma once
#include "Option.hpp"

class BlackScholesPricer {
public:
    BlackScholesPricer(double spot, double rate, double volatility);

    double price(const Option& option) const;
    double delta(const Option& option) const;

private:
    double m_spot;
    double m_rate;
    double m_volatility;
};
