import pytest
from build import forecastor as f
from build import agent as a
from build import market as m


def reference():
    tar = m.market(agent_amount=3, forecastor_amount=10)
    tar.putDividend(1.0)
    return tar

def nBINARY(n):
    tar = 0
    for i in range(n):
        tar = tar | (1 << i)
    return tar



def test_updatedividends():
    tar = reference()

    for i in range(10):
        tar.updateDividend()
        assert tar.getDividend() != pytest.approx(tar.getLastDividend(), rel=0.01)                


def test_updateCondition():
    tar = reference()

    rate = tar.getInterestRate()
    ref = 1 << 10 | 0 << 11

    # Test the condition when price is 0
    tar.putPrice(0*rate)
    tar.updateCondition()
    condition = tar.getCondition()
    assert condition == 0 | ref


    # test the first seven conditions
    threshold = [0.25, 0.50, 0.75, 0.875, 1.0, 1.125]
    for t in threshold:
        tar.putPrice((t + 0.01) / rate)
        tar.updateCondition()
        condition = tar.getCondition()
        assert condition & 0b111111 == nBINARY(threshold.index(t) + 1)

    # test the moving average condition
    length = [5, 10, 100, 500]
    for l in length:
        tar = reference()

        for i in range(500):
            tar.putPrice(10.0)
        for i in range(l-1):
            tar.putPrice(0)
        tar.putPrice(1.0)
        
        tar.updateCondition()
        condition = tar.getCondition()
        condition = condition & (0b1111 << 6)
        condition = condition >> 6

        assert condition == nBINARY(length.index(l) + 1)
 

def test_updatePrice():
    tar = reference()

    variance = 1.0
    div = 1.0
    stock = 1000.0
    risk_aversion = 0.5

    for i in range(10):
        for j in range(3):
            tar.setAgentVariance(j,i, variance)
    
    rate = tar.getInterestRate()

    price_term = 0
    div_term = 0
    constant = 0
    for i in range(3):
        price_term += (1.0 -1 - rate) / (variance * risk_aversion)
        div_term += (1.0) / (variance * risk_aversion)
        constant += 0

    tar.putDividend(div)

    ref_price = (stock - div_term * div - constant) / (price_term)
    
    tar.updatePrice()
    print(tar.agents[0].getAmount())
    price = tar.getPrice()

    assert price == pytest.approx(ref_price, rel=0.01)


def test_updateAgent():
    assert True