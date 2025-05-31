import pytest
from build import forecastor as f
from build import agent as a
import random

def reference():
    tar = a.agent(10,0.1)
    coeff = a.prediction_coeff_t()

    for i in range(10):
        tar.setVariance(i, 1)

    value = [100, 10, 110, 12]
    tar.setValues(value[0], value[1], value[2], value[3])
    
    return tar, coeff

def random_value():
    price = random.uniform(0.0, 100.0)
    div = random.uniform(0.0, 100.0)
    last_price = random.uniform(0.0, 100.0)
    last_div = random.uniform(0.0, 100.0)

    return price, div, last_price, last_div


def test_module_availability():
    tar = a.agent(10,0.1)
    assert True == True

def test_variance(): # not yet complete

    amount = 10
    rate = 0.1
    tar = a.agent(amount, rate)

    for i in range(amount):
        ref_var= tar.forecastor.variance[i]
        alpha = tar.forecastor.alpha[i]
        beta = tar.forecastor.beta[i]
        tau = 75

        
        price = random.uniform(0.0, 100.0)
        div = random.uniform(0.0, 100.0)
        last_price = random.uniform(0.0, 100.0)
        last_div = random.uniform(0.0, 100.0)
        
        tar.setValues(price, div, last_price, last_div)
        tar_var = tar.updateVariance(i)

        error = (price + div) - (alpha*(last_div+last_price) + beta)
        error = error * error
        error = error / (tau)
        ref_var = ref_var * (1 - 1.0/tau) + error

        assert tar_var == pytest.approx(ref_var, rel=0.01)

def test_chooseForecater():
    tar, coeff = reference()

    target = 5
    
    # manipulate variance
    for i in range(10):
        tar.setVariance(i, 1)
    
    tar.setVariance(target, 0.1)
    assert tar.chooseForecastor() == target

    # manipulate condition
    tar, coeff = reference()

    tar.forecastor.setCondition(0xff)

    for i in range(10):
        tar.forecastor.setConditionMaskAny(0x00, i)
        tar.forecastor.condition_any[i] = 0x00
        tar.forecastor.setConditionMask(0x00, i)

    tar.forecastor.setConditionMask(0xff, target)

    assert tar.chooseForecastor() == target

    # manipulate both condition and variance
    tar, coeff = reference()

    tar.forecastor.setCondition(0xff)

    for i in range(10):
        tar.forecastor.setConditionMaskAny(0x0F, i)

    for i in range(3, 8):
        tar.forecastor.setConditionMask(0xF0, i)

    tar.setVariance(target, 0.1)

    assert tar.chooseForecastor() == target


def test_prediction_coeff():
    tar, coeff = reference()

    target = 10

    coeff.price_term = target +1 
    coeff.div_term = target +2
    coeff.constant = target +3

    assert coeff.price_term == target + 1
    assert coeff.div_term == target + 2
    assert coeff.constant == target + 3


def test_prediction():
    tar, coeff = reference()
    
    target = 5
    variance = 0.1
    rate = 0.1
    risk_aversion = 0.5

    tar.forecastor.setCondition(0xff)

    for i in range(10):
        tar.forecastor.setConditionMaskAny(0x0F, i)

    for i in range(3, 8):
        tar.forecastor.setConditionMask(0xF0, i)

    tar.setVariance(target, variance)

    ref = coeff

    ref.price_term =  (10.0 -1 - rate) / (risk_aversion * variance)
    ref.div_term = (10.0) / (risk_aversion * variance)
    ref.constant = 0

    coeff = tar.getPrediction()

    assert coeff.price_term == pytest.approx(ref.price_term, rel=0.01)
    assert coeff.div_term == pytest.approx(ref.div_term, rel=0.01)
    assert coeff.constant == pytest.approx(ref.constant, rel=0.01)

def test_wealth():
    
    def wealth(rate, saving, hold, price, div):
        return (1 +rate) * saving + hold * (price + div)
    
    tar, coeff = reference()

    s = tar.saving
    h = tar.stock_hold
    r = tar.rate

    for i in range(10):
        price, div, last_price, last_div = random_value()
        tar.setValues(price, div, last_price, last_div)

        ref_wealth = wealth(r, s, h, price, div)

        res = tar.calculateWealth(r, s, h, price, div)

        assert res == pytest.approx(ref_wealth, rel=0.01)
