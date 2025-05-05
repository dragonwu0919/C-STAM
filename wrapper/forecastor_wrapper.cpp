#include <forecastor.hpp>

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(forecastor, m) {
    py::class_<forecastor>(m, "forecastor")
        .def(py::init<size_t>(), "Initialize forecastor with a specific amount of forecastors", 
             py::arg("amount"))
        .def("setValues", &forecastor::setValues, "set values for price, dividend, last_price, last_dividend", 
             py::arg("price"), py::arg("dividend"), py::arg("last_price"), py::arg("last_dividend"))
        .def("setCondition", &forecastor::setCondition, "set condition for forecastor", 
             py::arg("condition"))
        .def("getAmount", &forecastor::getAmount, "return the amount of forecastors")
        .def("getVariance", &forecastor::getVariance, "get the variance for a specific forecastor", 
             py::arg("index"))
        .def("verifyConditionMask", &forecastor::verifyConditionMask, "verify the condition for a specific forecastor, return true if the condition is met", 
             py::arg("index"))
        .def("updateVariance", &forecastor::updateVariance, "update the variance for a specific forecastor", 
             py::arg("index"))
        .def("setConditionMaskAny", &forecastor::setConditionMaskAny, "set condition for forecastor with any condition",
             py::arg("condition_any"), py::arg("index"))
        .def("setConditionMask", &forecastor::setConditionMask, "set condition for forecastor with specific condition",
             py::arg("condition"), py::arg("index"));
}