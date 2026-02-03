// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/StringTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/String.h>

namespace ftk
{
    namespace core_test
    {
        StringTest::StringTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::StringTest")
        {}

        StringTest::~StringTest()
        {}

        std::shared_ptr<StringTest> StringTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<StringTest>(new StringTest(context));
        }
        
        void StringTest::run()
        {
            _split();
            _format();
            _compare();
            _convert();
            _filler();
        }
        
        void StringTest::_split()
        {
            {
                std::string a = "a,b,c";
                auto b = split(a, ',');
                std::string c = join(b, ',');
                FTK_ASSERT(a == c);

                a = "a,,c";
                b = split(a, ',', SplitOptions::DiscardEmpty);
                c = join(b, ',');
                FTK_ASSERT("a,c" == c);

                a = "a,,c";
                b = split(a, ',', SplitOptions::KeepEmpty);
                c = join(b, ',');
                FTK_ASSERT(a == c);

                a = ",a,,c";
                b = split(a, ',', SplitOptions::KeepEmpty);
                c = join(b, ',');
                FTK_ASSERT(a == c);

                a = "a,,c,";
                b = split(a, ',', SplitOptions::KeepEmpty);
                c = join(b, ',');
                FTK_ASSERT(a == c);

                a = ",,a,,,c,,";
                b = split(a, ',', SplitOptions::KeepEmpty);
                c = join(b, ',');
                FTK_ASSERT(a == c);
            }
            {
                std::string a = "a,b.c";
                auto b = split(a, { ',', '.' });
                FTK_ASSERT(std::vector<std::string>({ "a", "b", "c" }) == b);

                a = "a,.c";
                b = split(a, { ',', '.' }, SplitOptions::DiscardEmpty);
                FTK_ASSERT(std::vector<std::string>({ "a", "c" }) == b);

                a = "a,.c";
                b = split(a, { ',', '.' }, SplitOptions::KeepEmpty);
                FTK_ASSERT(std::vector<std::string>({ "a", "", "c" }) == b);

                a = ".a,.c,";
                b = split(a, { ',', '.' }, SplitOptions::KeepEmpty);
                FTK_ASSERT(std::vector<std::string>({ "", "a", "", "c", "" }) == b);

                a = "..a,.,c,,";
                b = split(a, { ',', '.' }, SplitOptions::KeepEmpty);
                FTK_ASSERT(std::vector<std::string>({ "", "", "a", "", "", "c", "", "" }) == b);
            }
            {
                FTK_ASSERT(join({ "a", "b", "c" }, ',') == "a,b,c");
                FTK_ASSERT(join({ "a", "b", "c" }, ",.") == "a,.b,.c");
            }
            {
                auto lines = splitLines("");
                FTK_ASSERT(lines == std::vector<std::string>({ "" }));
                lines = splitLines("abc");
                FTK_ASSERT(lines == std::vector<std::string>({ "abc" }));

                lines = splitLines("abc\n");
                FTK_ASSERT(lines == std::vector<std::string>({ "abc", "" }));
                lines = splitLines("\nabc");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "abc" }));
                lines = splitLines("\na\nb\nc\n");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "a", "b", "c", "" }));

                lines = splitLines("abc\r");
                FTK_ASSERT(lines == std::vector<std::string>({ "abc", "" }));
                lines = splitLines("\rabc");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "abc" }));
                lines = splitLines("\ra\rb\rc\r");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "a", "b", "c", "" }));

                lines = splitLines("abc\r\n");
                FTK_ASSERT(lines == std::vector<std::string>({ "abc", "" }));
                lines = splitLines("\r\nabc");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "abc" }));
                lines = splitLines("\r\na\r\nb\r\nc\r\n");
                FTK_ASSERT(lines == std::vector<std::string>({ "", "a", "b", "c", "" }));
            }
        }
        
        void StringTest::_format()
        {
            {
                FTK_ASSERT(toUpper("abc") == "ABC");
                FTK_ASSERT(toLower("ABC") == "abc");
            }
            {
                std::string s = "abc";
                removeTrailingNewlines(s);
                FTK_ASSERT("abc" == s);
                s = "abc\n";
                removeTrailingNewlines(s);
                FTK_ASSERT("abc" == s);
                s = "abc\n\r";
                removeTrailingNewlines(s);
                FTK_ASSERT("abc" == s);
            }
            {
                const std::string s("abcdef");
                FTK_ASSERT("abcdef" == elide(s, 6));
                FTK_ASSERT("abc..." == elide(s, 3));
                FTK_ASSERT("..." == elide(s, 0));
            }
        }
        
        void StringTest::_compare()
        {
            {
                FTK_ASSERT(!compare("abc", "ABC", CaseCompare::Sensitive));
                FTK_ASSERT(compare("abc", "ABC", CaseCompare::Insensitive));
            }
            {
                FTK_ASSERT(!contains("abc", "AB", CaseCompare::Sensitive));
                FTK_ASSERT(contains("abc", "AB", CaseCompare::Insensitive));
            }
        }
        
        void StringTest::_convert()
        {
            {
                const std::string s("abc");
                const std::wstring w = toWide(s);
                FTK_ASSERT(s == fromWide(w));
            }
        }

        void StringTest::_filler()
        {
            {
                _print(getLoremIpsum(100));
            }
        }
    }
}

