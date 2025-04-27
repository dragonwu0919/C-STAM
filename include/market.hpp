#include <agent.hpp>

#define PERSISTENCE 0.95f
#define DIV_AVG 0.0f
#define DIV_VAR 0.0f
#define INTREST_RATE 0.05f
#define STOCK 1000.0f

typedef class market {
    std::vector<agent_t> agents;
    std::vector<double> price;
    std::vector<double> dividend;

    double const persistence = PERSISTENCE;
    double const dividend_avg = DIV_AVG;
    double const dividend_variance = DIV_VAR;
    double const interest_rate = INTREST_RATE;
    double const stock = STOCK;

    double gaussain();
    double getMA(size_t period);

public:
    void updateDividend();
    void updateCondition();
    void updatePrice();
    void updateAgent();


    market() = delete;
    market(size_t agent_amount, size_t forecastor_amount) : agents(agent_amount, agent_t(forecastor_amount, interest_rate)), price(), dividend() {
        price.reserve(1000);
        dividend.reserve(1000);
        price.push_back(0.0);
        dividend.push_back(dividend_avg);
    }
} market_t;