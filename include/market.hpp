#pragma once

#include <agent.hpp>

constexpr double PERSISTENCE = 0.95;
constexpr double DIV_AVG = 0.0;
constexpr double DIV_VAR = 1.0;
constexpr double INTREST_RATE = 0.05;
constexpr double STOCK = 1000.0;
constexpr double INIT_PRICE = 100.0;

class market {
public:
    std::vector<agent_t> agents;
    std::vector<double> price;
    std::vector<double> dividend;

    double const persistence = PERSISTENCE;
    double const dividend_avg = DIV_AVG;
    double const dividend_variance = DIV_VAR;
    double const interest_rate = INTREST_RATE;
    double const stock = STOCK;

    uint16_t condition = 0x00;

    double gaussain();
    double getMA(size_t period);

    void updateDividend();
    void updateCondition();
    uint16_t getCondition();

    void updatePrice();
    void updateAgent();

    double getDividend();
    double getLastDividend();
    void putDividend(double);

    double getInterestRate();

    double getPrice();
    void putPrice(double);

    void informAgent();
    
    void setAgentVariance(size_t, size_t, double );

    void forward();



    market() = delete;
    market(size_t agent_amount, size_t forecastor_amount) : agents(agent_amount, agent_t(forecastor_amount, interest_rate)), price(), dividend() {
        price.reserve(100);
        dividend.reserve(100);
        price.push_back(INIT_PRICE);
        price.push_back(INIT_PRICE);
        dividend.push_back(dividend_avg);
        dividend.push_back(dividend_avg);

        for (size_t i = 0; i < agents.size(); i++) {
            agents[i].setValues(price.back(), dividend.back(), price[price.size() - 2], dividend[dividend.size() - 2]);
            agents[i].fset.setCondition(condition);
        }
    }
};

typedef market market_t;