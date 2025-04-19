#include "../include/agent.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(agent, m) {
    py::class_<agent_t>(m, "Agent")
        .def(py::init<size_t, double>())
        .def("chooseForecastor", &agent_t::chooseForecastor)
        .def("getPrediction", &agent_t::getPrediction)
        .def("doEvolution", &agent_t::doEvolution);
}