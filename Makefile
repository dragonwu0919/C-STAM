CXX = gcc

CPP_DIR = cpp
HPP_DIR = $(CPP_DIR)/include
BUILD_DIR = build

all: model env

env:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi

clean:
	if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi

model: env model_agent.o model_market.o

model_agent.o: $(CPP_DIR)/model_agent.cpp $(HPP_DIR)/model_agent.hpp
	$(CXX) -c $< -o $(BUILD_DIR)/$@

model_market.o: $(CPP_DIR)/model_market.cpp $(HPP_DIR)/model_market.hpp
	$(CXX) -c $< -o $(BUILD_DIR)/$@
