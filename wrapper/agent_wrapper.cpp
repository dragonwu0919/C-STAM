#include <agent.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(agent, m) {

    // 綁定 agent 類
    py::class_<agent_t>(m, "agent")
        .def(py::init<size_t, double>())  // 綁定構造函數
        .def("chooseForecastor", &agent_t::chooseForecastor)
        .def("getPrediction", &agent_t::getPrediction) 
        .def("mutate", &agent_t::doMutation)
        .def("crossover", &agent_t::doCrossover)
        .def("setVariance", [](agent &self, size_t index, double value){ self.fset.setVariance(index, value);})
        .def_property("amount",
            [](agent_t &self) { return self.fset.getAmount(); }, [](){})
        .def_property_readonly("forecastor", [](agent & self)-> forecastor&
            { return self.fset;}, py::return_value_policy::reference_internal)
        .def("setValues", &agent_t::setValues,
            py::arg("price"), py::arg("dividend"),
            py::arg("last_price"), py::arg("last_dividend"));


    // 綁定 prediction_coeff_t 結構
    py::class_<prediction_coeff_t>(m, "prediction_coeff_t")
        .def(py::init<>())
        .def_readwrite("price_term", &prediction_coeff_t::price_term)
        .def_readwrite("div_term", &prediction_coeff_t::div_term)
        .def_readwrite("constant", &prediction_coeff_t::constant);
}