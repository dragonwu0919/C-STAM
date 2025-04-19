# structure

A draft of the cpp layer of this library.

```
- class forecaster 
    - prediction coefficent: 
        - type: vector of double float
        - size: N*2
    - condition: 
        - type: vector of uint16_t
        - size: N
    - variance:
        - type: vector of double float
        - size: N
    - prices: $(current price), $(current dividend), $(last variance), $(last price), $(last dividend): public
        - type: long int
    - amount
    - updataVariance(), public
        - input: certain predictor to be update  
        - dependancy: time constant, prediction coefficent, $(current price), $(current dividend), $(last variance), $(last price), $(last dividend)
    - getVarinace(), pubilc
        - input: certain predictor
        - output: its variance
    - setCondtion(), public
    - setValues(), public
    - verifyCondition(), public
        - input: certain predictor to be update
        - dependancy: global condition, local condition
        - output: whether the predictor fits global condition or not
    - getAmount(), public
        - output: the amount of hold forecastors 
- class agent
    - chooseForecastor()
        - input: none
        - dependancy: local hold forecasters, current price, current dividend
        - routine: verifyCondition() -> updateVariance() -> store the index and the varinaces of the updated ones -> choose forcastor with lowest variance
        - output: selected forecastor
    - getPrediction()
        - dependancy: chosen forcastor, risk aversion coefficient, interst rate
        - output: 3 linear coefficient of prediction
    - doEvolution()
- class market
    - value history
    - current value
    - updataDividend() 
        - dependency: mean dividend, persistence parameter, variance of dividend, dividend on last step
        - output: the dividend in this step
    - updateCondition()
    - clearMarket() -> updateMArket()
        - dependency: @eachAgent linear coefficient of prediction, current value 
    - updateAgent()

```