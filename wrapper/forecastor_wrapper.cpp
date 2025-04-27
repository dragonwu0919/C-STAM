#include "../include/forecastor.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(forecastor, m) {
    py::class_<forecastor>(m, "Forecastor")
        .def(py::init<size_t>())
        .def("setValues", &forecastor::setValues)
        .def("setCondition", &forecastor::setCondition)
        .def("getAmount", &forecastor::getAmount)
        .def("getVariance", &forecastor::getVariance)
        .def("verifyCondition", &forecastor::verifyCondition)
        .def("updateVariance", &forecastor::updateVariance);
}