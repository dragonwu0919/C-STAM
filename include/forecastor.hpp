#include <vector>
#include <cstdint>
#include <cstddef>

#define ALPHA_INIT 1.0f
#define BETA_INIT 0.0f
#define TIME_CONST 75


typedef class forecastor {

protected:
    std::vector<double> alpha;
    std::vector<double> beta;
    std::vector<double> variance;
    std::vector<uint16_t> condition;
    
    size_t amount;
    uint16_t ref_condition = 0xFF;

    int price = 0;
    int dividend = 0;
    int last_price = 0;
    int last_dividend = 0;

public:
    void updateVariance(size_t);
    double getVariance(size_t);
    bool verifyCondition(size_t);
    void setCondition(uint16_t condition);
    size_t getAmount();
    void setValues(int price, int dividend, int last_price, int last_dividend);

    // constructor
    forecastor() = delete;
    forecastor(size_t amount) : amount(amount), alpha(amount, ALPHA_INIT), beta(amount, BETA_INIT), variance(amount, 0.0f), condition(amount, 0xFF) 
    {}

} forecastor;