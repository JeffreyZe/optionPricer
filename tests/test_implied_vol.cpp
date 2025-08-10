#include "gtest/gtest.h"
#include "Option.hpp"
#include "BlackScholesPricer.hpp"

TEST(BlackScholesPricerTest, ImpliedVolatility_CallAndPut)
{
    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.05;
    double maturity = 1.0;
    double trueVol = 0.2;

    // ===== Call Option =====
    {
        Option callOption(strike, maturity, OptionType::Call);

        // Market price from true vol
        BlackScholesPricer pricer(spot, rate, trueVol);
        double marketPrice = pricer.price(callOption);

        // Good initial guess
        double estimatedVol = BlackScholesPricer::impliedVolatility(
            callOption, spot, rate, marketPrice, 0.3, 1e-8, 100);

        EXPECT_NEAR(estimatedVol, trueVol, 1e-6);

        // Bad initial guess (stress test)
        double estimatedVolBadGuess = BlackScholesPricer::impliedVolatility(
            callOption, spot, rate, marketPrice, 1.5, 1e-8, 200);

        EXPECT_NEAR(estimatedVolBadGuess, trueVol, 1e-3); // Use a looser tolerance
        EXPECT_GT(estimatedVolBadGuess, 1e-5); // Should not hit lower bound
        EXPECT_LT(estimatedVolBadGuess, 5.0);  // Should not hit upper bound

        double priceFromEstimatedVolBadGuess = BlackScholesPricer(spot, rate, estimatedVolBadGuess).price(callOption);
        EXPECT_NEAR(priceFromEstimatedVolBadGuess, marketPrice, 1e-3);
    }

    // ===== Put Option =====
    {
        Option putOption(strike, maturity, OptionType::Put);

        // Market price from true vol
        BlackScholesPricer pricer(spot, rate, trueVol);
        double marketPrice = pricer.price(putOption);

        // Good initial guess
        double estimatedVol = BlackScholesPricer::impliedVolatility(
            putOption, spot, rate, marketPrice, 0.25, 1e-8, 100);

        EXPECT_NEAR(estimatedVol, trueVol, 1e-6);

        // Bad initial guess (stress test)
        double estimatedVolBadGuess = BlackScholesPricer::impliedVolatility(
            putOption, spot, rate, marketPrice, 2.0, 1e-8, 200);

        EXPECT_NEAR(estimatedVolBadGuess, trueVol, 1e-3); // Use a looser tolerance
        EXPECT_GT(estimatedVolBadGuess, 1e-5); // Should not hit lower bound
        EXPECT_LT(estimatedVolBadGuess, 5.0);  // Should not hit upper bound
    }
}