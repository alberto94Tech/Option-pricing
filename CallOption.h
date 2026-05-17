#pragma once
#include <random>
 
class CallOption {
 
    private:
        double volatility, interest_rate, spot_price, strike_price;
        double maturity;
        double* call_price;
        double* stock_price;
        int steps;
 
        static double standardNormalCDF(double x);
 
    public:
        CallOption(double volatility, double interest_rate, double spot_price,
                   double strike_price, double maturity, int steps);
        ~CallOption();
        void computation();
        void simulation();
};
 