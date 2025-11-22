// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CorePy/Color.h>

#include <ftk/Core/Path.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace ftk
{
    void path(py::module_& m)
    {
        m.def("isDotFile", &isDotFile);
        m.def("split", [](const std::string& path) { return split(std::filesystem::u8path(path)); });
        m.def("getDrives", []
            {
                std::vector<std::string> out;
                for (const auto& i : getDrives())
                {
                    out.push_back(i.u8string());
                }
                return out;
            });

        py::enum_<UserPath>(m, "UserPath")
            .value("Home", UserPath::Home)
            .value("Desktop", UserPath::Desktop)
            .value("Documents", UserPath::Documents)
            .value("Downloads", UserPath::Downloads);
        m.def("getUserPath", [](UserPath value) { return getUserPath(value).u8string(); });

        m.def(
            "toString",
            py::overload_cast<int64_t, int>(&toString),
            py::arg("frame"),
            py::arg("pad") = 0);

        py::class_<PathOptions>(m, "PathOptions")
            .def_readwrite("seqNegative", &PathOptions::seqNegative)
            .def_readwrite("seqMaxDigits", &PathOptions::seqMaxDigits)
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self != pybind11::self);

        py::class_<Path>(m, "Path")
            .def(py::init<>())
            .def(py::init<std::string>())
            .def(py::init<std::string, std::string>())
            .def("get", &Path::get)
            .def_property_readonly("isEmpty", &Path::isEmpty)
            .def_property_readonly("hasProtocol", &Path::hasProtocol)
            .def_property_readonly("hasDir", &Path::hasDir)
            .def_property_readonly("hasBase", &Path::hasBase)
            .def_property_readonly("hasNum", &Path::hasNum)
            .def_property_readonly("hasExt", &Path::hasExt)
            .def_property_readonly("hasRequest", &Path::hasRequest)
            .def_property("protocol", &Path::getProtocol, &Path::setProtocol)
            .def_property("dir", &Path::getDir, &Path::setDir)
            .def_property("base", &Path::getBase, &Path::setBase)
            .def_property("num", &Path::getNum, &Path::setNum)
            .def_property("pad", &Path::getPad, &Path::setPad)
            .def_property("ext", &Path::getExt, &Path::setExt)
            .def_property("request", &Path::getRequest, &Path::setRequest)
            .def_property(
                "fileName",
                [](const Path& path) { return path.getFileName(); },
                &Path::setFileName)
            .def(
                "getFileName",
                [](const Path& path, bool dir) { return path.getFileName(dir); })
            .def_property("frames", &Path::getFrames, &Path::setFrames)
            .def("getFrame", &Path::getFrame, py::arg("frame"), py::arg("dir") = false)
            .def("getFrameRange", &Path::getFrameRange)
            .def("seq", &Path::seq, py::arg("path"))
            .def("addSeq", &Path::addSeq, py::arg("path"))
            .def_property_readonly("isAbs", &Path::isAbs)
            .def("testExt", &Path::testExt)
            .def_readonly_static("numbers", &Path::numbers)
            .def_readonly_static("pathSeparators", &Path::pathSeparators)
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self != pybind11::self);

        py::enum_<DirListSort>(m, "DirListSort")
            .value("Name", DirListSort::Name)
            .value("Extension", DirListSort::Extension)
            .value("Size", DirListSort::Size)
            .value("Time", DirListSort::Time);

        py::class_<DirListOptions>(m, "DirListOptions")
            .def_readwrite("sort", &DirListOptions::sort)
            .def_readwrite("sortReverse", &DirListOptions::sortReverse)
            .def_readwrite("filter", &DirListOptions::filter)
            .def_readwrite("filterFiles", &DirListOptions::filterFiles)
            .def_readwrite("filterExt", &DirListOptions::filterExt)
            .def_readwrite("seq", &DirListOptions::seq)
            .def_readwrite("seqNegative", &DirListOptions::seqNegative)
            .def_readwrite("seqMaxDigits", &DirListOptions::seqMaxDigits)
            .def_readwrite("hidden", &DirListOptions::hidden)
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self != pybind11::self);

        py::class_<DirEntry>(m, "DirEntry")
            .def_readwrite("path", &DirEntry::path)
            .def_readwrite("isDir", &DirEntry::isDir)
            .def_readwrite("size", &DirEntry::size)
            .def_readwrite("time", &DirEntry::time)
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self != pybind11::self);

        m.def(
            "dirList",
            [](const std::string& path, const DirListOptions& options)
            {
                return dirList(std::filesystem::u8path(path), options);
            },
            py::arg("path"),
            py::arg("options") = DirListOptions());

        m.def(
            "expandSeq",
            &expandSeq,
            py::arg("inPath"),
            py::arg("outPath"),
            py::arg("pathOptions") = PathOptions());
    }
}
