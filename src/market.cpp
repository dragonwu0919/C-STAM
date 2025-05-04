#include <market.hpp>
#include <random>



double market_t::gaussain() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<> dist(0.0, dividend_variance);

    return dist(gen);
}

void market::updateDividend() {
    static double temp = 0.0;

    temp = dividend_avg + persistence * (dividend.back() - dividend_avg) + gaussain();
    dividend.push_back(temp);
}


double market::getMA(size_t period) {
    if (price.size() < period) {
        double sum = 0.0;
        for (size_t i = 0; i < price.size(); i++) {
            sum += price[i];
        }
        return sum / price.size();
    } else {
        double sum = 0.0;
        for (size_t i = price.size() - period; i < price.size(); i++) {
            sum += price[i];
        }
        return sum / period;
    }
}

void market::updateCondition() {
    static uint16_t flag = 0;
    flag = 0;

    if(price.back() * interest_rate / dividend.back() > 0.25) flag |= 0x01 << 0;
    if(price.back() * interest_rate / dividend.back() > 0.50) flag |= 0x01 << 1;
    if(price.back() * interest_rate / dividend.back() > 0.75) flag |= 0x01 << 2;
    if(price.back() * interest_rate / dividend.back() > 0.875) flag |= 0x01 << 3;
    if(price.back() * interest_rate / dividend.back() > 1.0) flag |= 0x01 << 4;
    if(price.back() * interest_rate / dividend.back() > 1.125) flag |= 0x01 << 5;
    if(price.back() > getMA(5)) flag |= 0x01 << 6;
    if(price.back() > getMA(10)) flag |= 0x01 << 7;
    if(price.back() > getMA(100)) flag |= 0x01 << 8;
    if(price.back() > getMA(500)) flag |= 0x01 << 9;

    flag |= 0x01 << 10;
    flag |= 0x00 << 11;
}

void market::updatePrice() {
    double price_term = 0.0;
    double dividend_term = 0.0;
    double constant = 0.0;
    prediction_coeff_t coeff = {0};

    for (size_t i = 0; i < agents.size(); i++) {
        coeff = agents[i].getPrediction();
        price_term += coeff.price_term;
        dividend_term += coeff.div_term;
        constant += coeff.constant;
    }

    double price = 0.0;
    price = (stock - dividend_term*dividend.back() - constant) / (price_term);

    this->price.push_back(price);
}

