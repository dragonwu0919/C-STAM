#include "../include/agent.hpp"

#include <unordered_map>

size_t agent_t::chooseForecastor() {
    size_t best_index = 0;
    double best_value = 0.0;
    double value = -1.0;

    for (size_t i = 0; i < getAmount(); ++i) {
        if (verifyCondition(i)) {
            if((value == -1.0) || (getVariance(i) < best_value)) {
                best_index = i;
                best_value = getVariance(i);
            }
        }
    }
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

    return coeff;
}

void agent_t::doEvolution() {
    return;
}
