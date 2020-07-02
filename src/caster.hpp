#pragma once

// #include <arrow/array/array_primitive.h>

namespace pybind11 { namespace detail {
    template <typename ArrayType> struct gen_type_caster {
    public:
        // this doesn't work: PYBIND11_TYPE_CASTER(std::shared_ptr<ArrayType>, _(ArrayType::TypeClass::type_name()));
        PYBIND11_TYPE_CASTER(std::shared_ptr<ArrayType>, _("pyarrow::Array"));
        // Python -> C++
        bool load(handle src, bool) {
            PyObject *source = src.ptr();
            if (!arrow::py::is_array(source))
                return false;
            arrow::Result<std::shared_ptr<arrow::Array>> result = arrow::py::unwrap_array(source);
            if(!result.ok())
                return false;
            value = std::static_pointer_cast<ArrayType>(result.ValueOrDie());
            return true;
        }
        // C++ -> Python
        static handle cast(std::shared_ptr<ArrayType> src, return_value_policy /* policy */, handle /* parent */) {
            return arrow::py::wrap_array(src);
        }
    };
    template <>
    struct type_caster<std::shared_ptr<arrow::DoubleArray>> : public gen_type_caster<arrow::DoubleArray> {
    };
}} // namespace pybind11::detail
