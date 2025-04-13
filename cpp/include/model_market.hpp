#include <vector>
#include <cstdint>

#include "model_agent.hpp"


typedef struct market {
    std::vector<class agent> agents;
    int stock = 1000;
    double rate, price, dividend;
    double last_dividend, last_price;
    double dividend_variance, diviend_avg;
    uint16_t condition;

    std::vector<double> price_history;
    
    // constructor
    market() = delete;
    market(size_t agent_count, size_t predictor_count=10, double rate = 0.1, double dividend_variance = 0.1f, double div_avg = 10, double price = 1000) :  
        rate(rate),price(price), 
        dividend_variance(dividend_variance),
        diviend_avg(div_avg), price_history(3,1000){

        agents.reserve(agent_count);
        for(size_t i = 0; i < agent_count; i++){
            agents.emplace_back(predictor_count); 
        }

        for(auto & agent : agents) {
            agent.div = &dividend;
            agent.price = &price;
            agent.rate = &rate;
            agent.last_div = &last_dividend;
            agent.last_price = &last_price;
        }
    }

    // member functions
    void iterateDividend();
    void clearMarket();
    double gassain(); 
    void checkCondition();
    double getMA(size_t period);
    void step();


} market;

