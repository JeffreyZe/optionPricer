#include "Option.hpp"

Option::Option(double strike, double maturity, OptionType type)
    : m_strike(strike), m_maturity(maturity), m_type(type) {}

double Option::getStrike() const
{
    return m_strike;
}

double Option::getMaturity() const
{
    return m_maturity;
}

OptionType Option::getType() const
{
    return m_type;
}
