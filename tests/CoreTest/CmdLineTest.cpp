// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/CmdLineTest.h>

#include <ftk/Core/CmdLine.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace core_test
    {
        CmdLineTest::CmdLineTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::app_test::CmdLineTest")
        {}

        CmdLineTest::~CmdLineTest()
        {}

        std::shared_ptr<CmdLineTest> CmdLineTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<CmdLineTest>(new CmdLineTest(context));
        }
        
        void CmdLineTest::run()
        {
            std::vector<std::string> argv =
            {
                "-flag",
                "-boolOption", "1",
                "-intOption", "10",
                "100",
                "string",
                "list0",
                "list1",
                "list2"
            };

            auto flagCmdLineOption = CmdLineFlag::create(
                { "-flag" },
                "This is a flag option");
            _print(flagCmdLineOption->getHelp());
            flagCmdLineOption->parse(argv);
            FTK_ASSERT(flagCmdLineOption->found());
            FTK_ASSERT(!flagCmdLineOption->getMatchedName().empty());

            auto boolCmdLineOption = CmdLineOption<bool>::create(
                { "-boolOption" },
                "This is a boolean option",
                std::string(),
                "0",
                join({ "0", "1" }, ", "));
            _print(boolCmdLineOption->getHelp());
            boolCmdLineOption->parse(argv);
            FTK_ASSERT(boolCmdLineOption->getValue());

            auto intCmdLineOption = CmdLineOption<int>::create(
                { "-intOption" },
                "This is an integer option",
                std::string(),
                0);
            _print(intCmdLineOption->getHelp());
            intCmdLineOption->parse(argv);
            FTK_ASSERT(10 == intCmdLineOption->getValue());

            auto intCmdLineArg = CmdLineArg<int>::create(
                "int",
                "This is an integer argument");
            FTK_ASSERT(!intCmdLineArg->getName().empty());
            FTK_ASSERT(!intCmdLineArg->isOptional());
            _print(intCmdLineArg->getHelp());
            intCmdLineArg->parse(argv);
            FTK_ASSERT(100 == intCmdLineArg->getValue());

            auto stringCmdLineArg = CmdLineArg<std::string>::create(
                "string",
                "This is a string argument");
            FTK_ASSERT(!stringCmdLineArg->getName().empty());
            FTK_ASSERT(!stringCmdLineArg->isOptional());
            _print(stringCmdLineArg->getHelp());
            stringCmdLineArg->parse(argv);
            FTK_ASSERT("string" == stringCmdLineArg->getValue());

            auto listCmdLineArg = CmdLineListArg<std::string>::create(
                "list",
                "This is a list argument");
            FTK_ASSERT(!listCmdLineArg->getName().empty());
            FTK_ASSERT(!listCmdLineArg->isOptional());
            _print(listCmdLineArg->getHelp());
            listCmdLineArg->parse(argv);
            const std::vector<std::string>& list = listCmdLineArg->getList();
            FTK_ASSERT(3 == list.size());
            FTK_ASSERT("list0" == list[0]);
            FTK_ASSERT("list1" == list[1]);
            FTK_ASSERT("list2" == list[2]);

            try
            {
                intCmdLineArg->parse(argv);
                FTK_ASSERT(false);
            }
            catch (const std::exception&)
            {}
            try
            {
                stringCmdLineArg->parse(argv);
                FTK_ASSERT(false);
            }
            catch (const std::exception&)
            {}
            argv.push_back("-intOption");
            try
            {
                intCmdLineOption->parse(argv);
                FTK_ASSERT(false);
            }
            catch (const std::exception&)
            {}
        }
    }
}

