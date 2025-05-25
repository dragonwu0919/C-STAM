#include <agent.hpp>

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <random>



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

    // find the best forecastors
    constexpr double replace_ratio = 0.3;
    constexpr double mutation_ratio = 0.1;
    constexpr double mutaion_magnitude = 0.1;
    constexpr double crossover_ratio = 0.5;
    


    size_t elected_amount = this->getAmount() * replace_ratio / 1;
    size_t replace_amount = this->getAmount() * replace_ratio / 1;

    std::vector<std::pair<double, size_t>> value_index;
    std::vector<std::pair<double, size_t>> best_forecastors;
    
    value_index.reserve(this->getAmount());
    best_forecastors.reserve(elected_amount);

    for (size_t i = 0; i < this->getAmount(); i++) {
        value_index.emplace_back(this->getVariance(i), i);
    }

    std::sort(value_index.begin(), value_index.end());

    for(auto & pair : value_index) {
        best_forecastors.emplace_back(pair);
        if (best_forecastors.size() >= elected_amount) {
            break;
        }
    }
    
    // do mutation
    // collect the parameters of the best forecastors
    std::vector<double> alpha_params;
    std::vector<double> beta_params;
    std::vector<uint16_t> condition_params;
    std::vector<uint16_t> condition_mask_params;

    for(auto & pair : best_forecastors) {
        size_t index = pair.second;
        alpha_params.push_back(this->alpha[index]);
        beta_params.push_back(this->beta[index]);
        condition_params.push_back(this->condition[index]);
        condition_mask_params.push_back(this->condition_any[index]);
    }
    // do mutation on the parameters

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1.0 - mutaion_magnitude, 1.0 + mutaion_magnitude);
    std::uniform_real_distribution<> mutation_chance(0.0, 1.0);

    for (size_t i = 0; i < alpha_params.size(); i++) {
        if (mutation_chance(gen) < mutation_ratio) { 
            alpha_params[i] *= dist(gen); 
            beta_params[i] *= dist(gen);  
        }
    }
    
    // do crossover

    size_t crossover_amount = alpha_params.size() * crossover_ratio / 1;
    for(size_t i = 0; i < crossover_amount; i++) {
        size_t index1 = std::uniform_int_distribution<>(0, alpha_params.size() - 1)(gen);
        size_t index2 = std::uniform_int_distribution<>(0, alpha_params.size() - 1)(gen);

        if (index1 != index2) {
            double alpha_temp = (alpha_params[index1] + alpha_params[index2]) / 2.0;
            double beta_temp = (beta_params[index1] + beta_params[index2]) / 2.0;

            uint16_t condition_temp = (condition_params[index1] & 0xFF00) | (condition_params[index2] & 0x00FF);
            uint16_t condition_mask_temp = (condition_mask_params[index1] & 0xFF00) | (condition_mask_params[index2] & 0x00FF);


            alpha_params[index1] = alpha_temp;
            alpha_params[index2] = alpha_temp;
            beta_params[index1] = beta_temp;
            beta_params[index2] = beta_temp;
            condition_params[index1] = condition_temp;
            condition_params[index2] = condition_temp;
            condition_mask_params[index1] = condition_mask_temp;
            condition_mask_params[index2] = condition_mask_temp;
        }
    }   



    // do generalization
    // pass, since i don't implement the code to record the last time each
    // forecastor was used
    
    // replace the worst forecastors with the new ones

    for (size_t i = 0; i < replace_amount; i++) {
        size_t worst_index = value_index[value_index.size() - 1 - i].second;

        this->alpha[worst_index] = alpha_params[i];
        this->beta[worst_index] = beta_params[i];
        this->condition[worst_index] = condition_params[i];
        this->condition_any[worst_index] = condition_mask_params[i];
    }
    
}
