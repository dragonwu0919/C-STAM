import pytest
from build import forecastor as f
from build import agent as a
from build import market as m

import random

def reference():
    tar = a.agent(10,0.1)
    coeff = a.prediction_coeff_t()

    value = [100, 10, 110, 12]
    tar.forecastor.setValues(value[0], value[1], value[2], value[3])
    
    return tar, coeff


def test_mutation():
    tar, g = reference()
    ref, g = reference()

    for i in range(tar.amount):
        tar.forecastor.alpha[i] = i +1
        ref.forecastor.alpha[i] = i +1
        tar.forecastor.beta[i] = i +1
        ref.forecastor.beta[i] = i +1
    
    tar.mutate(1.0, 1.0)

    for i in range(tar.amount):
        assert ref.forecastor.alpha[i] != tar.forecastor.alpha[i]
        assert ref.forecastor.beta[i] != tar.forecastor.beta[i]


def test_crossover():
    tar, g = reference()
    ref, g = reference()

    for i in range(tar.forecastor.amount):
        temp = random.randint(0, 0xFFFF)
        tar.forecastor.condition[i] = temp
        ref.forecastor.condition[i] = temp
        tar.forecastor.condition_any[i] = temp
        ref.forecastor.condition_any[i] = temp

    tar.crossover(1.0,1.0)

    for i in range(tar.forecastor.amount):
        assert ref.forecastor.condition[i] != tar.forecastor.condition[i]
        assert ref.forecastor.condition_any[i] != tar.forecastor.condition_any[i]