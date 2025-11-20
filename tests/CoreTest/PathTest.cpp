// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/PathTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>

namespace ftk
{
    namespace core_test
    {
        PathTest::PathTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::PathTest")
        {}

        PathTest::~PathTest()
        {}

        std::shared_ptr<PathTest> PathTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<PathTest>(new PathTest(context));
        }
        
        void PathTest::run()
        {
            _util();
            _split();
            _drives();
            _userPaths();
            _path();
        }

        void PathTest::_util()
        {
            FTK_ASSERT(!isDotFile(""));
            FTK_ASSERT(!isDotFile("login"));
            FTK_ASSERT(isDotFile(".login"));
            FTK_ASSERT(isDotFile("."));
            FTK_ASSERT(isDotFile(".."));
        }

        void PathTest::_split()
        {
            {
                const std::vector<std::string> pieces = split("");
                FTK_ASSERT(pieces.empty());
            }
            {
                const std::vector<std::string> pieces = split("/");
                FTK_ASSERT(1 == pieces.size());
                FTK_ASSERT("/" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("a");
                FTK_ASSERT(1 == pieces.size());
                FTK_ASSERT("a" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("/a");
                FTK_ASSERT(2 == pieces.size());
                FTK_ASSERT("/" == pieces[0]);
                FTK_ASSERT("a" == pieces[1]);
            }
            {
                const std::vector<std::string> pieces = split("/a/b/c");
                FTK_ASSERT(4 == pieces.size());
                FTK_ASSERT("/" == pieces[0]);
                FTK_ASSERT("a" == pieces[1]);
                FTK_ASSERT("b" == pieces[2]);
                FTK_ASSERT("c" == pieces[3]);
            }
            {
                const std::vector<std::string> pieces = split("/a/b/c/");
                FTK_ASSERT(4 == pieces.size());
                FTK_ASSERT("/" == pieces[0]);
                FTK_ASSERT("a" == pieces[1]);
                FTK_ASSERT("b" == pieces[2]);
                FTK_ASSERT("c" == pieces[3]);
            }
            {
                const std::vector<std::string> pieces = split("a/b/c/");
                FTK_ASSERT(3 == pieces.size());
                FTK_ASSERT("a" == pieces[0]);
                FTK_ASSERT("b" == pieces[1]);
                FTK_ASSERT("c" == pieces[2]);
            }
#if defined(_WINDOWS)
            {
                const std::vector<std::string> pieces = split("c:");
                FTK_ASSERT(1 == pieces.size());
                FTK_ASSERT("c:" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("c:\\");
                FTK_ASSERT(1 == pieces.size());
                FTK_ASSERT("c:\\" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("c:\\a");
                FTK_ASSERT(2 == pieces.size());
                FTK_ASSERT("c:\\" == pieces[0]);
                FTK_ASSERT("a" == pieces[1]);
            }
#endif // _WINDOWS
        }

        void PathTest::_drives()
        {
            for (const auto& drive : getDrives())
            {
                _print(Format("Drive: {0}").arg(drive));
            }
        }

        void PathTest::_userPaths()
        {
            for (auto path : getUserPathEnums())
            {
                _print(Format("{0}: {1}").arg(path).arg(getUserPath(path)));
            }
        }

        void PathTest::_path()
        {
            {
                struct Data
                {
                    std::string path;
                    std::string protocol;
                    std::string dir;
                    std::string base;
                    std::string num;
                    int pad = 0;
                    std::string ext;
                    std::string request;
                };
                const std::vector<Data> data =
                {
                    { "", "", "", "", "", 0, "", "" },
                    { "f", "", "", "f", "", 0, "", "" },
                    { "file", "", "", "file", "", 0, "", "" },
                    { "file.txt", "", "", "file", "", 0, ".txt", "" },
                    { "/tmp/file.txt", "", "/tmp/", "file", "", 0, ".txt", "" },
                    { "/tmp/render.1.exr", "", "/tmp/", "render.", "1", 0, ".exr", "" },
                    { "/tmp/render.0001.exr", "", "/tmp/", "render.", "0001", 4, ".exr", "" },
                    { "/tmp/render0001.exr", "", "/tmp/", "render", "0001", 4, ".exr", "" },
                    { ".", "", "", ".", "", 0, "", "" },
                    { "..", "", "", "..", "", 0, "", "" },
                    { "/.", "", "/", ".", "", 0, "", "" },
                    { "./", "", "./", "", "", 0, "", "" },
                    { ".dotfile", "", "", ".dotfile", "", 0, "", "" },
                    { "/tmp/.dotfile", "", "/tmp/", ".dotfile", "", 0, "", "" },
                    { "/tmp/.dotdir/.dotfile", "", "/tmp/.dotdir/", ".dotfile", "", 0, "", "" },
                    { "0", "", "", "", "0", 1, "", "" },
                    { "0001", "", "", "", "0001", 4, "", "" },
                    { "####", "", "", "", "####", 4, "", "" },
                    { "/tmp/0001", "", "/tmp/", "", "0001", 4, "", "" },
                    { "/tmp/####", "", "/tmp/", "", "####", 4, "", "" },
                    { "/tmp/0001.exr", "", "/tmp/", "", "0001", 4, ".exr", "" },
                    { "/tmp/####.exr", "", "/tmp/", "", "####", 4, ".exr", "" },
                    { "0001.exr", "", "", "", "0001", 4, ".exr", "" },
                    { "####.exr", "", "", "", "####", 4, ".exr", "" },
                    { "1.exr", "", "", "", "1", 0, ".exr", "" },
                    { "C:", "", "C:", "", "", 0, "", "" },
                    { "C:/", "", "C:/", "", "", 0, "", "" },
                    { "C:/tmp/file.txt", "", "C:/tmp/", "file", "", 0, ".txt", "" },
                    { "file:///tmp/render.1.exr", "file://", "/tmp/", "render.", "1", 0, ".exr", "" },
                    { "http:///tmp/render.1.exr", "http://", "/tmp/", "render.", "1", 0, ".exr", "" },
                    { "http:///tmp/render.1.exr?user=foo;password=bar", "http://", "/tmp/", "render.", "1", 0, ".exr", "?user=foo;password=bar" }
                };
                for (const auto& i : data)
                {
                    const Path p(i.path);
                    FTK_ASSERT(p.get() == i.path);
                    std::string tmp = p.getProtocol();
                    FTK_ASSERT(tmp == i.protocol);
                    tmp = p.getDir();
                    FTK_ASSERT(tmp == i.dir);
                    tmp = p.getBase();
                    FTK_ASSERT(tmp == i.base);
                    tmp = p.getNum();
                    FTK_ASSERT(tmp == i.num);
                    FTK_ASSERT(p.getPad() == i.pad);
                    tmp = p.getExt();
                    FTK_ASSERT(tmp == i.ext);
                    tmp = p.getRequest();
                    FTK_ASSERT(tmp == i.request);
                }
            }
            {
                Path p("render.1.exr");
                FrameRange r(1, 100);
                p.setFrames(r);
                FTK_ASSERT(r == p.getFrames());
            }
            {
                std::string tmp = Path("render.exr").getFrame(100);
                FTK_ASSERT(tmp == "render.exr");
                tmp = Path("render.1.exr").getFrame(100);
                FTK_ASSERT(tmp == "render.100.exr");
                tmp = Path("render.0001.exr").getFrame(100);
                FTK_ASSERT(tmp == "render.0100.exr");
                tmp = Path("/tmp/render.0001.exr").getFrame(100, false);
                FTK_ASSERT(tmp == "render.0100.exr");
            }
            {
                Path p("render.1.exr");
                FTK_ASSERT("1" == p.getFrameRange());
                const Path p2("render.100.exr");
                const Path p3("render.exr");
                FTK_ASSERT(p.seq(p2));
                FTK_ASSERT(!p.seq(p3));
                FTK_ASSERT(p.addSeq(p2));
                FTK_ASSERT(FrameRange(1, 100) == p.getFrames());
                FTK_ASSERT(!p.addSeq(p3));
                FTK_ASSERT(FrameRange(1, 100) == p.getFrames());
                FTK_ASSERT("1-100" == p.getFrameRange());
            }
            {
                const Path p("render.1.exr");
                const Path p2("render.100.exr");
                FTK_ASSERT(p == p);
                FTK_ASSERT(p != p2);
            }
        }
    }
}

