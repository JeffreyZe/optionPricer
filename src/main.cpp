#include <iostream>
#include "Option.hpp"
#include "BlackScholesPricer.hpp"

int main()
{
    double strike = 100.0;
    double maturity = 1.0; // in years
    double spot = 100.0;
    double rate = 0.05; // 5%
    double vol = 0.2;   // 20%

    Option callOption(strike, maturity, OptionType::Call);
    Option putOption(strike, maturity, OptionType::Put);

    BlackScholesPricer pricer(spot, rate, vol);

    std::cout << "=== Call Option ===\n";
    std::cout << "Price: " << pricer.price(callOption) << "\n";
    std::cout << "Delta: " << pricer.delta(callOption) << "\n";
    std::cout << "Gamma: " << pricer.gamma(callOption) << "\n";
    std::cout << "Vega : " << pricer.vega(callOption) << "\n";
    std::cout << "Theta: " << pricer.theta(callOption) << "\n";
    std::cout << "Rho  : " << pricer.rho(callOption) << "\n\n";

    std::cout << "=== Put Option ===\n";
    std::cout << "Price: " << pricer.price(putOption) << "\n";
    std::cout << "Delta: " << pricer.delta(putOption) << "\n";
    std::cout << "Gamma: " << pricer.gamma(putOption) << "\n";
    std::cout << "Vega : " << pricer.vega(putOption) << "\n";
    std::cout << "Theta: " << pricer.theta(putOption) << "\n";
    std::cout << "Rho  : " << pricer.rho(putOption) << "\n";

    return 0;
}
