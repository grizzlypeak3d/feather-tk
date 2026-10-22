// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/DiagSystemTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/DiagSystem.h>

namespace ftk
{
    namespace core_test
    {
        DiagSystemTest::DiagSystemTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::DiagSystemTest")
        {}

        DiagSystemTest::~DiagSystemTest()
        {}

        std::shared_ptr<DiagSystemTest> DiagSystemTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DiagSystemTest>(new DiagSystemTest(context));
        }

        void DiagSystemTest::run()
        {
            // A system of its own, so the samplers are only its own and
            // these.
            auto system = DiagSystem::create(_context);
            FTK_CHECK(std::vector<std::string>({ "Memory", "Objects" }) == system->getGroups());

            // A group the order does not name follows the ones it does; one
            // it names goes where it says, whenever it is added.
            int64_t value = 0;
            system->addSampler("Test/A: {0}", [&value] { return ++value; });
            system->addSampler("Test/A: {0}", [] { return 0; });
            system->addSampler("Frame/Time: {0}ms", [] { return 1000; }, DiagFormat{ 1000.0, 1 });
            FTK_CHECK(std::vector<std::string>({ "Frame", "Memory", "Objects", "Test" }) == system->getGroups());
            FTK_CHECK(1 == system->getNames("Test").size());
            FTK_CHECK(system->hasSampler("Test/A: {0}"));
            FTK_CHECK(1000.0 == system->getFormat("Frame/Time: {0}ms").divisor);

            // An application puts its own groups first; the rest keep the
            // order they were added in.
            system->setGroupOrder({ "Test" });
            FTK_CHECK(std::vector<std::string>({ "Test" }) == system->getGroupOrder());
            FTK_CHECK(std::vector<std::string>({ "Test", "Memory", "Objects", "Frame" }) == system->getGroups());
            auto order = system->getGroupOrder();
            order.push_back("Frame");
            system->setGroupOrder(order);
            FTK_CHECK(std::vector<std::string>({ "Test", "Frame", "Memory", "Objects" }) == system->getGroups());

            // The history is kept to the most samples asked for.
            system->setSamplesMax(3);
            for (int i = 0; i < 5; ++i)
            {
                system->tick();
            }
            const auto& samples = system->getSamples();
            FTK_CHECK(3 == samples.at("Test/A: {0}").size());
            FTK_CHECK(5 == samples.at("Test/A: {0}").back());
            system->setSamplesMax(2);
            FTK_CHECK(2 == system->getSamples().at("Test/A: {0}").size());
            FTK_CHECK("Time: 1.0ms" == diagText("Time: {0}ms", system->getFormat("Frame/Time: {0}ms"), 1000));
        }
    }
}
