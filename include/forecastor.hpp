#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

constexpr double ALPHA_INIT = 10.0;
constexpr double BETA_INIT = 0.0;

class forecastor {

public:
    size_t amount;
    uint16_t ref_condition = 0xFF;

    std::vector<double> alpha;
    std::vector<double> beta;
    std::vector<double> variance;
    std::vector<uint16_t> condition;
    std::vector<uint16_t> condition_any;
    
    bool verifyConditionMask(size_t);

    // getter and setters
    double getVariance(size_t);
    void setVariance(size_t index , double value);

    void setCondition(uint16_t condition);
    void setConditionMask(uint16_t condition, size_t index);
    void setConditionMaskAny(uint16_t condition_any, size_t index);
    
    size_t getAmount();

    // constructor
    forecastor() = delete;
    forecastor(size_t amount) : amount(amount), alpha(amount, ALPHA_INIT), beta(amount, BETA_INIT), variance(amount, 1.0f), condition(amount, 0xFF), condition_any(amount, 0xFF)
    {}

};

