#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>

#include <arrow/python/pyarrow.h>
// we might get duplicate decl error if we also include this
// #include <arrow/array/array_primitive.h>
// #include <arrow/type_fwd.h>
#include <arrow/array/builder_primitive.h>

#include "caster.hpp"

namespace py = pybind11;
using DoubleArray = arrow::DoubleArray;
using Array = arrow::Array;

double sum(std::shared_ptr<arrow::DoubleArray> a) {
    double sum = 0;
    for(int i = 0; i < a->length(); i++) {
        sum += a->Value(i);
    }
    return sum;
}

std::shared_ptr<arrow::DoubleArray> madd(std::shared_ptr<arrow::DoubleArray>& a,
                                        std::shared_ptr<arrow::DoubleArray>& b,
                                        std::shared_ptr<arrow::DoubleArray>& c) {
    if ((a->length() != b->length()) || (a->length() != c->length())) {
        throw std::length_error("Arrays are not of equal length");
    }
    arrow::DoubleBuilder builder;
    arrow::Status status = builder.Resize(a->length());
    if (!status.ok()) {
        throw std::bad_alloc();
    }
    for(int i = 0; i < a->length(); i++) {
        builder.UnsafeAppend(a->Value(i) + b->Value(i) * c->Value(i));
    }
    std::shared_ptr<arrow::DoubleArray> array;
    arrow::Status st = builder.Finish(&array);
    return array;
}

PYBIND11_MODULE(ext, m) {
    arrow::py::import_pyarrow();
    m.doc() = "vaex-arrow extension";
    m.def("madd", &madd, py::call_guard<py::gil_scoped_release>());
    m.def("sum", &sum, py::call_guard<py::gil_scoped_release>());
}
