#include <forecastor.hpp>
#include <random>

void forecastor::setCondition(uint16_t condition) {
    this->ref_condition = condition;
}

void forecastor::setConditionMaskAny(uint16_t mask, size_t index) {
    if (index >= this->amount) {
        return; // need to replace with exception later on
    }
    
    this->condition_any[index] = mask;
}

void forecastor::setConditionMask(uint16_t condition, size_t index) {
    if (index >= this->amount) {
        return; // need to replace with exception later on
    }
    
    this->condition[index] = condition;
}

size_t forecastor::getAmount() {
    return this->amount;
}

double forecastor::getVariance(size_t index) {
    if (index >= this->amount) {
        return -1 ; // need to replace with exception later on
    }
    
    return this->variance[index];
}


bool forecastor::verifyConditionMask(size_t index) {
    static uint16_t filtered_ref = 0;
    static uint16_t filtered_condition = 0;

    filtered_ref = this->ref_condition & ~(this->condition_any[index]);
    filtered_condition = this->condition[index] & ~(this->condition_any[index]);

    return (filtered_ref == filtered_condition);
}


void forecastor::setVariance(size_t index, double value) {
    if (index >= this->amount) {
        return; // need to replace with exception later on
    }
    
    this->variance[index] = value;
}

void forecastor::refactorAlpha(double variance,double offset) {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(- variance, variance);

    this->alpha.resize(this->amount);
    for (size_t i = 0; i < this->amount; i++) {
        this->alpha[i] = dis(gen) + offset;
    }
}

void forecastor::refactorBeta(double variance,double offset) {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis( - variance, variance);

    this->alpha.resize(this->amount);
    for (size_t i = 0; i < this->amount; i++) {
        this->beta[i] = dis(gen) + offset;
    }
}