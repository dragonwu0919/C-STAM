#include <forecastor.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<uint16_t>);


PYBIND11_MODULE(forecastor, m) {
     pybind11::bind_vector<std::vector<double>>(m, "VectorDouble");
     pybind11::bind_vector<std::vector<uint16_t>>(m, "VectorUInt16");


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
          py::arg("condition"), py::arg("index"))
     .def_readwrite("price", &forecastor::price, "Current price of the forecastor")
     .def_readwrite("dividend", &forecastor::dividend, "Current dividend of the forecastor")
     .def_readwrite("last_price", &forecastor::last_price, "Last price of the forecastor")
     .def_readwrite("last_dividend", &forecastor::last_dividend, "Last dividend of the forecastor")
     .def_readwrite("alpha", &forecastor::alpha)
     .def_readwrite("beta", &forecastor::beta)
     .def_readwrite("variance", &forecastor::variance)
     .def_readwrite("condition", &forecastor::condition)
     .def_readwrite("condition_any", &forecastor::condition_any);
}