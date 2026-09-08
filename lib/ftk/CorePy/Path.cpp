// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Path.h>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/filesystem.h>

#include <sstream>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void path(nb::module_& m)
        {
            m.def("isDotFile", &isDotFile);
            m.def("split", [](const std::string& path) { return split(toFileSystem(path)); });
            m.def("getDrives", []
                {
                    std::vector<std::string> out;
                    for (const auto& i : getDrives())
                    {
                        out.push_back(fromFileSystem(i));
                    }
                    return out;
                });

            FTK_ENUM_PY(m, UserPath);
            FTK_ENUM_BIND(m, UserPath);

            m.def("getUserPath", [](UserPath value) { return fromFileSystem(getUserPath(value)); });

            m.def(
                "toString",
                nb::overload_cast<int64_t, int>(&toString),
                nb::arg("frame"),
                nb::arg("pad") = 0);

            nb::class_<PathOptions>(m, "PathOptions")
                .def(nb::init<>())
                .def_rw("seqNegative", &PathOptions::seqNegative)
                .def_rw("seqMaxDigits", &PathOptions::seqMaxDigits)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<FrameSeq>(m, "FrameSeq")
                .def(nb::init<>())
                .def(nb::init<RangeI64, int>(), nb::arg("range"), nb::arg("inc") = 1)
                .def(nb::init<int64_t, int64_t, int>(), nb::arg("min"), nb::arg("max"), nb::arg("inc") = 1)
                .def(nb::init<int64_t>(), nb::arg("frame"))
                .def_rw("range", &FrameSeq::range)
                .def_rw("inc", &FrameSeq::inc)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            m.def("toFrameSeq", &toFrameSeq, nb::arg("frames"));
            m.def(
                "toFrames",
                nb::overload_cast<const std::vector<FrameSeq>&>(&toFrames),
                nb::arg("seq"));
            m.def(
                "addFrame",
                [](std::vector<FrameSeq> seq, int64_t frame)
                {
                    addFrame(seq, frame);
                    return seq;
                },
                nb::arg("seq"),
                nb::arg("frame"));
            m.def(
                "getFrameCount",
                nb::overload_cast<const std::vector<FrameSeq>&>(&getFrameCount),
                nb::arg("seq"));
            m.def("getRange", &getRange, nb::arg("seq"));
            m.def(
                "getLabel",
                nb::overload_cast<const std::vector<FrameSeq>&, int>(&getLabel),
                nb::arg("seq"),
                nb::arg("pad") = 0);

            nb::class_<Path>(m, "Path")
                .def(nb::init<>())
                .def(nb::init<std::string>())
                .def(nb::init<std::string, std::string>())
                .def(nb::init<std::string, const PathOptions&>())
                .def("get", &Path::get)
                .def_prop_ro("isEmpty", &Path::isEmpty)
                .def_prop_ro("hasProtocol", &Path::hasProtocol)
                .def_prop_ro("hasSeqWildcard", &Path::hasSeqWildcard)
                .def_prop_ro("hasDir", &Path::hasDir)
                .def_prop_ro("hasBase", &Path::hasBase)
                .def_prop_ro("hasNum", &Path::hasNum)
                .def_prop_ro("hasExt", &Path::hasExt)
                .def_prop_ro("hasRequest", &Path::hasRequest)
                .def_prop_rw("protocol", &Path::getProtocol, &Path::setProtocol)
                .def_prop_rw("dir", &Path::getDir, &Path::setDir)
                .def_prop_rw("base", &Path::getBase, &Path::setBase)
                .def_prop_rw("num", &Path::getNum, &Path::setNum)
                .def_prop_rw("pad", &Path::getPad, &Path::setPad)
                .def_prop_rw("ext", &Path::getExt, &Path::setExt)
                .def_prop_rw("request", &Path::getRequest, &Path::setRequest)
                .def_prop_rw(
                    "fileName",
                    [](const Path& path) { return path.getFileName(); },
                    &Path::setFileName)
                .def(
                    "getFileName",
                    [](const Path& path, bool dir) { return path.getFileName(dir); },
                    nb::arg("dir") = false)
                .def_prop_rw("frames", &Path::getFrames, &Path::setFrames, nb::rv_policy::copy)
                // Not "seq", which is already the predicate below.
                .def_prop_rw("frameSeq", &Path::getSeq, &Path::setSeq, nb::rv_policy::copy)
                .def_prop_ro("seqSize", &Path::getSeqSize)
                .def_prop_ro("isSeq", &Path::isSeq)
                .def_prop_ro("isPartialSeq", &Path::isPartialSeq)
                .def("getFrame", &Path::getFrame, nb::arg("frame"), nb::arg("dir") = false)
                .def("getFrameRange", &Path::getFrameRange,
                    nb::arg("count") = false)
                .def("seq", &Path::seq, nb::arg("path"))
                .def("addSeq", &Path::addSeq, nb::arg("path"))
                .def_prop_ro("isAbs", &Path::isAbs)
                .def("testExt", &Path::testExt)
                .def("getNumbers", &Path::getNumbers)
                .def("getPathSeparators", &Path::getPathSeparators)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            FTK_ENUM_PY(m, DirListSort);
            FTK_ENUM_BIND(m, DirListSort);

            nb::class_<DirListOptions>(m, "DirListOptions")
                .def(nb::init<>())
                .def_rw("sort", &DirListOptions::sort)
                .def_rw("sortReverse", &DirListOptions::sortReverse)
                .def_rw("filter", &DirListOptions::filter)
                .def_rw("filterFiles", &DirListOptions::filterFiles)
                .def_rw("filterExt", &DirListOptions::filterExt)
                .def_rw("seq", &DirListOptions::seq)
                .def_rw("seqExts", &DirListOptions::seqExts)
                .def_rw("seqNegative", &DirListOptions::seqNegative)
                .def_rw("seqMaxDigits", &DirListOptions::seqMaxDigits)
                .def_rw("hidden", &DirListOptions::hidden)
                .def_rw("depth", &DirListOptions::depth)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            nb::class_<DirEntry>(m, "DirEntry")
                .def_rw("path", &DirEntry::path)
                .def_rw("isDir", &DirEntry::isDir)
                .def_rw("size", &DirEntry::size)
                .def_rw("time", &DirEntry::time)
                .def(nanobind::self == nanobind::self)
                .def(nanobind::self != nanobind::self);

            m.def(
                "dirList",
                [](const std::string& path, const DirListOptions& options)
                {
                    return dirList(toFileSystem(path), options);
                },
                nb::arg("path"),
                nb::arg("options") = DirListOptions());

            m.def(
                "expandSeq",
                &expandSeq,
                nb::arg("path"),
                nb::arg("pathOptions") = PathOptions());
        }
    }
}
