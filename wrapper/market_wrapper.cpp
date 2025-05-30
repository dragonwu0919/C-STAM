#include <forecastor.hpp>
#include <market.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>  

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<agent_t>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);

PYBIND11_MODULE(market, m) {
     py::bind_vector<std::vector<agent_t>>(m, "AgentList");
     py::bind_vector<std::vector<double>>(m, "VectorDouble");
    
     py::class_<market>(m, "market")
        .def(py::init<size_t, size_t>(), "Initialize forecastor with a specific amount of forecastors", 
             py::arg("agent_amount"), py::arg("forecastor_amount"))
        .def("updateDividend", &market::updateDividend, "Update the dividend of the market")
        .def("updateCondition", &market::updateCondition, "Update the condition of the market")
        .def("getCondition", &market::getCondition, "Get the condition of the market")
        .def("updatePrice", &market::updatePrice, "Update the price of the market")
        .def("updateAgent", &market::updateAgent, "Update the agent of the market")
        .def("getDividend", &market::getDividend, "Get the current dividend of the market")
        .def("getLastDividend", &market::getLastDividend, "Get the last dividend of the market")
        .def("gaussain", &market::gaussain)
        .def("getInterestRate", &market::getInterestRate)
        .def("getPrice", &market::getPrice)
        .def("putDividend", &market::putDividend)
        .def("putPrice", &market::putPrice)
        .def("informAgent", &market::informAgent)
        .def("setAgentVariance", &market::setAgentVariance, "Set the variance of the agent", py::arg("index"), py::arg("forecastor_index"), py::arg("value"))
        .def_readwrite("agents", &market::agents)
        .def("forward", &market::forward, "Forward the market state")
        .def_readwrite("price", &market::price, "Price history of the market")
        .def_readwrite("dividend", &market::dividend, "Dividend history of the market");
}