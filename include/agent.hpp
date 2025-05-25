#pragma once

#include <forecastor.hpp>
#include <vector>

constexpr double RISK_AVERSION = 0.5f;

struct prediction_coeff_t {
    double price_term;
    double div_term;
    double constant;
} ;

class agent: public forecastor {
// agent is a class that inherits from forecastor
// It is used to manage the agent's state and actions
private: 
    double risk_aversion = RISK_AVERSION;
    double rate;

public:
    prediction_coeff_t pred_coeff = {0.0, 0.0, 0.0};    
    size_t chooseForecastor();
    prediction_coeff_t getPrediction();
    void doEvolution();

    //constructor
    agent() = delete;
    agent(size_t amount, double rate) : forecastor(amount), rate(rate){
    }

};

typedef agent agent_t;
