#include <gtest/gtest.h>
#include "BlackScholesPricer.hpp"
#include "Option.hpp"

// Finite difference method for numerical derivatives helper function
double finiteDifference(std::function<double(double)> func, double x, double h = 1e-5)
{
    return (func(x + h) - func(x - h)) / (2 * h);
}

TEST(BlackScholesPricerTest, CallOptionPrice)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0); // <-- add 0.0 for dividend
    Option callOption(100.0, 1.0, OptionType::Call);

    double price = pricer.price(callOption);
    EXPECT_NEAR(price, 10.4506, 1e-4);
}

TEST(BlackScholesPricerTest, PutOptionPrice)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0);
    Option putOption(100.0, 1.0, OptionType::Put);

    double price = pricer.price(putOption);
    EXPECT_NEAR(price, 5.5735, 1e-4);
}

TEST(BlackScholesPricerTest, PutCallParity)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0);
    Option callOption(100.0, 1.0, OptionType::Call);
    Option putOption(100.0, 1.0, OptionType::Put);

    double callPrice = pricer.price(callOption);
    double putPrice = pricer.price(putOption);

    double expectedParity = 100.0 - 100.0 * std::exp(-0.05 * 1.0);
    EXPECT_NEAR(callPrice - putPrice, expectedParity, 1e-4);
}

// Test for delta matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionDelta)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0);
    Option callOption(100.0, 1.0, OptionType::Call);

    double delta = pricer.delta(callOption);
    double expectedDelta = finiteDifference([&](double spot)
                                            {
        BlackScholesPricer tmpPricer(spot, 0.05, 0.2, 0.0);
        return tmpPricer.price(Option(100.0, 1.0, OptionType::Call)); }, 100.0);

    EXPECT_NEAR(delta, expectedDelta, 1e-4);
}

// Test for gamma matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionGamma)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0);
    Option callOption(100.0, 1.0, OptionType::Call);

    double gamma = pricer.gamma(callOption);
    double expectedGamma = finiteDifference([&](double spot)
                                            {
        BlackScholesPricer tmpPricer(spot, 0.05, 0.2, 0.0);
        return tmpPricer.delta(Option(100.0, 1.0, OptionType::Call)); }, 100.0);

    EXPECT_NEAR(gamma, expectedGamma, 1e-4);
}

// Test for vega matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionVega)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2, 0.0);
    Option callOption(100.0, 1.0, OptionType::Call);

    double vega = pricer.vega(callOption);
    double expectedVega = finiteDifference([&](double vol)
                                           {
        BlackScholesPricer tmpPricer(100.0, 0.05, vol, 0.0);
        return tmpPricer.price(Option(100.0, 1.0, OptionType::Call)); }, 0.2);

    EXPECT_NEAR(vega, expectedVega, 1e-4);
}

TEST(BSM_Dividends, PriceImpactAndGreeks)
{
    // dividendYield q is 0.03
    double S = 100, K = 100, r = 0.05, q = 0.03, vol = 0.2, T = 1.0;
    Option call(K, T, OptionType::Call), put(K, T, OptionType::Put);

    BlackScholesPricer p0(S, r, vol, 0.0), pq(S, r, vol, q);

    EXPECT_GT(p0.price(call), pq.price(call));
    EXPECT_LT(p0.price(put), pq.price(put));

    // Finite difference delta check (spot bump)
    auto priceCallAt = [&](double s)
    { return BlackScholesPricer(s, r, vol, q).price(call); };
    double h = 1e-3;
    double fdDelta = (priceCallAt(S + h) - priceCallAt(S - h)) / (2 * h);
    EXPECT_NEAR(fdDelta, pq.delta(call), 1e-4);

    // IV with dividends round trip
    double mkt = pq.price(call);
    double iv = BlackScholesPricer::impliedVolatility(call, S, r, mkt, 0.3, 1e-8, 200, q);
    EXPECT_NEAR(iv, vol, 1e-6);
}