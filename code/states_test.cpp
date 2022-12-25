#include "states.hpp"
#include <vector>

int main(){
    int n = 6;
    std::vector<double> init_state;
    for(size_t i = 0 ; i < n; i ++){
        init_state.push_back(double(i));
    }
    double total_sec = 1.0, time_inc = 0.1;

    States states;
    states.print_states();
    states.states_reset(total_sec, time_inc, init_state);
    states.print_states();
    init_state.pop_back();
    states.states_reset(total_sec*5, time_inc*2, init_state);
    states.print_states();


}

#ifndef DEBUG
    #define DEBUG 0
#endif

#if DEBUG==0
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
namespace py = pybind11;

PYBIND11_MODULE(_states, m) {
    m.doc() = "This is a class states.";
    py::class_<States>(m, "States")
        .def(py::init([](std::vector<double> init_states, double total_sec, double time_inc) { return new States( init_states, total_sec, time_inc); }))
        .def("print_states", &States::print_states)
        ;
    // py::class_<Spinball_cls, States>(m, "Spinball")
    //     .def(py::init([](std::vector<double> init_state, std::vector<double> omega, double total_time, double time_inc, double rho, double r, double m) { return new Spinball_cls(init_state, omega, total_time, time_inc, rho, r, m);; }))
    //     // .def("RK4_main", &Spinball_cls::RK4_main)
    //     .def("print_states", &Spinball_cls::print_time_states)
    //     ;
        // .def(py::init([](size_t nrow, size_t ncol, std::vector<double> const & vec) { return new Matrix(nrow, ncol, vec); }))
    //     
    //     .def("__setitem__", [](Matrix & mat,std::pair<size_t,size_t> key, double val) { mat.setter(key,val); })
    //     .def("__getitem__", [](Matrix & mat,std::pair<size_t,size_t> key) { return mat.getter(key); })
    //     .def_property_readonly("nrow", &Matrix::nrow)
    //     .def_property_readonly("ncol", &Matrix::ncol)
    //     .def_property_readonly("array", [](Matrix &m) -> py::array_t<double>
    //     {
    //         return py::array_t<double>(
    //             { m.nrow(), m.ncol() },                              // Buffer dimensions
    //             { sizeof(double) * m.ncol(), sizeof(double) },       // Strides (in bytes) for each index
    //             m.get_buffer_pointer(),                              // Pointer to buffer
    //             py::cast(m)
    //         );
    //     })
	// .def("__eq__", &Matrix::operator==)
    //     ;
    // m.def("multiply_naive", &multiply_naive, "A function that multiply_naive");
    // m.def("multiply_tile", &multiply_tile, "A function that multiply_tile");
    // m.def("multiply_mkl", &multiply_mkl, "A function that multiply_mkl");
}
#endif