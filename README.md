# Option pricing simulator
A Monte Carlo simulator for European call options written in modern C++ using object-oriented principles.

## What it does

Simulates 10,000 stock price paths using geometric Brownian motion and estimates the call option price as the discounted average payoff at maturity. The Black-Scholes price at each step along the path is also computed and saved for future scope such as delta hedging.


## Parameters

| Parameter | Default | Description |
|---|---|---|
| volatility | 0.20 | Annual volatility (σ) |
| interest_rate | 0.05 | Risk-free rate (r) |
| spot_price | 100.0 | Current stock price (S) |
| strike_price | 110.0 | Strike price (K) |
| maturity | 1.0 | Time to maturity in years (T) |
| steps | 252 | Number of time steps (trading days) |


## Methodology

Following a risk-neutral formulation, the governing equation of the stock price $$dS_t = r S_t \, dt + \sigma S_t \, dW_t$$  is discretised as 

$$S_{t+\Delta t} = S_t \cdot \exp\left[\left(r - \frac{\sigma^2}{2}\right)\Delta t + \sigma\sqrt{\Delta t}\varepsilon\right], \quad \varepsilon \sim \mathcal{N}(0,1)$$

and the Monte Carlo estimate of the option price at maturity T is given by 

$$\hat{C}(T) = e^{-rT} \cdot \frac{1}{M} \sum_{i=1}^{M} \max(S_T^{(i)} - K,\ 0)$$

In addition, the call price using the Black-Scholes model is computed at each time step as 

$$C(t_i) = S_{t_i} \cdot \Phi(d_1) - K e^{-r\tau} \cdot \Phi(d_2), \quad $$

where $\Phi$ is the standard normal cumulative distribution function and $\tau = T - t_i$ is the time to maturity with

$$d_1 = \frac{\ln(S/K) + \left(r + \frac{\sigma^2}{2}\right)\tau}{\sigma\sqrt{\tau}}, \qquad d_2 = d_1 - \sigma\sqrt{\tau}$$

## Build

```
g++ -o pricer main.cpp CallOption.cpp
```

## Run

```
./pricer
```
