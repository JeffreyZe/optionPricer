# OptionPricer

A simple yet extendable C++ option pricing library, starting with the Black–Scholes model.  

## 🚀 Features
- European call/put pricing via closed-form Black–Scholes formula, with continuous dividend yield support
- Configurable parameters: spot price, strike, maturity, interest rate, volatility, dividentYield
- Calculation of Greeks (divident-aware): Delta, Gamma, Vega, Theta, Rho
- Implied Vol calculation using Newton–Raphson, with dividend support
- Modular design (`Option`, `BlackScholesPricer`, `Utils`) for easy extension
- Unit tests powered by GoogleTest

## 🧭 Roadmap
- Monte Carlo simulation pricer
- PDE solver for option pricing
- Support for exotic options (Barrier, Asian, American approximation)
- Performance optimizations
- Documentation improvements
- And so on ..

## 🛠️ Build & Test Instructions

### Dev Dependencies
```bash
brew install cmake
brew install googletest
```

### Build & Run
```bash
# Create build directory
mkdir -p build && cd build

# Configure & build
cmake ..
make

# Run the pricer
./pricer

# Run unit tests
ctest
```

## 💡 Motivation
My personal project as part of my Financial Engineering/Quantitative Finance learning journey, bridging my software engineering background with financial engineering concepts.
