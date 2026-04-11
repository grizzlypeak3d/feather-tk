// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/LRUCacheTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LRUCache.h>

#include <algorithm>

namespace ftk
{
    namespace core_test
    {
        LRUCacheTest::LRUCacheTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::LRUCacheTest")
        {}

        LRUCacheTest::~LRUCacheTest()
        {}

        std::shared_ptr<LRUCacheTest> LRUCacheTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<LRUCacheTest>(new LRUCacheTest(context));
        }
        
        void LRUCacheTest::run()
        {
            LRUCache<int, bool> c;
            c.setMax(3);
            c.setMax(3);
            FTK_ASSERT(3 == c.getMax());
            FTK_ASSERT(0 == c.getSize());
            FTK_ASSERT(0 == c.getCount());
            _print(Format("Percentage: {0}").arg(c.getPercentage()));
            
            c.add(0, true);
            _print(Format("Percentage: {0}").arg(c.getPercentage()));
            c.add(1, true);
            _print(Format("Percentage: {0}").arg(c.getPercentage()));
            c.add(2, true);
            _print(Format("Percentage: {0}").arg(c.getPercentage()));
            c.add(3, true);
            _print(Format("Percentage: {0}").arg(c.getPercentage()));
            FTK_ASSERT(3 == c.getSize());

            FTK_ASSERT(c.contains(2));
            bool v = false;
            FTK_ASSERT(c.get(2, v));
            FTK_ASSERT(v);
            FTK_ASSERT(!c.get(0, v));
            c.remove(2);
            FTK_ASSERT(!c.contains(2));
            c.clear();
            FTK_ASSERT(0 == c.getSize());
            
            c.add(0, true);
            c.add(1, true);
            c.add(2, true);
            c.add(3, true);
            auto keys = c.getKeys();
            FTK_ASSERT(std::find(keys.begin(), keys.end(), 1) != keys.end());
            FTK_ASSERT(std::find(keys.begin(), keys.end(), 2) != keys.end());
            FTK_ASSERT(std::find(keys.begin(), keys.end(), 3) != keys.end());
            FTK_ASSERT(c.getValues()[0]);
            FTK_ASSERT(c.getValues()[1]);
            FTK_ASSERT(c.getValues()[2]);
            
            c.setMax(2);
            FTK_ASSERT(2 == c.getSize());
        }
    }
}

