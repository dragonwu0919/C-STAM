#include <forecastor.hpp>
#include <market.hpp>

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(market, m) {
    py::class_<market>(m, "market")
        .def(py::init<size_t, size_t>(), "Initialize forecastor with a specific amount of forecastors", 
             py::arg("agent_amount"), py::arg("forecastor_amount"))
        .def("updateDividend", &market::updateDividend, "Update the dividend of the market")
        .def("updateCondition", &market::updateCondition, "Update the condition of the market")
        .def("updatePrice", &market::updatePrice, "Update the price of the market")
        .def("updateAgent", &market::updateAgent, "Update the agent of the market");
}