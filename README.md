# C-STAM

Basic Information
================

C-STAM is an implementation of the Santa Fe Institute Artificial Stock Market 
(SFI-ASM). It is written in C++ for computational efficiency, while Python is 
used to provide a user-friendly API.

GitHub Repository: https://github.com/dragonwu0919/C-STAM

Problem to Solve
================

The Santa Fe Artificial Stock Market (SFI-ASM) is an agent-based computational 
model designed to simulate complex financial market dynamics through 
interactions of adaptive agents. Each agent evolves trading strategies over 
time using a genetic algorithm. To handle the significant computational load 
efficiently, this project implements the SFI-ASM in C++, leveraging its 
computational performance, while providing a convenient Python API for ease of 
use and integration.


System Architecture
===================

The Santa Fe Institute Artificial Stock Market (SFI-ASM) simulates a market 
comprising stocks and a banking asset, in which agents perform stock 
transactions.

- Stock market
  - market clearing
  - dividend
  - price
- agent
  - predictor
    - condition part
    - forecaste part
- Genetic algorithm

```
function step() {
  # Update market condition based on price and dividend history
  updateCondition()

  # Inform agents about the current market state
  informAgent()

  # Calculate the new stock price based on agents' predictions and demand
  updatePrice()

  # Update the dividend for the next step
  updateDividend()

  # Inform agents again with the updated market state
  informAgent()

  # For each agent:
    # Update the variance of the chosen forecaster
    # Update the agent's wealth based on the new market state

  # Perform genetic algorithm operations on agents:
    # Mutation
    # Crossover
}
```

Each agent maintains multiple rules. Each rule contains parameters used by a 
forecaster to estimate future stock prices, and classifiers to determine the 
specific market conditions under which each rule is applicable. Based on these 
conditions, agents calculate their demand for stocks. The genetic algorithm 
iteratively evolves these rules by selecting and modifying them according to 
their performance.

There are two kind of asset in this model: stock and bank. Both of them grant
agent additional interest at the end of each step. The difference is that
dividend iterates with a random number, as banking provide a risk-free interest
rate.

After every agent provide its demand, the model need to do market clearing. To
acheive this, the agents' demand functions are deliberately designed as linear
functions of the stock price, which enables straightforward market clearing
through adjustments to ensure stock supply and demand equilibrium.

The pseudo-code mentioned above will be implemented in the C++ layer, which may
look like the following:

- class forecastor
  - uint16_t ref_condition
    - Reference condition used for forecasting.
  - std::vector<double> alpha
    - Alpha parameters for forecasting.
  - std::vector<double> beta
    - Beta parameters for forecasting.
  - std::vector<double> variance
    - Variance values for each forecaster.
  - std::vector<uint16_t> condition
    - Specific market conditions for each forecaster.
  - std::vector<uint16_t> condition_any
    - General market conditions for each forecaster.
  - void refactorAlpha(double variance, double offset)
    - Refactor alpha parameters based on variance and offset.
  - void refactorBeta(double variance, double offset)
    - Refactor beta parameters based on variance and offset.

- class agent
  - forecastor fset
    - The forecastor used by the agent.
  - double risk_aversion
    - Risk aversion parameter for the agent.
  - double stock_hold
    - Amount of stock held by the agent.
  - double wealth
    - Total wealth of the agent.
  - double saving
    - Savings of the agent.
  - prediction_coeff_t pred_coeff
    - Coefficients used for predicting stock price and dividend.
  - void updateWealth()
    - Update the agent's wealth based on the current market state.
  - void updateVariance(size_t index)
    - Update the variance of the chosen forecaster.
  - void doMutation(double replace_ratio, double mutation_ratio)
    - Perform mutation on the agent's forecasting rules.
  - void doCrossover(double replace_ratio, double crossover_ratio)
    - Perform crossover on the agent's forecasting rules.

- class market
  - std::vector<agent_t> agents
    - List of agents participating in the market.
  - std::vector<double> price
    - History of stock prices.
  - std::vector<double> dividend
    - History of dividends.
  - double interest_rate
    - Risk-free interest rate for banking assets.
  - void updateCondition()
    - Update market conditions based on price and dividend history.
  - void updatePrice()
    - Calculate the new stock price based on agents' predictions and demand.
  - void updateDividend()
    - Update the dividend for the next step.
  - void informAgent()
    - Inform agents about the current market state.
  - void forward()
    - Execute one step of market evolution, including updating conditions, prices, dividends, and agents' states.

API Description
===============

- `setup`: Initializes the system.
  - Initializes the market with a specified number of agents and forecasters.
  - Example:
    ```python
    market = m.market(agent_amount=5, forecastor_amount=10)
    ```

- `step`: Executes a round of trading.
  - Advances the market state by one step, including updating conditions, prices, dividends, and agents' states.
  - Example:
    ```python
    market.forward()
    ```

- `show`: Retrieve the current parameters of the system.
  - Provides access to market parameters such as price history, dividend history, and agent states.
  - Example:
    ```python
    print(market.price)  # Access price history
    print(market.dividend)  # Access dividend history
    print(market.agents[0].wealth)  # Access wealth of the first agent
    ```

- `evolve`: Runs one iteration of the genetic algorithm.
  - Performs mutation and crossover operations on agents' forecasting rules.
  - Example:
    ```python
    for agent in market.agents:
        agent.mutate(replace_ratio=0.1, mutation_ratio=0.05)
        agent.crossover(replace_ratio=0.1, crossover_ratio=0.05)
    ```

- `modify`: Directly modifies a specific parameter of the system.
  - Allows direct modification of market or agent parameters such as variance, wealth, or stock holdings.
  - Example:
    ```python
    market.setAgentVariance(index=0, forecastor_index=1, value=0.2)  # Modify variance
    market.agents[0].wealth = 1000.0  # Modify wealth of the first agent
    market.agents[0].stock_hold = 50.0  # Modify stock holdings of the first agent
    ```

Engineering Infrastructure
==========================

The calculation is implemented in C++ and wrapped using pybind11 to expose a 
Python API. The C++ codebase is compiled with CMake. Unit testing is performed 
using Google Test for the C++ components and pytest for the Python interface.

References
==========
- Building the Santa Fe Artificial Stock Market, Blake LeBaron, Brandeis 
University, June 2002.
- https://github.com/felixschmitz/ArtificialStockMarketReproduction
