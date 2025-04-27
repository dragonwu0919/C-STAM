#include "forecastor.hpp"
#include <vector>

#define RISK_AVERSION 0.5f


typedef struct prediction {
    double price_term;
    double div_term;
    double constant;
} prediction_coeff_t;

typedef class agent: public forecastor {
// agent is a class that inherits from forecastor
// It is used to manage the agent's state and actions
private: 
    double risk_aversion = RISK_AVERSION;
    double rate;

public:
    size_t chooseForecastor();
    prediction_coeff_t getPrediction();
    void doEvolution();

    //constructor
    agent() = delete;
    agent(size_t amount, double rate) : forecastor(amount), rate(rate){
    }

} agent_t;

