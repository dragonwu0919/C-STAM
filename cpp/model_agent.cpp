#include "include/model_agent.hpp"
#include <iostream>
#include <cmath>


void agent::evaluate(uint16_t const & condition) {
    static std::vector<bool> flag(predictors.size(), false);


    // Here we just print a message for demonstration purposes
    std::cout << "Evaluating predictors..." << std::endl;

    // generate canditate predictors based on the current condition

    for(auto & predictor : predictors) {
        if(predictor.condition & condition != condition) {
            flag[&predictor - &predictors[0]] = false;
        }else{
            flag[&predictor - &predictors[0]] = true;
            // evalute the accuracy of the candidate predictors 
            predictor.variance = (1.0f-1/tao)*predictor.variance + (1/tao)*std::pow(((*price-*div)-(predictor.forecaster[0]*(*last_price+*last_div)+predictor.forecaster[1])), 2);
        }
    }

    // choose the best predictor based on the evaluation
    current_predictor = nullptr;
    for(auto & predictor : predictors) {
        if(flag[&predictor - &predictors[0]]) {        
            if(predictor.variance < current_predictor->variance || current_predictor == nullptr) {
                current_predictor = &predictor;
            }
        }
    }
}

void agent::evolve() {
    // Here we just print a message for demonstration purposes
    std::cout << "Evolving predictors..." << std::endl;
}


demand_para const & agent::demand() {
    static demand_para d;
    d.price_para = (current_predictor->forecaster[0]-1-*rate)/(risk_aversion*current_predictor->variance);
    d.dividend_para = current_predictor->forecaster[0]/(risk_aversion*current_predictor->variance);
    d.offset = current_predictor->forecaster[1];

    return d;
}