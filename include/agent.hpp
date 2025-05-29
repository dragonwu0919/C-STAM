#pragma once

#include <forecastor.hpp>
#include <vector>

constexpr double RISK_AVERSION = 0.5f;

struct prediction_coeff_t {
    double price_term;
    double div_term;
    double constant;
} ;

class agent{
// agent is a class that inherits from forecastor
// It is used to manage the agent's state and actions
public: 
    forecastor fset; // the forecastor used by the agent

    double risk_aversion = RISK_AVERSION;
    double rate;

    double stock_hold;
    double wealth;
    double saving;

    double a_price = 0;
    double a_dividend = 0;
    double a_last_price = 0;
    double a_last_dividend = 0;

    prediction_coeff_t pred_coeff = {0.0, 0.0, 0.0};    
    size_t chooseForecastor();
    prediction_coeff_t getPrediction();
    void doMutation(double replace_ratio, double mutation_ratio);
    void doCrossover(double replace_ratio, double crossover_ratio);

    double updateWealth();

    void setValues(double price, double dividend, double last_price, double last_dividend){
        a_price = price;
        a_dividend = dividend;
        a_last_price = last_price;
        a_last_dividend = last_dividend;
        
        fset.setValues(price, dividend, last_price, last_dividend);
    };

    //constructor
    agent() = delete;
    agent(size_t amount, double rate) : fset(amount), rate(rate){
    }

};

typedef agent agent_t;
