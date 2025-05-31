import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))

import forecastor as f
import agent as a
import market as m

import matplotlib.pyplot as plt

def simulate_market():
    # 初始化 market
    tar = m.market(agent_amount=5, forecastor_amount=10)

    for i in range(len(tar.agents)):
        tar.agents[i].forecastor.refactorAlpha(1, 50)
        tar.agents[i].forecastor.refactorBeta(.001, tar.getInterestRate())

    # 設定初始股息
    tar.putDividend(1.0)

    # 模擬 100 步的市場演進
    steps = 100
    agent_alpha = {j: [[] for _ in range(len(tar.agents[j].forecastor.alpha))] for j in range(len(tar.agents))}
    agent_beta = {j: [[] for _ in range(len(tar.agents[j].forecastor.beta))] for j in range(len(tar.agents))}
    agent_wealth = {j: [] for j in range(len(tar.agents))}  # 初始化 wealth

    for _ in range(steps):
        tar.forward()  # 執行 forward 方法
        for j in range(len(tar.agents)):
            agent_wealth[j].append(tar.agents[j].wealth)  # 採集 wealth
            for k in range(len(tar.agents[j].forecastor.alpha)):
                agent_alpha[j][k].append(tar.agents[j].forecastor.alpha[k])  # 採集 alpha
            for k in range(len(tar.agents[j].forecastor.beta)):
                agent_beta[j][k].append(tar.agents[j].forecastor.beta[k])    # 採集 beta

    prices = tar.price  # 直接使用 price history
    return prices, agent_alpha, agent_beta, agent_wealth

def visualize_prices_and_wealth(prices, agent_wealth):
    # 可視化價格和 wealth 演進
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
    plt.show(block=False)  # 非阻塞模式

def visualize_agent_alpha_beta(agent_alpha, agent_beta):
    # 統一顯示所有代理的 alpha 和 beta 演進
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
    plt.show(block=False)  # 非阻塞模式

if __name__ == "__main__":
    # 模擬市場並可視化
    prices, agent_alpha, agent_beta, agent_wealth = simulate_market()
    visualize_prices_and_wealth(prices, agent_wealth)  # 顯示價格和 wealth 演進
    visualize_agent_alpha_beta(agent_alpha, agent_beta)  # 顯示所有代理的 alpha 和 beta 演進

    # 保持視窗開啟
    input("Press Enter to close all plots...")