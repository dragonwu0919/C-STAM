import pytest 
from build import forecastor as f

def get_forecastor():
    values = [100, 10, 110, 12]

    tar = f.forecastor(10)

    tar.setValues(values[0], values[1], values[2], values[3])

    return tar, values


def update_variance(forecastor, i, price, div, past_price, past_div):
    alpha = 1.0
    beta = 0 
    tau = 75
    var = forecastor.getVariance(i)


    past = ( 1- 1/tau) * var
    error = (1/tau) * pow(price + div - (alpha*(past_price + past_div) + beta), 2)

    return past + error



def test_init():
    tar = f.forecastor(10)
    
    assert tar.getAmount() == 10

    for i in range(10):
        assert tar.getVariance(i) == pytest.approx(1.0, rel=0.01)

def test_variance():
    [tar, values] = get_forecastor()

    for i in range(10):
        assert tar.getVariance(i) == pytest.approx(1.0, rel=0.01)

    for i in range(10):
        ref = update_variance(tar, i, values[0], values[1], values[2], values[3])
        tar.updateVariance(i)


        assert tar.getVariance(i) == pytest.approx(ref, rel=0.01)

def test_condition():
    [tar, values] = get_forecastor()

    for i in range(10):
        assert tar.verifyConditionMask(i) == True

    tar.setCondition(0x00)
    
    for i in range(10):
        tar.setConditionMaskAny(0x0F, i)
        tar.setConditionMask(0xF0, i)
        assert tar.verifyConditionMask(i) == False

        tar.setConditionMask(0x0F, i)
        assert tar.verifyConditionMask(i) == True



    