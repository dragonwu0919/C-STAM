import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "../build"))

import forecastor as f
import agent as a
import market as m

import matplotlib.pyplot as plt  # 修正引入方式

def simulate_market():
    # 初始化 market
    tar = m.market(agent_amount=3, forecastor_amount=10)

    # 設定初始股息
    tar.putDividend(1.0)

    # 模擬 100 步的市場演進
    steps = 1000
    agent_wealth = {j: [] for j in range(len(tar.agents))}

    for _ in range(steps):
        tar.forward()  # 執行 forward 方法
        for j in range(len(tar.agents)):
            agent_wealth[j].append(tar.agents[j].wealth)  # 採集 agent 的 wealth

    return tar.price, agent_wealth

def visualize_prices_and_wealth(prices, agent_wealth):
    # 可視化價格演進
    plt.figure(figsize=(12, 8))

    # Plot price evolution
    plt.subplot(2, 1, 1)
    plt.plot(prices, label="Price Evolution", color="blue")
    plt.xlabel("Time Steps")
    plt.ylabel("Price")
    plt.title("Market Price Evolution")
    plt.legend()
    plt.grid(True)

    # Plot agent wealth evolution
    plt.subplot(2, 1, 2)
    for agent_id, wealth in agent_wealth.items():
        plt.plot(wealth, label=f"Agent {agent_id} Wealth")
    plt.xlabel("Time Steps")
    plt.ylabel("Wealth")
    plt.title("Agent Wealth Evolution")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # 模擬市場並可視化
    prices, agent_wealth = simulate_market()
    visualize_prices_and_wealth(prices, agent_wealth)