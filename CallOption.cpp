#include "CallOption.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

CallOption::CallOption(double volatility, double interest_rate, double spot_price,
                       double strike_price, double maturity, int steps) {
    this->volatility     = volatility;
    this->interest_rate  = interest_rate;
    this->spot_price     = spot_price;
    this->strike_price   = strike_price;
    this->maturity       = maturity;
    this->steps          = steps;
    this->call_price     = nullptr;
    this->stock_price    = nullptr;
}

CallOption::~CallOption() {
    delete[] call_price;
    delete[] stock_price;
}

double CallOption::standardNormalCDF(double x) {
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));
}

void CallOption::computation() {
    double dt = maturity / steps;
    this->stock_price[0] = spot_price;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> normal{0.0, 1.0};

    for (int i = 0; i < steps; i++) {
        double t   = i * dt;
        double tau = maturity - t;

        double epsilon   = normal(gen);
        double drift     = (interest_rate - 0.5 * volatility * volatility) * dt;
        double diffusion = volatility * std::sqrt(dt) * epsilon;
        stock_price[i+1] = stock_price[i] * std::exp(drift + diffusion);

        if (tau > 1e-10) {
            double d1 = (std::log(stock_price[i+1] / strike_price) + (interest_rate + 0.5 * volatility * volatility) * tau) / (volatility * std::sqrt(tau));
            double d2 = d1 - volatility * std::sqrt(tau);
            call_price[i] = stock_price[i+1] * standardNormalCDF(d1) - std::exp(-interest_rate * tau) * strike_price * standardNormalCDF(d2);
        } else {
            call_price[i] = std::max(stock_price[i+1] - strike_price, 0.0);
        }
    }
}

void CallOption::simulation() {
    const int M = 10000;
    double sum_payoff = 0.0;

    this->call_price  = new double[steps];
    this->stock_price = new double[steps + 1];

    for (int m = 0; m < M; m++) {
        computation();
        sum_payoff += std::max(stock_price[steps] - strike_price, 0.0);
    }

    double mc_price = std::exp(-interest_rate * maturity) * sum_payoff / M;
    std::cout << "Monte Carlo price: " << mc_price << std::endl;
    
}