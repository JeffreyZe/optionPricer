#include "BlackScholesPricer.hpp"
#include "Utils.hpp"
#include <cmath>
#include <algorithm>

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

// Implied volatility using bisection/Newton-Raphson hybrid method:
//   Bracketed search [low, high] to ensure convergence
//   Starts with a clamped initial guess inside [volLower, volUpper]
//   Uses Newton-Raphson if vega is sufficiently large, otherwise falls back to bisection
//   Expands the upper bound if market price is too low
double BlackScholesPricer::impliedVolatility(const Option &option,
                                             double spot,
                                             double rate,
                                             double marketPrice,
                                             double initialVol,
                                             double tolerance,
                                             int maxIterations)
{
    const double volLower = 1e-8;      // lower bound for volatility
    const double volUpper = 5.0;       // upper bound (should be large enough)
    const double vegaTolerance = 1e-8; // treat vega below vegaTolerance as "too small"

    // Helper to compute model price at a given vol
    auto priceAt = [&](double vol)
    {
        BlackScholesPricer p(spot, rate, vol);
        return p.price(option);
    };

    // initialize bracket [low, high]
    double low = volLower;
    double high = volUpper;
    double f_low = priceAt(low) - marketPrice;
    double f_high = priceAt(high) - marketPrice;

    // If marketPrice is smaller than near-zero-vol price (very unlikely though), return low
    if (f_low > 0.0)
        return low;

    // If marketPrice is higher than theorectical price with highest vol,
    // try expanding the high bound (rare for sensible market prices)
    if (f_high < 0.0)
    {
        int expand_iter = 0;
        while (f_high < 0.0 && expand_iter++ < 50)
        {
            high = std::min(high * 2.0, 100.0); // double the high bound, but cap at 100.0
            f_high = priceAt(high) - marketPrice;
            if (high >= 100.0)
                break;
        }
        // if still not bracketed, return current high as best guess
        if (f_high < 0.0)
            return high;
    }

    // Start from a clamped initial guess inside [low, high]
    double vol = std::clamp(initialVol, low, high);
    // double vol = std::max(low, std::min(initialVol, high));

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        BlackScholesPricer p(spot, rate, vol);
        double price = p.price(option);
        double diff = price - marketPrice;

        if (std::fabs(diff) < tolerance)
            return vol; // converged

        double vega = p.vega(option);

        double newVol;
        if (vega > vegaTolerance)
        {
            // Newton-Raphson candidate
            newVol = vol - diff / vega;

            // If NR goes outside the bracket or is NaN, use bisection instead
            if (std::isnan(newVol) || newVol <= low || newVol >= high)
            {
                newVol = 0.5 * (low + high);
            }
        }
        else
        {
            // Vega too small → use bisection
            newVol = 0.5 * (low + high);
        }

        double f_new = priceAt(newVol) - marketPrice;

        // Update bracket using sign of f_new (price - marketPrice)
        if (f_new > 0.0)
        {
            high = newVol;
            f_high = f_new;
        }
        else
        {
            low = newVol;
            f_low = f_new;
        }

        // Set vol to newVol clamped inside updated bracket
        vol = std::max(low, std::min(newVol, high));
    }

    // return best guess if not converged within maxIterations
    return vol;
}
