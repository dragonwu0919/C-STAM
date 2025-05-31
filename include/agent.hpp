#pragma once

#include <forecastor.hpp>
#include <vector>

constexpr double RISK_AVERSION = 0.5f;
constexpr double INIT_SAVING = 1000.0f; // Initial wealth for the agent
constexpr int TIME_CONST = 75;


struct prediction_coeff_t {
    double price_term;
    double div_term;
    double constant;
} ;

class agent{
public: 

    forecastor fset; // the forecastor used by the agent

    double risk_aversion = RISK_AVERSION;
    double rate;

    double stock_hold = 0; // Amount of stock held by the agent
    double wealth ;
    double saving = INIT_SAVING; // Initial saving amount

    double price = 0;
    double dividend = 0;
    double last_price = 0;
    double last_dividend = 0;

    prediction_coeff_t pred_coeff = {0.0, 0.0, 0.0};    
    size_t chooseForecastor();
    prediction_coeff_t getPrediction();
    void doMutation(double replace_ratio, double mutation_ratio);
    void doCrossover(double replace_ratio, double crossover_ratio);

    double calculateWealth(double r, double s, double h, double p, double d){
        return (1+r) * s + h * p + h * d;
    }

    double updateWealth() {
        // update stock hold 
        // Since h(t) = a*p(t) + b*d(t) + c, and p(t) is solved as new price,  we can use the new price and dividend to calculate the stock hold
        stock_hold = pred_coeff.price_term * price + pred_coeff.div_term * last_dividend + pred_coeff.constant;
        saving = wealth - stock_hold * price; // update saving
        
        wealth = calculateWealth(rate, saving, stock_hold, price, dividend);
        return wealth;
    }

    double updateVariance(size_t index){
        static double ratio = 1.0f / TIME_CONST;
        static double past_term;
        static double error_term;

        past_term = this->fset.variance[index] * (1 - ratio);
        
        error_term = (this->price + this->dividend) - this->fset.alpha[index]* (this->last_price + this->last_dividend) - this->fset.beta[index];
        error_term = error_term * error_term;
        error_term = error_term * ratio;

        this->fset.variance[index] = past_term + error_term;
        if (this->fset.variance[index] < 0.0f) {
            this->fset.variance[index] = 0.0f;
        }

        return past_term + error_term;
    }

    void setValues(double p, double d, double lp, double ld){
        price = p;
        dividend = d;
        last_price = lp;
        last_dividend = ld;
    };

    //constructor
    agent() = delete;
    agent(size_t amount, double rate) : fset(amount), rate(rate){
        wealth = calculateWealth(rate, saving, stock_hold, 0.0f, 0.0f);
    }

};

typedef agent agent_t;
