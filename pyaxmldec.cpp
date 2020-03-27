#include <pybind11/pybind11.h>
#include "axmldec.hpp"

class AXMLDec
{
    public:
        AXMLDec() {}
        ~AXMLDec() {}

        std::string process(char const* input, size_t size)
        {
            return process_bytes(input, size);
        }
};


namespace py = pybind11;

PYBIND11_MODULE(pyaxmldec, m) {
    // optional module docstring
    m.doc() = "pybind11 pyaxmldec plugin";

    // bindings to AXMLDec class
    py::class_<AXMLDec>(m, "AXMLDec")
        .def(py::init<>())
        .def("process", &AXMLDec::process);
}
