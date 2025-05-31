import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))

import forecastor as f
import agent as a
import market as m

import matplotlib.pyplot as plt

def simulate_market():
    tar = m.market(agent_amount=5, forecastor_amount=5)

    for i in range(len(tar.agents)):
        tar.agents[i].forecastor.refactorAlpha(50, 100)
        tar.agents[i].forecastor.refactorBeta(.001, tar.getInterestRate())

    tar.putDividend(1.0)

    steps = 1000
    agent_alpha = {j: [[] for _ in range(len(tar.agents[j].forecastor.alpha))] for j in range(len(tar.agents))}
    agent_beta = {j: [[] for _ in range(len(tar.agents[j].forecastor.beta))] for j in range(len(tar.agents))}
    agent_wealth = {j: [] for j in range(len(tar.agents))}  

    print(f"Step {1}/{steps}", end="")
    for _ in range(steps):
        tar.forward()  
        print(f"\rStep {_ + 1}/{steps}", end="")
        for j in range(len(tar.agents)):
            agent_wealth[j].append(tar.agents[j].wealth) 
            for k in range(len(tar.agents[j].forecastor.alpha)):
                agent_alpha[j][k].append(tar.agents[j].forecastor.alpha[k]) 
            for k in range(len(tar.agents[j].forecastor.beta)):
                agent_beta[j][k].append(tar.agents[j].forecastor.beta[k])    

    print("")
    prices = tar.price  
    return prices, agent_alpha, agent_beta, agent_wealth

def visualize_prices_and_wealth(prices, agent_wealth):
    plt.figure(figsize=(12, 8))

    # Plot price evolution
    plt.subplot(2, 1, 1)
    plt.plot(prices, label="Price Evolution", color="blue")
    plt.xlabel("Time Steps")
    plt.ylabel("Price")
    plt.title("Market Price Evolution")
    plt.legend()
    plt.grid(True)

    # Plot wealth evolution for each agent
    plt.subplot(2, 1, 2)
    for agent_id, wealth in agent_wealth.items():
        plt.plot(wealth, label=f"Agent {agent_id} Wealth")
    plt.xlabel("Time Steps")
    plt.ylabel("Wealth")
    plt.title("Agent Wealth Evolution")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show(block=False)

def visualize_agent_alpha_beta(agent_alpha, agent_beta):
    num_agents = len(agent_alpha)
    fig, axes = plt.subplots(num_agents, 2, figsize=(12, 6 * num_agents))

    # Plot alpha and beta evolution for each agent
    for agent_id in range(num_agents):
        # Alpha evolution
        for forecastor_id, alpha in enumerate(agent_alpha[agent_id]):
            axes[agent_id, 0].plot(alpha, label=f"Forecastor {forecastor_id} Alpha")
        axes[agent_id, 0].set_xlabel("Time Steps")
        axes[agent_id, 0].set_ylabel("Alpha")
        axes[agent_id, 0].set_title(f"Agent {agent_id} Alpha Evolution")
        axes[agent_id, 0].legend()
        axes[agent_id, 0].grid(True)

        # Beta evolution
        for forecastor_id, beta in enumerate(agent_beta[agent_id]):
            axes[agent_id, 1].plot(beta, label=f"Forecastor {forecastor_id} Beta")
        axes[agent_id, 1].set_xlabel("Time Steps")
        axes[agent_id, 1].set_ylabel("Beta")
        axes[agent_id, 1].set_title(f"Agent {agent_id} Beta Evolution")
        axes[agent_id, 1].legend()
        axes[agent_id, 1].grid(True)

    plt.tight_layout()
    plt.show(block=False)

if __name__ == "__main__":
    prices, agent_alpha, agent_beta, agent_wealth = simulate_market()
    visualize_prices_and_wealth(prices, agent_wealth)  
    visualize_agent_alpha_beta(agent_alpha, agent_beta)  

    input("Press Enter to close all plots...")