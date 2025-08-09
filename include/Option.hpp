#pragma once

enum class OptionType
{
    Call,
    Put
};

class Option
{
public:
    Option(double strike, double maturity, OptionType type);
    virtual ~Option() = default;

    double getStrike() const;
    double getMaturity() const;
    OptionType getType() const;

private:
    double m_strike;
    double m_maturity;
    OptionType m_type;
};
