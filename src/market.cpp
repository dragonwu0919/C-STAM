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

    condition = flag;
}

uint16_t market::getCondition() {
    return condition;
}

void market::updatePrice() {
    double price_term = 0.0;
    double dividend_term = 0.0;
    double constant = 0.0;
    prediction_coeff_t coeff = {0,0,0};

    for (size_t i = 0; i < agents.size(); i++) {
        coeff = agents[i].getPrediction();
        price_term += coeff.price_term;
        dividend_term += coeff.div_term;
        constant += coeff.constant;
    }

    double price_temp = 0.0;
    price_temp = (stock - dividend_term*dividend.back() - constant) / (price_term);

    if(price_temp <0) this->price.push_back(price.back());
    else this->price.push_back(price_temp);
}

void market::updateAgent() {
    for(size_t i = 0; i < agents.size(); i++) {
        agents[i].doMutation(0.2, 1.0);
        agents[i].doCrossover(0.2, 1.0);
    }
    return ;
}


double market::getDividend() {
    return dividend.back();
}

double market::getLastDividend() {
    return *(dividend.end() - 2);
}

double market::getInterestRate(){
    return interest_rate;
}

double market::getPrice(){
    return price.back();
}

void market::putDividend(double t){
    dividend.push_back(t);
}

void market::putPrice(double t){
    price.push_back(t);
}

void market::informAgent() {
    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].setValues(price.back(), dividend.back(), price[price.size() - 2], dividend[dividend.size() - 2]);
        agents[i].fset.setCondition(condition);
    }
}

void market::setAgentVariance(size_t index, size_t forecastor_index, double value) {
    if (index >= agents.size() || forecastor_index >= agents[index].fset.getAmount()) {
        return; // need to replace with exception later on
    }
    
    agents[index].fset.setVariance(forecastor_index, value);
}


void market::forward() {
    updateCondition();
    informAgent();
    updatePrice();
    updateDividend();
    informAgent();

    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].updateVariance(agents[i].chooseForecastor());
        agents[i].updateWealth();
    }

    updateAgent();
}