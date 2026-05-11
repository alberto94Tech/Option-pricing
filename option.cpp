#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>

class CallOption{

    private:

    double volatility, interest_rate, spot_price, strike_price;
    double maturity;
    double* call_price;
    double* stock_price;
    int steps;




    public:
    CallOption(double volatility, double interest_rate, double spot_price, double strike_price, double maturity, int steps){

        this->volatility=volatility;
        this->interest_rate=interest_rate;
        this->spot_price=spot_price;
        this->strike_price=strike_price;  

        this->call_price=nullptr;  
        this->stock_price=nullptr;

        this->steps=steps;
        this->maturity=maturity;


       
    }

    ~CallOption(){
        std::cout<<"hello from destructor"<<std::endl;
        delete[] call_price;
        delete[] stock_price;
        return;
        
    }
    
    static double standardNormalCDF(double x) {
    return 0.5 * (1 + std::erf(x / std::sqrt(2.0)));


}
    void computation(){   
        
        

        double dt=maturity/steps;

        this->stock_price[0]=spot_price;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution <double> normal{0.0, 1.0};


        //std::cout<<steps<<std::endl;
        std::cout<<dt <<std::endl;
        for(int i=0;i<steps;i++){
            double t=i*dt; //current time
            double tau=maturity-t; //time remaining to maturity

            stock_price[i+1]=stock_price[i]*std::exp((interest_rate-0.5*volatility*volatility)*dt+(volatility*std::sqrt(dt)*normal(gen)));
            std::cout<<stock_price[i]<<std::endl;

            if(tau>1e-10){

                double d1 = (std::log(stock_price[i+1]/strike_price)+(interest_rate + 0.5*volatility*volatility)*(tau))/(volatility*std::sqrt(tau));
                double d2 = d1-volatility*std::sqrt(tau); 
                call_price[i]= stock_price[i+1]*standardNormalCDF(d1)-std::exp(-interest_rate*(tau))*strike_price*standardNormalCDF(d2); 
            }
            else{
                call_price[i]= std::max(stock_price[i+1] - strike_price, 0.0);

            }
            
            
        }
    }

    void simulation(){

        const int M = 10000;
        double sum_payoff=0.0;
        double average_price;
        this->call_price = new double [steps];
        this->stock_price = new double [steps+1];


        for(int i=0;i<M;i++){
            computation();
            sum_payoff+=std::max(stock_price[steps]-strike_price, 0.0);

        }

        average_price = std::exp(-interest_rate * maturity) * sum_payoff / M;
        std::cout << "Monte Carlo price: " << average_price << std::endl;
    }
   
        
        

     
};

int main(){

    
    CallOption call(0.2, 0.05, 100.0, 110.0, 1.0, 252);

    call.simulation();

    

    return 0;
}