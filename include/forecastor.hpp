#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

constexpr double ALPHA_INIT = 1.0;
constexpr double BETA_INIT = 0.0;
constexpr int TIME_CONST = 75;

class forecastor {

public:
    size_t amount;
    uint16_t ref_condition = 0xFF;

    std::vector<double> alpha;
    std::vector<double> beta;
    std::vector<double> variance;
    std::vector<uint16_t> condition;
    std::vector<uint16_t> condition_any;

    double price = 0;
    double dividend = 0;
    double last_price = 0;
    double last_dividend = 0;

    void updateVariance(size_t);
    
    bool verifyConditionMask(size_t);

    // getter and setters
    double getVariance(size_t);
    void setVariance(size_t index , double value);

    void setCondition(uint16_t condition);
    void setConditionMask(uint16_t condition, size_t index);
    void setConditionMaskAny(uint16_t condition_any, size_t index);
    
    size_t getAmount();
    
    void setValues(double price, double dividend, double last_price, double last_dividend);
    void setPrice(double);
    void setLastPrice(double);
    void setDividend(double);
    void setLastDividend(double);


    // constructor
    forecastor() = delete;
    forecastor(size_t amount) : amount(amount), alpha(amount, ALPHA_INIT), beta(amount, BETA_INIT), variance(amount, 1.0f), condition(amount, 0xFF), condition_any(amount, 0xFF)
    {}

};

