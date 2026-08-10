// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/PathTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/FileIO.h>
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
            _enums();
            _util();
            _drives();
            _userPaths();
            _tmpDir();
            _frameSeq();
            _path();
            _dirList();
            _expandSeq();
        }

        void PathTest::_enums()
        {
            FTK_TEST_ENUM(UserPath);
            FTK_TEST_ENUM(DirListSort);
        }

        void PathTest::_util()
        {
            {
                FTK_CHECK(!isDotFile(""));
                FTK_CHECK(!isDotFile("login"));
                FTK_CHECK(isDotFile(".login"));
                FTK_CHECK(isDotFile("."));
                FTK_CHECK(isDotFile(".."));
            }
            {
                const std::vector<std::string> pieces = split("");
                FTK_CHECK(pieces.empty());
            }
            {
                const std::vector<std::string> pieces = split("/");
                FTK_CHECK(1 == pieces.size());
                FTK_CHECK("/" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("a");
                FTK_CHECK(1 == pieces.size());
                FTK_CHECK("a" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("/a");
                FTK_CHECK(2 == pieces.size());
                FTK_CHECK("/" == pieces[0]);
                FTK_CHECK("a" == pieces[1]);
            }
            {
                const std::vector<std::string> pieces = split("/a/b/c");
                FTK_CHECK(4 == pieces.size());
                FTK_CHECK("/" == pieces[0]);
                FTK_CHECK("a" == pieces[1]);
                FTK_CHECK("b" == pieces[2]);
                FTK_CHECK("c" == pieces[3]);
            }
            {
                const std::vector<std::string> pieces = split("/a/b/c/");
                FTK_CHECK(4 == pieces.size());
                FTK_CHECK("/" == pieces[0]);
                FTK_CHECK("a" == pieces[1]);
                FTK_CHECK("b" == pieces[2]);
                FTK_CHECK("c" == pieces[3]);
            }
            {
                const std::vector<std::string> pieces = split("a/b/c/");
                FTK_CHECK(3 == pieces.size());
                FTK_CHECK("a" == pieces[0]);
                FTK_CHECK("b" == pieces[1]);
                FTK_CHECK("c" == pieces[2]);
            }
#if defined(_WINDOWS)
            {
                const std::vector<std::string> pieces = split("c:");
                FTK_CHECK(1 == pieces.size());
                FTK_CHECK("c:" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("c:\\");
                FTK_CHECK(1 == pieces.size());
                FTK_CHECK("c:\\" == pieces[0]);
            }
            {
                const std::vector<std::string> pieces = split("c:\\a");
                FTK_CHECK(2 == pieces.size());
                FTK_CHECK("c:\\" == pieces[0]);
                FTK_CHECK("a" == pieces[1]);
            }
#endif // _WINDOWS
            {
                FTK_CHECK(appendSeparator("tmp") == "tmp/");
                FTK_CHECK(appendSeparator("/tmp") == "/tmp/");
                FTK_CHECK(appendSeparator("\\tmp") == "\\tmp\\");

                // An empty directory has to stay relative: appending a
                // separator would name the root instead.
                FTK_CHECK(appendSeparator("").empty());
                FTK_CHECK(Path("", "render.tif").get() == "render.tif");
            }
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

        void PathTest::_tmpDir()
        {
            {
                TmpDir tmpDir;
                _print(Format("Tmp dir: {0}").arg(tmpDir.getPath().u8string()));
                FileIO::create(tmpDir.getPath() / "render.exr", FileMode::Write);
            }
        }

        void PathTest::_frameSeq()
        {
            {
                std::vector<int64_t> frames = {};
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(0 == seqs.size());
                frames = toFrames(seqs);
                FTK_CHECK(0 == frames.size());
            }
            {
                std::vector<int64_t> frames = { 0 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0) == seqs[0]);
                frames = toFrames(seqs);
                FTK_CHECK(1 == frames.size());
                FTK_CHECK(0 == frames[0]);
            }
            {
                std::vector<int64_t> frames = { 0, 1 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 1) == seqs[0]);
                frames = toFrames(seqs);
                FTK_CHECK(2 == frames.size());
                FTK_CHECK(0 == frames[0]);
                FTK_CHECK(1 == frames[1]);
            }
            {
                std::vector<int64_t> frames = { 0, 2 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 2, 2) == seqs[0]);
                frames = toFrames(seqs);
                FTK_CHECK(2 == frames.size());
                FTK_CHECK(0 == frames[0]);
                FTK_CHECK(2 == frames[1]);
            }
            {
                std::vector<int64_t> frames = { 0, 1, 2 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 2) == seqs[0]);
            }
            {
                std::vector<int64_t> frames = { 0, 1, 2, 3 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 3) == seqs[0]);
            }
            {
                std::vector<int64_t> frames = { 0, 1, 2, 4 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(2 == seqs.size());
                FTK_CHECK(FrameSeq(0, 2) == seqs[0]);
                FTK_CHECK(FrameSeq(4, 4) == seqs[1]);
            }
            {
                std::vector<int64_t> frames = { 0, 1, 2, 4, 5 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(2 == seqs.size());
                FTK_CHECK(FrameSeq(0, 2) == seqs[0]);
                FTK_CHECK(FrameSeq(4, 5) == seqs[1]);
            }
            {
                std::vector<int64_t> frames = { 0, 2, 4, 6 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 6, 2) == seqs[0]);
            }
            {
                std::vector<int64_t> frames = { 0, 3, 6, 9 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(1 == seqs.size());
                FTK_CHECK(FrameSeq(0, 9, 3) == seqs[0]);
            }
            {
                std::vector<int64_t> frames = { 0, 1, 2, 4, 6, 8 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(2 == seqs.size());
                FTK_CHECK(FrameSeq(0, 2) == seqs[0]);
                FTK_CHECK(FrameSeq(4, 8, 2) == seqs[1]);
            }
            {
                std::vector<int64_t> frames = { 0, 2, 4, 5, 6, 7 };
                auto seqs = toFrameSeq(frames);
                FTK_CHECK(2 == seqs.size());
                FTK_CHECK(FrameSeq(0, 4, 2) == seqs[0]);
                FTK_CHECK(FrameSeq(5, 7) == seqs[1]);
            }
            {
                std::vector<FrameSeq> seqs;
                _print("Frame sequence: " + getLabel(seqs));
                seqs.push_back(FrameSeq(0, 2));
                _print("Frame sequence: " + getLabel(seqs));
                seqs.push_back(FrameSeq(4, 8, 2));
                _print("Frame sequence: " + getLabel(seqs));
                FTK_CHECK("0-2,4-8:2" == getLabel(seqs));
                FTK_CHECK("0000-0002,0004-0008:2" == getLabel(seqs, 4));
                FTK_CHECK(6 == getFrameCount(seqs));
                FTK_CHECK(RangeI64(0, 8) == getRange(seqs));
            }
            {
                std::vector<FrameSeq> seqs;
                FTK_CHECK(0 == getFrameCount(seqs));
                FTK_CHECK(!getRange(seqs).has_value());
            }

            // Adding frames in order should give the same sequences as
            // converting them all at once.
            {
                const std::vector<std::vector<int64_t> > data =
                {
                    { 0 },
                    { 0, 1 },
                    { 0, 2 },
                    { 0, 1, 2 },
                    { 0, 1, 2, 4 },
                    { 0, 1, 2, 4, 5 },
                    { 0, 2, 4, 6 },
                    { 0, 3, 6, 9 },
                    { 0, 1, 2, 4, 6, 8 },
                    { 0, 2, 4, 5, 6, 7 },
                    { 1, 2, 3, 4, 5, 10, 11, 12, 20 }
                };
                for (const auto& frames : data)
                {
                    std::vector<FrameSeq> seqs;
                    for (int64_t frame : frames)
                    {
                        addFrame(seqs, frame);
                    }
                    FTK_CHECK(toFrameSeq(frames) == seqs);
                    FTK_CHECK(frames.size() == getFrameCount(seqs));
                }
            }

            // Frames added out of order or repeated should still give the
            // same set of frames.
            {
                const std::vector<std::vector<int64_t> > data =
                {
                    { 2, 0, 1 },
                    { 0, 4, 2 },
                    { 0, 2, 4, 1, 3 },
                    { 20, 10, 11, 12, 1, 2, 3, 4, 5 },
                    { 5, 5, 5, 1, 1 }
                };
                for (const auto& frames : data)
                {
                    std::vector<FrameSeq> seqs;
                    for (int64_t frame : frames)
                    {
                        addFrame(seqs, frame);
                    }
                    FTK_CHECK(toFrames(toFrameSeq(frames)) == toFrames(seqs));
                }
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
                    { "/tmp/render-0001.exr", "", "/tmp/", "render-", "0001", 4, ".exr", "" },
                    { "/tmp/render-1.exr", "", "/tmp/", "render", "-1", 0, ".exr", "" },
                    { "/tmp/render-12.exr", "", "/tmp/", "render", "-12", 0, ".exr", "" },
                    { ".", "", "", ".", "", 0, "", "" },
                    { "..", "", "", "..", "", 0, "", "" },
                    { "/.", "", "/", ".", "", 0, "", "" },
                    { "./", "", "./", "", "", 0, "", "" },
                    { ".dotfile", "", "", ".dotfile", "", 0, "", "" },
                    { "/tmp/.dotfile", "", "/tmp/", ".dotfile", "", 0, "", "" },
                    { "/tmp/.dotdir/.dotfile", "", "/tmp/.dotdir/", ".dotfile", "", 0, "", "" },
                    { "0", "", "", "", "0", 1, "", "" },
                    { "0001", "", "", "", "0001", 4, "", "" },
                    { "-0001", "", "", "-", "0001", 4, "", "" },
                    { "-1", "", "", "", "-1", 0, "", "" },
                    { "####", "", "", "", "####", 4, "", "" },
                    { "/tmp/0001", "", "/tmp/", "", "0001", 4, "", "" },
                    { "/tmp/-0001", "", "/tmp/", "-", "0001", 4, "", "" },
                    { "/tmp/####", "", "/tmp/", "", "####", 4, "", "" },
                    { "/tmp/0001.exr", "", "/tmp/", "", "0001", 4, ".exr", "" },
                    { "/tmp/-0001.exr", "", "/tmp/", "-", "0001", 4, ".exr", "" },
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
                    FTK_CHECK(p.get() == i.path);
                    std::string tmp = p.getProtocol();
                    FTK_CHECK(tmp == i.protocol);
                    tmp = p.getDir();
                    FTK_CHECK(tmp == i.dir);
                    tmp = p.getBase();
                    FTK_CHECK(tmp == i.base);
                    tmp = p.getNum();
                    FTK_CHECK(tmp == i.num);
                    FTK_CHECK(p.getPad() == i.pad);
                    tmp = p.getExt();
                    FTK_CHECK(tmp == i.ext);
                    tmp = p.getRequest();
                    FTK_CHECK(tmp == i.request);
                }
            }
            {
                Path p("render.1.exr");
                RangeI64 r(1, 100);
                p.setFrames(r);
                FTK_CHECK(r == p.getFrames());
            }
            {
                std::string tmp = Path("render.exr").getFrame(100);
                FTK_CHECK(tmp == "render.exr");
                tmp = Path("render.1.exr").getFrame(100);
                FTK_CHECK(tmp == "render.100.exr");
                tmp = Path("/tmp/render.0001.exr").getFrame(100);
                FTK_CHECK(tmp == "render.0100.exr");
                tmp = Path("/tmp/render.0001.exr").getFrame(100, true);
                FTK_CHECK(tmp == "/tmp/render.0100.exr");
            }
            {
                // The sign goes in front of the padding rather than being
                // padded around, so that a negative frame gives back a name
                // that can be read again.
                FTK_CHECK("0001" == toString(1, 4));
                FTK_CHECK("-0001" == toString(-1, 4));
                FTK_CHECK("1" == toString(1));
                FTK_CHECK("-1" == toString(-1));
                FTK_CHECK("/tmp/render-2.exr" ==
                    Path("/tmp/render-1.exr").getFrame(-2, true));
                // A minus in front of padded digits belongs to the base, so
                // this is frame two of "shot-", not minus two of "shot".
                FTK_CHECK("/tmp/shot-0000002.tif" ==
                    Path("/tmp/shot-0000001.tif").getFrame(2, true));
            }
            {
                PathOptions options;
                Path path("render.0000000001.exr", options);
                FTK_CHECK(!path.hasNum());
                options.seqNegative = false;
                path = Path("render-0001.exr", options);
                FTK_CHECK("0001" == path.getNum());
            }
            {
                Path p("render.1.exr");
                FTK_CHECK(!p.isSeq());
                FTK_CHECK("1" == p.getFrameRange());
                const Path p2("render.100.exr");
                const Path p3("render.exr");
                FTK_CHECK(p.seq(p2));
                FTK_CHECK(!p.seq(p3));
                FTK_CHECK(p.addSeq(p2));
                FTK_CHECK(p.isSeq());
                FTK_CHECK(RangeI64(1, 100) == p.getFrames());
                FTK_CHECK(!p.addSeq(p3));
                FTK_CHECK(RangeI64(1, 100) == p.getFrames());
                FTK_CHECK("1-100" == p.getFrameRange());
            }
            {
                // A sequence missing frames keeps the frames it has, not just
                // the range they span.
                Path p("render.1.exr");
                FTK_CHECK(!p.isPartialSeq());
                FTK_CHECK(1 == p.getSeqSize());
                for (int frame : { 2, 3, 4, 5, 10, 11, 12 })
                {
                    FTK_CHECK(p.addSeq(Path(Format("render.{0}.exr").arg(frame).str())));
                }
                FTK_CHECK(p.isSeq());
                FTK_CHECK(p.isPartialSeq());
                FTK_CHECK(RangeI64(1, 12) == p.getFrames());
                FTK_CHECK(8 == p.getSeqSize());
                FTK_CHECK("1-5,10-12" == getLabel(p.getSeq()));
                FTK_CHECK("render.1.exr" == p.get());

                // Setting a contiguous range replaces the sequence.
                p.setFrames(RangeI64(1, 12));
                FTK_CHECK(!p.isPartialSeq());
                FTK_CHECK(12 == p.getSeqSize());
            }
            {
                Path p("render.#.exr");
                FTK_CHECK(!p.isSeq());
                FTK_CHECK(p.hasSeqWildcard());
                const Path p2("render.1.exr");
                const Path p3("render.100.exr");
                FTK_CHECK(p.addSeq(p2));
                FTK_CHECK(p.addSeq(p3));
                FTK_CHECK(p.isSeq());
                FTK_CHECK(!p.hasSeqWildcard());
                FTK_CHECK("render.1.exr" == p.get());
            }
            {
                Path p("render.1.exr");
                p.setProtocol("file://");
                FTK_CHECK("file://render.1.exr" == p.get());
                p.setDir("/tmp/");
                FTK_CHECK("file:///tmp/render.1.exr" == p.get());
                p.setBase("lighting");
                FTK_CHECK("file:///tmp/lighting1.exr" == p.get());
                p.setNum("0100");
                FTK_CHECK("file:///tmp/lighting0100.exr" == p.get());
                p.setPad(6);
                FTK_CHECK("file:///tmp/lighting000100.exr" == p.get());
                p.setExt(".tiff");
                FTK_CHECK("file:///tmp/lighting000100.tiff" == p.get());
                p.setRequest("?user=foo;password=bar");
                FTK_CHECK("file:///tmp/lighting000100.tiff?user=foo;password=bar" == p.get());
            }
            {
                FTK_CHECK(!Path("render.exr").isAbs());
                FTK_CHECK(Path("/tmp/render.exr").isAbs());
                FTK_CHECK(Path("C:/tmp/render.exr").isAbs());
            }
            {
                const Path p("render.1.exr");
                const Path p2("render.100.exr");
                FTK_CHECK(p == p);
                FTK_CHECK(p != p2);
            }
        }

        void PathTest::_dirList()
        {
            {
                std::filesystem::path dir = _getTempDir() / "PathTest1";
                std::filesystem::create_directory(dir);
                {
                    FileIO::create(dir / "render.exr", FileMode::Write);
                }
                {
                    FileIO::create(dir / "0001.exr", FileMode::Write);
                }
                {
                    FileIO::create(dir / "0002", FileMode::Write);
                }
                {
                    std::filesystem::create_directory(dir / "tmp");
                }
                for (int i = 0; i < 10; ++i)
                {
                    FileIO::create(dir / Format("render.{0}.exr").arg(i).str(), FileMode::Write);
                }
                for (int i = 100; i < 103; ++i)
                {
                    FileIO::create(dir / Format("render.{0}.png").arg(i).str(), FileMode::Write);
                }
                auto dirEntries = dirList(dir);
                FTK_CHECK(6 == dirEntries.size());
                _print("List 0: " + dirEntries[0].path.getFileName());
                _print("List 1: " + dirEntries[1].path.getFileName());
                _print("List 2: " + dirEntries[2].path.getFileName());
                _print("List 3: " + dirEntries[3].path.getFileName());
                _print("List 4: " + dirEntries[4].path.getFileName());
                _print("List 5: " + dirEntries[5].path.getFileName());
                FTK_CHECK("tmp" == dirEntries[0].path.getFileName());
                FTK_CHECK("0001.exr" == dirEntries[1].path.getFileName());
                FTK_CHECK("0002" == dirEntries[2].path.getFileName());
                FTK_CHECK("render.0.exr" == dirEntries[3].path.getFileName());
                FTK_CHECK(dirEntries[3].path.getFrames().has_value());
                FTK_CHECK(0 == dirEntries[3].path.getFrames().value().min());
                FTK_CHECK(9 == dirEntries[3].path.getFrames().value().max());
                FTK_CHECK("render.100.png" == dirEntries[4].path.getFileName());
                FTK_CHECK(100 == dirEntries[4].path.getFrames().value().min());
                FTK_CHECK(102 == dirEntries[4].path.getFrames().value().max());
                FTK_CHECK("render.exr" == dirEntries[5].path.getFileName());

                DirListOptions options;
                options.seqExts.push_back(".exr");
                dirEntries = dirList(dir, options);
                FTK_CHECK(8 == dirEntries.size());
                FTK_CHECK("tmp" == dirEntries[0].path.getFileName());
                FTK_CHECK("0001.exr" == dirEntries[1].path.getFileName());
                FTK_CHECK("0002" == dirEntries[2].path.getFileName());
                FTK_CHECK("render.0.exr" == dirEntries[3].path.getFileName());
                FTK_CHECK("render.100.png" == dirEntries[4].path.getFileName());
                FTK_CHECK("render.101.png" == dirEntries[5].path.getFileName());
                FTK_CHECK("render.102.png" == dirEntries[6].path.getFileName());
                FTK_CHECK("render.exr" == dirEntries[7].path.getFileName());
            }
            {
                std::filesystem::path dir = _getTempDir() / "PathTest2";
                std::filesystem::create_directory(dir);
                for (int i = 0; i < 10; ++i)
                {
                    FileIO::create(dir / Format("asteroid_V02.{0}.exr").arg(i, 4, '0').str(), FileMode::Write);
                }
                for (int i = 0; i < 10; ++i)
                {
                    FileIO::create(dir / Format("asteroid_V01.{0}.exr").arg(i, 4, '0').str(), FileMode::Write);
                }
                DirListOptions options;
                options.seqExts.push_back(".exr");
                auto dirEntries = dirList(dir, options);
                FTK_CHECK(2 == dirEntries.size());
                _print("List 0: " + dirEntries[0].path.getFileName());
                _print("List 1: " + dirEntries[1].path.getFileName());
            }
            {
                // A sequence with gaps.
                std::filesystem::path dir = _getTempDir() / "PathTest4";
                std::filesystem::create_directory(dir);
                const std::vector<int> frames = { 1, 2, 3, 4, 5, 10, 11, 12, 20 };
                for (int i : frames)
                {
                    FileIO::create(
                        dir / Format("render.{0}.exr").arg(i, 4, '0').str(),
                        FileMode::Write);
                }
                auto dirEntries = dirList(dir);
                FTK_CHECK(1 == dirEntries.size());
                const Path& path = dirEntries[0].path;
                _print("Partial sequence: " + getLabel(path.getSeq()));
                FTK_CHECK(path.isSeq());
                FTK_CHECK(path.isPartialSeq());
                FTK_CHECK(RangeI64(1, 20) == path.getFrames());
                FTK_CHECK(frames.size() == path.getSeqSize());
                FTK_CHECK("0001-0020" == path.getFrameRange());
                FTK_CHECK("render.0001.exr" == path.getFileName());
                FTK_CHECK("1-5,10-12,20" == getLabel(path.getSeq()));
                FTK_CHECK(
                    std::vector<int64_t>(frames.begin(), frames.end()) ==
                    toFrames(path.getSeq()));
            }
        }

        void PathTest::_expandSeq()
        {
            std::filesystem::path dir = _getTempDir() / "PathTest3";
            std::filesystem::create_directory(dir);
            FileIO::create(dir / "render.exr", FileMode::Write);
            for (int i = 0; i < 10; ++i)
            {
                FileIO::create(dir / Format("render.{0}.exr").arg(i).str(), FileMode::Write);
            }
            for (int i = 100; i < 103; ++i)
            {
                FileIO::create(dir / Format("render.{0}.png").arg(i).str(), FileMode::Write);
            }

            Path path = expandSeq(Path((dir / "render.0.exr").u8string()));
            FTK_CHECK(0 == path.getFrames().value().min());
            FTK_CHECK(9 == path.getFrames().value().max());

            path = expandSeq(Path((dir / "render.#.exr").u8string()));
            FTK_CHECK(0 == path.getFrames().value().min());
            FTK_CHECK(9 == path.getFrames().value().max());

            path = expandSeq(Path((dir / "render.0.png").u8string()));
            FTK_CHECK(100 == path.getFrames().value().min());
            FTK_CHECK(102 == path.getFrames().value().max());

            path = expandSeq(Path((dir / "render.0.tiff").u8string()));
            FTK_CHECK(0 == path.getFrames().value().min());
            FTK_CHECK(0 == path.getFrames().value().max());

            {
                std::filesystem::path dir2 = _getTempDir() / "PathTest5";
                std::filesystem::create_directory(dir2);
                const std::vector<int> frames = { 1, 2, 3, 8, 9 };
                for (int i : frames)
                {
                    FileIO::create(
                        dir2 / Format("render.{0}.exr").arg(i, 4, '0').str(),
                        FileMode::Write);
                }
                path = expandSeq(Path((dir2 / "render.####.exr").u8string()));
                _print("Partial sequence: " + getLabel(path.getSeq()));
                FTK_CHECK(path.isPartialSeq());
                FTK_CHECK(RangeI64(1, 9) == path.getFrames());
                FTK_CHECK("1-3,8-9" == getLabel(path.getSeq()));
                FTK_CHECK(
                    std::vector<int64_t>(frames.begin(), frames.end()) ==
                    toFrames(path.getSeq()));
            }
        }
    }
}

