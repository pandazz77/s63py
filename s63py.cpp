#include "s63py_wrappers.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(s63py, m){
    py::exception<S63exception>(m,"S63Exception");

    m.def("validateCellPermit",&S63WR::validateCellPermit,
        py::arg("permit"),
        py::arg("HW_ID")
    );
    m.def("createUserPermit",&S63WR::createUserPermit,
        py::arg("M_KEY"),
        py::arg("HW_ID"),
        py::arg("M_ID")
    );
    m.def("extractHwIdFromUserpermit",&S63WR::extractHwIdFromUserpermit,
        py::arg("userpermit"),
        py::arg("M_KEY")
    );
    m.def("createCellPermit",&S63WR::createCellPermit,
        py::arg("HW_ID"),
        py::arg("CK1"),
        py::arg("CK2"),
        py::arg("cellname"),
        py::arg("expiry_date")
    );
    m.def("extractCellKeysFromCellpermit",&S63WR::extractCellKeysFromCellpermit,
        py::arg("cellpermit"),
        py::arg("HW_ID")
    );
    m.def("decryptCell",&S63WR::decryptCell,
        py::arg("path"),
        py::arg("keys")
    );
    m.def("encryptCell",&S63WR::encryptCell,
        py::arg("buf"),
        py::arg("key")
    );
    m.def("decryptAndUnzipCellByKey",&S63WR::decryptAndUnzipCellByKey,
        py::arg("in_path"),
        py::arg("keys"),
        py::arg("out_path")
    );

    py::class_<S63WR::S63Client_WRAPPED>(m,"S63Client")
    .def(py::init<const py::bytearray&,const py::bytearray&,const py::bytearray&>(),
        py::arg("HW_ID"),
        py::arg("M_KEY"),
        py::arg("M_ID")
    )
    .def("getHWID",&S63WR::S63Client_WRAPPED::getHWID)
    .def("getMID",&S63WR::S63Client_WRAPPED::getMID)
    .def("getMKEY",&S63WR::S63Client_WRAPPED::getMKEY)
    .def("setHWID",&S63WR::S63Client_WRAPPED::setHWID,
        py::arg("HW_ID")
    )
    .def("setMID",&S63WR::S63Client_WRAPPED::setMID,
        py::arg("M_ID")    
    )
    .def("setMKEY",&S63WR::S63Client_WRAPPED::setMKEY,
        py::arg("M_KEY")
    )
    .def("installCellPermit",&S63WR::S63Client_WRAPPED::installCellPermit,
        py::arg("cellPermit")
    )
    .def("importPermitFile",&S63WR::S63Client_WRAPPED::importPermitFile,
        py::arg("permitFile")
    )
    .def("getUserpermit",&S63WR::S63Client_WRAPPED::getUserpermit)
    .def("open",&S63WR::S63Client_WRAPPED::open,
        py::arg("s63cellpath")
    )
    .def("decryptAndUnzipCell",py::overload_cast<const std::string&, const std::string&>(&S63WR::S63Client_WRAPPED::decryptAndUnzipCell),
        py::arg("in_path"),
        py::arg("out_path")
    )
    .def("decryptAndUnzipCell",py::overload_cast<const std::string&, const std::string&, const std::string&>(&S63WR::S63Client_WRAPPED::decryptAndUnzipCell),
        py::arg("in_path"),
        py::arg("cellPermit"),
        py::arg("out_path")
    );
};