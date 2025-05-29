#include <forecastor.hpp>

void forecastor::setValues(double price, double dividend, double last_price, double last_dividend) {
    this->price = price;
    this->dividend = dividend;
    this->last_price = last_price;
    this->last_dividend = last_dividend;
}

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

void forecastor::setVariance(size_t index, double value) {
    if (index >= this->amount) {
        return; // need to replace with exception later on
    }
    
    this->variance[index] = value;
}

void forecastor::setPrice(double price) {
    this->price = price;
}
void forecastor::setLastPrice(double last_price) {
    this->last_price = last_price;
}  
void forecastor::setDividend(double dividend) {
    this->dividend = dividend;
}
void forecastor::setLastDividend(double last_dividend) {
    this->last_dividend = last_dividend;
}