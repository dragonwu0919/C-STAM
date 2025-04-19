CXX = g++

CPP_DIR = src
HPP_DIR = include
BUILD_DIR = build
WRAPPER_DIR = wrapper


WRAP_FLAGS = -O3 -Wall -std=c++11 -fPIC
PYBIND11_INCLUDES = $(shell python3 -m pybind11 --includes)

TARGETS = forecastor agent market
BINARIES = $(addprefix $(BUILD_DIR)/, $(addsuffix .so, $(TARGETS)))


all: binary

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
	fi

	@if ! python -m pip show pytest > /dev/null 2>&1; then \
        echo "pytest not found. Installing..."; \
        python -m pip install pytest; \
    fi

binary: env $(BINARIES)

clean:
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi

# 泛化的模式規則，用於生成 .so 文件
$(BUILD_DIR)/%.so: $(BUILD_DIR)/%.o $(BUILD_DIR)/%_wrapper.o
	@$(CXX) -shared $^ -o $@

# 各個.o 文件的生成規則
$(BUILD_DIR)/%.o: $(CPP_DIR)/%.cpp $(HPP_DIR)/%.hpp
	$(CXX) -c $(WRAP_FLAGS) $(PYBIND11_INCLUDES) $< -o $@

$(BUILD_DIR)/%_wrapper.o: $(WRAPPER_DIR)/%_wrapper.cpp # for wrapper
	$(CXX) -c $(WRAP_FLAGS) $(PYBIND11_INCLUDES) $< -o $@


