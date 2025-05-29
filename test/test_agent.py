import pytest
from build import forecastor as f
from build import agent as a

def reference():
    tar = a.agent(10,0.1)
    coeff = a.prediction_coeff_t()

    for i in range(10):
        tar.setVariance(i, 1)

    value = [100, 10, 110, 12]
    tar.forecastor.setValues(value[0], value[1], value[2], value[3])
    
    return tar, coeff


def test_module_availability():
    tar = a.agent(10,0.1)
    assert True == True

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

    ref.price_term =  (1.0 -1 - rate) / (risk_aversion * variance)
    ref.div_term = (1.0) / (risk_aversion * variance)
    ref.constant = 0

    coeff = tar.getPrediction()

    assert coeff.price_term == pytest.approx(ref.price_term, rel=0.01)
    assert coeff.div_term == pytest.approx(ref.div_term, rel=0.01)
    assert coeff.constant == pytest.approx(ref.constant, rel=0.01)
