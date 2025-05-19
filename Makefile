VERBOSE ?= 0
DEBUG ?= 0


STDERR = /dev/null
ifeq ($(VERBOSE), 1)
    STDERR = /dev/stderr
endif

CXX = g++

CPP_DIR = src
HPP_DIR = include
BUILD_DIR = build
WRAPPER_DIR = wrapper
TEST_DIR = test

WRAP_FLAGS = -Werror -Wall -Wextra -std=c++11 -fPIC
PYBIND11_INCLUDES = $(shell python3 -m pybind11 --includes)
LOCAL_INCLUDES = -I$(CPP_DIR) -I$(HPP_DIR) -I$(WRAPPER_DIR)

TARGETS = forecastor agent market
BINARIES = $(addprefix $(BUILD_DIR)/, $(addsuffix .so, $(TARGETS)))

ifeq ($(DEBUG), 1)
	WRAP_FLAGS += -g 
else
	WRAP_FLAGS += -O3
endif

all: binary

verbose:
	$(MAKE) VERBOSE=1 


debug:
	$(MAKE) DEBUG=1 VERBOSE=1


env:
	@if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	@if [ -z "$$VIRTUAL_ENV" ]; then \
		echo "Error: You are not in a Python virtual environment (venv). Please activate a venv and try again."; \
		exit 1; \
	else \
		echo "Python virtual environment detected: $$VIRTUAL_ENV"; \
	fi

	@if ! python -m pip show pybind11 > /dev/null 2>&1; then \
		echo "pybind11 not found. Installing..."; \
		python -m pip install pybind11; \
	else \
		echo "pybind11 is already installed."; \
	fi

	@if ! python -m pip show pytest > /dev/null 2>&1; then \
		echo "pytest not found. Installing..."; \
		python -m pip install pytest; \
	else \
		echo "pytest is already installed."; \
	fi

binary: env $(BINARIES)

clean:
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
	@if [ -d $(TEST_DIR)/__pycache__ ]; then rm -rf $(TEST_DIR)/__pycache__; fi


# specified target
$(BUILD_DIR)/agent.so: $(BUILD_DIR)/agent.o $(BUILD_DIR)/agent_wrapper.o $(BUILD_DIR)/forecastor.o
	@$(CXX) -shared $^ -o $@


$(BUILD_DIR)/market.so: $(BUILD_DIR)/agent.o $(BUILD_DIR)/market_wrapper.o $(BUILD_DIR)/market.o $(BUILD_DIR)/forecastor.o
	@$(CXX) -shared $^ -o $@


# 泛化的模式規則，用於生成 .so 文件
$(BUILD_DIR)/%.so: $(BUILD_DIR)/%.o $(BUILD_DIR)/%_wrapper.o
	@$(CXX) -shared $^ -o $@

# 各個.o 文件的生成規則
$(BUILD_DIR)/%.o: $(CPP_DIR)/%.cpp $(HPP_DIR)/%.hpp
	@$(CXX) -c $(WRAP_FLAGS) $(PYBIND11_INCLUDES) $< -o $@ $(LOCAL_INCLUDES) > /dev/null 2> $(STDERR)

$(BUILD_DIR)/%_wrapper.o: $(WRAPPER_DIR)/%_wrapper.cpp # for wrapper
	@$(CXX) -c $(WRAP_FLAGS) $(PYBIND11_INCLUDES) $< -o $@ $(LOCAL_INCLUDES) > /dev/null 2> $(STDERR)


test: verbose 
	@echo "Running tests..."
	@python3 -m pytest -xvs test/

