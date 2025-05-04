#include <agent.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(agent, m) {

    // 綁定 agent 類
    py::class_<agent_t, forecastor>(m, "agent")
        .def(py::init<size_t, double>())  // 綁定構造函數
        .def("chooseForecastor", &agent_t::chooseForecastor)
        .def("doPrediction", [](agent &self){
            self.getPrediction();}) 
        .def("doEvolution", &agent_t::doEvolution)
        .def_property("price", 
            [](agent_t &self) { return self.pred_coeff.price_term; },
            [](agent_t &self, int price) { self.pred_coeff.price_term = price; })
        .def_property("Amount",
            [](agent_t &self) { return self.getAmount(); }, [](){});
}