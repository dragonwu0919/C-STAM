import pytest
from build import forecastor as f
from build import agent as a

def reference():
    tar = a.agent(10,0.1)
    return tar


def test_module_availability():
    tar = a.agent(10,0.1)
    assert True == True

def test_fileterForecater():
    tar = reference()

    print(tar.Amount)

    tar.chooseForecastor()

    assert True == True