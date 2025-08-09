# OptionPricer

A simple yet extendable C++ option pricing library, starting with the Black–Scholes model.  

## 📌 Features (current)
- European call/put pricing via closed-form Black–Scholes formula
- Configurable spot price, strike, maturity, interest rate, and volatility
- Modular design (`Option`, `BlackScholesPricer`, `Utils`) for easy extension

## 🚧 Upcoming
- Greeks (Delta, Gamma, Vega, Theta, Rho)
- Monte Carlo simulation pricer
- PDE solver for option pricing
- Unit tests with GoogleTest
- Support for other option types (Barrier, Asian, American approximation)
- And so on ..

## 🛠 Build Instructions

### Using CMake
```bash
# Create build directory
mkdir -p build && cd build

# Configure & build
cmake ..
make

# Run
./pricer
```

## 💡 Motivation
My personal project as part of my Financial Engineering/Quantitative Finance learning journey, bridging my software engineering background with financial engineering concepts.

It serves both as a learning exercise and as a demonstration of my ability to design robust, extendable pricing tools.