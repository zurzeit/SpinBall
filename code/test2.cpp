#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Pet {
public:
    Pet(const std::string &name) : name(name) { }
    std::string name;
    std::
};

class Dog : public Pet {
public:
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};

PYBIND11_MODULE(_animal, m) {
    py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def_readwrite("name", &Pet::name);

    // Method 1: template parameter:
    py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")
        .def(py::init<const std::string &>())
        .def("bark", &Dog::bark);
}