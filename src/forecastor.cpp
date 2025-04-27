#include "../include/forecastor.hpp"

void forecastor::setValues(int price, int dividend, int last_price, int last_dividend) {
    this->price = price;
    this->dividend = dividend;
    this->last_price = last_price;
    this->last_dividend = last_dividend;
}

void forecastor::setCondition(uint16_t condition) {
    this->ref_condition = condition;
}


size_t forecastor::getAmount() {
    return this->amount;
}

double forecastor::getVariance(size_t index) {
    return this->variance[index];
}


bool forecastor::verifyCondition(size_t index) {
    return (this->condition[index] & this->ref_condition) == this->ref_condition;
}

void forecastor::updateVariance(size_t index) {
    static double ratio = 1.0f / TIME_CONST;
    static double past_term;
    static double error_term;

    past_term = this->variance[index] * (1 - ratio);
    
    error_term = (this->price + this->dividend) - alpha[index]* (this->last_price + this->last_dividend) - beta[index];
    error_term = error_term * error_term;
    error_term = error_term * ratio;

    this->variance[index] = past_term + error_term;
    if (this->variance[index] < 0.0f) {
        this->variance[index] = 0.0f;
    }
}