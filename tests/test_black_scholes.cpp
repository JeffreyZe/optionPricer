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
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option callOption(100.0, 1.0, OptionType::Call);

    double price = pricer.price(callOption);
    EXPECT_NEAR(price, 10.4506, 1e-4); // Expected value may vary based on implementation
}

TEST(BlackScholesPricerTest, PutOptionPrice)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option putOption(100.0, 1.0, OptionType::Put);

    double price = pricer.price(putOption);
    EXPECT_NEAR(price, 5.5735, 1e-4); // Expected value may vary based on implementation
}

TEST(BlackScholesPricerTest, PutCallParity)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option callOption(100.0, 1.0, OptionType::Call);
    Option putOption(100.0, 1.0, OptionType::Put);

    double callPrice = pricer.price(callOption);
    double putPrice = pricer.price(putOption);

    // Put-Call Parity: C - P = S - K * exp(-r * T)
    double expectedParity = 100.0 - 100.0 * std::exp(-0.05 * 1.0);
    EXPECT_NEAR(callPrice - putPrice, expectedParity, 1e-4);
}

// Test for delta matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionDelta)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option callOption(100.0, 1.0, OptionType::Call);

    double delta = pricer.delta(callOption);
    double expectedDelta = finiteDifference([&](double spot)
                                            {
                                                BlackScholesPricer tmpPricer(spot, 0.05, 0.2);
                                                return tmpPricer.price(Option(100.0, 1.0, OptionType::Call)); }, 100.0);

    EXPECT_NEAR(delta, expectedDelta, 1e-4);
}

// Test for gamma matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionGamma)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option callOption(100.0, 1.0, OptionType::Call);

    double gamma = pricer.gamma(callOption);
    double expectedGamma = finiteDifference([&](double spot)
                                            {
                                                BlackScholesPricer tmpPricer(spot, 0.05, 0.2);
                                                return tmpPricer.delta(Option(100.0, 1.0, OptionType::Call)); }, 100.0);

    EXPECT_NEAR(gamma, expectedGamma, 1e-4);
}

// Test for vega matching finite difference approximation
TEST(BlackScholesPricerTest, CallOptionVega)
{
    BlackScholesPricer pricer(100.0, 0.05, 0.2);
    Option callOption(100.0, 1.0, OptionType::Call);

    double vega = pricer.vega(callOption);
    double expectedVega = finiteDifference([&](double vol)
                                           {
                                               BlackScholesPricer tmpPricer(100.0, 0.05, vol);
                                               return tmpPricer.price(Option(100.0, 1.0, OptionType::Call)); }, 0.2);

    EXPECT_NEAR(vega, expectedVega, 1e-4);
}