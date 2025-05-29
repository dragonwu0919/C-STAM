import pytest 
from build import forecastor as f

def get_forecastor():
    tar = f.forecastor(10)

    return tar


def test_init():
    tar = f.forecastor(10)
    
    assert tar.getAmount() == 10

    for i in range(10):
        assert tar.getVariance(i) == pytest.approx(1.0, rel=0.01)



def test_condition():
    tar = get_forecastor()

    for i in range(10):
        assert tar.verifyConditionMask(i) == True

    tar.setCondition(0x00)
    
    for i in range(10):
        tar.setConditionMaskAny(0x0F, i)
        tar.setConditionMask(0xF0, i)
        assert tar.verifyConditionMask(i) == False

        tar.setConditionMask(0x0F, i)
        assert tar.verifyConditionMask(i) == True



    