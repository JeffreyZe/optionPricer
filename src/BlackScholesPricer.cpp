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

double BlackScholesPricer::delta(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();
    OptionType type = option.getType();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d1 = (std::log(m_spot / K) + (m_rate + 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;

    if (type == OptionType::Call)
    {
        return normalCDF(d1);
    }
    else
    {
        return normalCDF(d1) - 1.0;
    }
}

double BlackScholesPricer::gamma(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d1 = (std::log(m_spot / K) + (m_rate + 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;

    return normalPDF(d1) / (m_spot * sigmaSqrtT);
}

double BlackScholesPricer::vega(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d1 = (std::log(m_spot / K) + (m_rate + 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;

    return m_spot * normalPDF(d1) * std::sqrt(T);
}

double BlackScholesPricer::theta(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();
    OptionType type = option.getType();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d1 = (std::log(m_spot / K) + (m_rate + 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;
    double d2 = d1 - sigmaSqrtT;

    if (type == OptionType::Call)
    {
        // Return theta per year (do not divide by 365)
        return -m_spot * normalPDF(d1) * m_volatility / (2 * std::sqrt(T)) - m_rate * K * std::exp(-m_rate * T) * normalCDF(d2);
    }
    else
    {
        // Return theta per year (do not divide by 365)
        return -m_spot * normalPDF(d1) * m_volatility / (2 * std::sqrt(T)) + m_rate * K * std::exp(-m_rate * T) * normalCDF(-d2);
    }
}

double BlackScholesPricer::rho(const Option &option) const
{
    double K = option.getStrike();
    double T = option.getMaturity();
    OptionType type = option.getType();

    double sigmaSqrtT = m_volatility * std::sqrt(T);

    double d2 = (std::log(m_spot / K) + (m_rate - 0.5 * m_volatility * m_volatility) * T) / sigmaSqrtT;

    if (type == OptionType::Call)
    {
        return K * T * std::exp(-m_rate * T) * normalCDF(d2);
    }
    else
    {
        return -K * T * std::exp(-m_rate * T) * normalCDF(-d2);
    }
}
