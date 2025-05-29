import pytest
from build import forecastor as f
from build import agent as a
from build import market as m

import random

def reference():
    tar = a.agent(10,0.1)
    coeff = a.prediction_coeff_t()

    value = [100, 10, 110, 12]
    tar.setValues(value[0], value[1], value[2], value[3])
    
    return tar, coeff


def test_mutation():
    tar, g = reference()
    ref, g = reference()

    for i in range(tar.amount):
        tar.alpha[i] = i +1
        ref.alpha[i] = i +1
        tar.beta[i] = i +1
        ref.beta[i] = i +1
    
    tar.mutate(1.0, 1.0)

    for i in range(tar.amount):
        assert ref.alpha[i] != tar.alpha[i]
        assert ref.beta[i] != tar.beta[i]


def test_crossover():
    tar, g = reference()
    ref, g = reference()

    for i in range(tar.amount):
        temp = random.randint(0, 0xFFFF)
        tar.condition[i] = temp
        ref.condition[i] = temp
        tar.condition_any[i] = temp
        ref.condition_any[i] = temp

    tar.crossover(1.0,1.0)

    for i in range(tar.amount):
        assert ref.condition[i] != tar.condition[i]
        assert ref.condition_any[i] != tar.condition_any[i]