#include <agent.hpp>

#include <unordered_map>

#include <stdio.h>
#include <iostream>

size_t agent_t::chooseForecastor() {
    size_t best_index = 0;
    double best_value = -1.0;
    size_t bound = this->getAmount();


    for (size_t i = 0; i < bound; i++) {
        
        if (this->verifyConditionMask(i)) {
            if((best_value == -1.0) || (this->getVariance(i) < best_value)) {
                best_index = i;
                best_value = this->getVariance(i);
            }
        }
    }

    return best_index;
}

prediction_coeff_t agent_t::getPrediction() {
    static prediction_coeff_t coeff;
    size_t index = 0;
    static double alpha = 0;
    static double beta = 0;
    static double variance = 0; 

    index = chooseForecastor();
    alpha = this->alpha[index];
    beta = this->beta[index];
    variance = this->variance[index];

    coeff.price_term = (alpha - 1 - rate) / (risk_aversion * variance);
    coeff.div_term = (alpha) / (risk_aversion * variance);
    coeff.constant = beta;

    pred_coeff = coeff;

    return coeff;
}

void agent_t::doEvolution() {
    return;
}
