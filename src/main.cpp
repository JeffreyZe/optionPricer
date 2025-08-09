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

    std::cout << "Call Price: " << pricer.price(callOption) << std::endl;
    std::cout << "Put Price: " << pricer.price(putOption) << std::endl;

    return 0;
}
