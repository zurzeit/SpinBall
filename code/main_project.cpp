#ifndef DEBUG
    #define DEBUG 0
#endif

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include "spinball_cls.hpp"

#if DEBUG==0
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#endif



struct baseball{
    double m = 145/1000.0;//g
    double r = 73/2.0/1000.0;//m
    
};
struct curveball : baseball{
    double w = 3000/60.0*2*M_PI;//rpm avg 
    double vx = 0*1.609*1000/3600;
    double x = 0, y = 0, z = 1.7, vy = 0, vz = 0;//mph
    std::vector<double> init_state{x,vx,y,vy,z,vz};
};
int main()
{

    /*
        w:omega, 
        theta: the angle with x-axis in the x-y plane, 
        phi: the angle with x-axis in the x-z plane
    */
    struct curveball ball;
    double rho = 1.204; // kg/m3

    // double w_element = sqrt(w*w/3.0);
    std::vector<double> init_state, omega(3,0);
    init_state = ball.init_state;
    omega.at(2) = ball.w;
    std::vector<std::vector<double>> ans_states;

    Spinball_cls func_cls(ball.m, ball.r, rho);
    func_cls.print_states();
    double total_sec = 1, time_inc = 0.1;
    ans_states = func_cls.RK4_main(total_sec, time_inc, init_state, omega);
    func_cls.print_states();
    omega.at(1) = ball.w/2;
    ans_states = func_cls.RK4_main(total_sec*5, time_inc*2, init_state, omega);
    func_cls.print_states();

// std::cout << rho*ball.vx*2*ball.r/1.819*100000;
    return 0;
}


#if DEBUG==0
PYBIND11_MODULE(_spinball, m) {
    m.doc() = "This is a class spinball.";
    py::class_<States>(m, "States")
        .def(py::init<>())
        .def("print_states", &States::print_states)
        // .def_property_readonly("Done_timestamp", &States::Done_timestamp)
        .def_property_readonly("states", &States::get_states)
        .def_property_readonly("Total_timestamp", &States::get_Total_timestamp)
        .def_property_readonly("Done_timestamp", &States::get_Done_timestamp)
        .def_property_readonly("Final_timestamp", &States::get_Final_timestamp)
        .def_property_readonly("Num_state", &States::get_Num_state)
        ;
    py::class_<Spinball_cls, States>(m, "Spinball")
        // .def(py::init([](double m, double r, double rho, double max_x_distance){ return new Spinball_cls(m, r, rho, max_x_distance);}))
        .def(py::init<double, double , double , double>(), py::arg("m"), py::arg("r"), py::arg("rho"), py::arg("max_x_distance")=std::numeric_limits<double>::infinity())
        .def("RK4_main", &Spinball_cls::RK4_main, py::return_value_policy::copy)
        .def_property_readonly("R", &Spinball_cls::get_R)
        .def_property_readonly("Omega", &Spinball_cls::get_Omega)
        .def_property_readonly("Max_x_distance", &Spinball_cls::get_Max_x_distance)
        
        ;
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