#include "include/model_market.hpp"
#include <random>

// void iterateDividend();
// void clearMarket();


void market::clearMarket() {
    double price_para = 0;
    double dividend_para = 0;
    double offset = 0;


    for(auto & agent : agents) {
        static demand_para d;
        d = agent.demand();
        price_para += d.price_para;
        dividend_para += d.dividend_para;
        offset += d.offset;
    }

    last_price = price;
    price = (stock - dividend_para * dividend - offset) / price_para;
    price_history.push_back(price);
}

void market::iterateDividend() {
    static double weight = 0.95;
    last_dividend = dividend;
    dividend = diviend_avg + weight * (last_dividend - diviend_avg) + gassain();
}


double market::gassain() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<> dist(0.0, dividend_variance);

    return dist(gen);
}

void market::step(){
    iterateDividend();

    checkCondition();

    for(auto & agent: agents){
        agent.evaluate(condition);
    }

    clearMarket();
}

double market::getMA(size_t period) {
    if(price_history.size() < period) {
        double sum = 0.0;
        for(size_t i = 0; i < price_history.size(); i++) {
            sum += price_history[i];
        }
        return sum / price_history.size();
    }

    double sum = 0.0;
    for(size_t i = price_history.size() - period; i < price_history.size(); i++) {
        sum += price_history[i];
    }

    return sum / period;
}

void market::checkCondition() {
    condition = 0;

    if(price * rate / dividend > 0.25) {
        condition |= 0x01 < 0;
    }

    if(price * rate / dividend > 0.5) {
        condition |= 0x01 < 1;
    }   

    if(price * rate / dividend > 0.75) {
        condition |= 0x01 < 2;
    }

    if(price * rate / dividend > 0.875 ) {
        condition |= 0x01 < 3;
    }

    if(price * rate / dividend > 1.0) {
        condition |= 0x01 < 4;
    }

    if(price * rate / dividend > 1.125) {
        condition |= 0x01 < 5;
    }

    if(price > getMA(5)){
        condition |= 0x01 < 6;
    }

    if(price > getMA(10)){
        condition |= 0x01 < 7;
    }

    if(price > getMA(100)){
        condition |= 0x01 < 8;
    }

    if(price > getMA(500)){
        condition |= 0x01 < 9;
    }

    condition |= 0x01 < 10;
    condition |= 0x01 < 11;
}