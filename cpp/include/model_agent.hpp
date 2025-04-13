#include <vector>
#include <cstdint>


typedef struct predictor {
    uint16_t condition; 
    double forecaster[2]; // One forecaster requires three parameters
    double variance;

    // constructor
    predictor() {
        condition = 0xFF;
        forecaster[0] = 0.0f;
        forecaster[1] = 0.0f;
        variance = 0.0f;
    }
} predictor;

typedef struct demand_para {
    double price_para;
    double dividend_para;
    double offset;
} demand_para;



typedef struct agent {
    std::vector<predictor> predictors;
    predictor* current_predictor;
    double* div;
    double* price;
    double* rate;
    double* last_div;
    double* last_price;

    double const tao = 75.0;
    double const risk_aversion = 0.5f;

    // constructor
    agent() = delete;
    agent(size_t predictor_count) 
        : predictors(predictor_count), 
          current_predictor(nullptr), 
          div(nullptr), 
          price(nullptr), 
          rate(nullptr), 
          last_div(nullptr), 
          last_price(nullptr)
        {}

    // used by agent itself
    void evaluate(uint16_t const & condition);   
    void evolve();                       
    
    // provide for class market
    demand_para const & demand(); // to support market clearing
} agent ;

