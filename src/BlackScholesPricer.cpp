#include "BlackScholesPricer.hpp"
#include "Utils.hpp"
#include <cmath>

BlackScholesPricer::BlackScholesPricer(double spot, double rate, double volatility)
    : m_spot(spot), m_rate(rate), m_volatility(volatility) {}

double BlackScholesPricer::price(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();
    OptionType type = option.getType();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d1 = (std::log(m_spot / K) + (m_rate + 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;
    double d2 = d1 - sigmaSqrtT;

    if (type == OptionType::Call)
    {
        return m_spot * normalCDF(d1) - K * std::exp(-m_rate * T) * normalCDF(d2);
    }
    else
    {
        return K * std::exp(-m_rate * T) * normalCDF(-d2) - m_spot * normalCDF(-d1);
    }
}
