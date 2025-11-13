// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/BoxTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Box.h>

#include <sstream>

namespace ftk
{
    namespace core_test
    {
        BoxTest::BoxTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::BoxTest")
        {}

        BoxTest::~BoxTest()
        {}

        std::shared_ptr<BoxTest> BoxTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<BoxTest>(new BoxTest(context));
        }
        
        void BoxTest::run()
        {
            _members();
            _functions();
            _operators();
            _serialize();
        }
        
        void BoxTest::_members()
        {
            {
                const Box2I b2;
                FTK_ASSERT(V2I() == b2.min);
                FTK_ASSERT(V2I() == b2.max);
                const Box3F b3;
                FTK_ASSERT(V3F() == b3.min);
                FTK_ASSERT(V3F() == b3.max);
            }
            {
                const V2I min2(1, 2);
                const V2I max2(3, 4);
                const Box2I b2(min2, max2);
                FTK_ASSERT(min2 == b2.min);
                FTK_ASSERT(max2 == b2.max);
                FTK_ASSERT(1 == b2.x());
                FTK_ASSERT(2 == b2.y());
                FTK_ASSERT(3 == b2.w());
                FTK_ASSERT(3 == b2.h());
                const V3F min3(1.F, 2.F, 3.F);
                const V3F max3(4.F, 5.F, 6.F);
                const Box3F b3(min3, max3);
                FTK_ASSERT(min3 == b3.min);
                FTK_ASSERT(max3 == b3.max);
                FTK_ASSERT(1.F == b3.x());
                FTK_ASSERT(2.F == b3.y());
                FTK_ASSERT(3.F == b3.z());
                FTK_ASSERT(3.F == b3.w());
                FTK_ASSERT(3.F == b3.h());
                FTK_ASSERT(3.F == b3.d());
            }
            {
                const V2I min2(1, 2);
                const Size2I size2(3, 2);
                const Box2I b2(min2, size2);
                FTK_ASSERT(min2 == b2.min);
                FTK_ASSERT(V2I(3, 3) == b2.max);
                FTK_ASSERT(size2 == b2.size());
                const V2F min2f(1.F, 2.F);
                const Size2F size2f(3.F, 2.F);
                const Box2F b2f(min2f, size2f);
                FTK_ASSERT(min2f == b2f.min);
                FTK_ASSERT(V2F(4.F, 4.F) == b2f.max);
                FTK_ASSERT(size2f == b2f.size());
                const V3F min3(1.F, 2.F, 3.F);
                const Size3F size3(4.F, 5.F, 6.F);
                const Box3F b3(min3, size3);
                FTK_ASSERT(min3 == b3.min);
                FTK_ASSERT(V3F(5.F, 7.F, 9.F) == b3.max);
                FTK_ASSERT(size3 == b3.size());
            }
        }
        
        void BoxTest::_functions()
        {
            {
                const Box3F b(1.F, 2.F, 3.F, 4.F, 5.F, 6.F);
                const V3F c = center(b);
                FTK_ASSERT(V3F(3.F, 4.5F, 6.F) == c);
            }
            {
                const Box2F b(1.F, 2.F, 3.F, 4.F);
                const float a = area(b);
                FTK_ASSERT(12.F == a);
            }
            {
                const Box3F b(1.F, 2.F, 3.F, 4.F, 5.F, 6.F);
                const float v = volume(b);
                FTK_ASSERT(120.F == v);
            }
            {
                const Box2F b(1.F, 2.F, 3.F, 4.F);
                const Box2F b1 = move(b, V2F(-1.F, -2.F));
                FTK_ASSERT(Box2F(0.F, 0.F, 3.F, 4.F) == b1);
            }
            {
                const Box2I b(0, 0, 3, 3);
                FTK_ASSERT(contains(b, Box2I(0, 0, 1, 1)));
                FTK_ASSERT(!contains(b, Box2I(-1, -1, 1, 1)));
                FTK_ASSERT(contains(b, V2I(1, 1)));
                FTK_ASSERT(!contains(b, V2I(-1, -1)));
            }
            {
                const Box2F b(0.F, 0.F, 2.F, 2.F);
                FTK_ASSERT(contains(b, Box2F(0.F, 0.F, 1.F, 1.F)));
                FTK_ASSERT(!contains(b, Box2F(-1.F, -1.F, 1.F, 1.F)));
                FTK_ASSERT(contains(b, V2F(1.F, 1.F)));
                FTK_ASSERT(!contains(b, V2F(-1.F, -1.F)));
            }
            {
                const Box2I b(0, 0, 3, 3);
                FTK_ASSERT(intersects(b, Box2I(-1, -1, 2, 2)));
                FTK_ASSERT(!intersects(b, Box2I(-2, -2, 2, 2)));
                const Box2I b1 = intersect(b, Box2I(-1, -1, 2, 2));
                FTK_ASSERT(Box2I(0, 0, 1, 1) == b1);
            }
            {
                const Box2F b(0.F, 0.F, 2.F, 2.F);
                FTK_ASSERT(intersects(b, Box2F(-1.F, -1.F, 2.F, 2.F)));
                FTK_ASSERT(!intersects(b, Box2F(-2.F, -2.F, 1.F, 1.F)));
                const Box2F b1 = intersect(b, Box2F(-1.F, -1.F, 2.F, 2.F));
                FTK_ASSERT(Box2F(0.F, 0.F, 1.F, 1.F) == b1);
            }
            {
                const Box2I b(1, 2, 3, 4);
                const Box2I b1 = expand(b, Box2I(1, 2, 4, 5));
                const Box2I b2 = expand(b, V2I(4, 5));
                FTK_ASSERT(Box2I(1, 2, 4, 5) == b1);
                FTK_ASSERT(Box2I(1, 2, 4, 4) == b2);
            }
            {
                const Box2I b(1, 2, 3, 4);
                const Box2I b1 = margin(b, V2I(1, 2));
                const Box2I b2 = margin(b, 1);
                const Box2I b3 = margin(b, 1, 2);
                const Box2I b4 = margin(b, 1, 2, 3, 4);
                FTK_ASSERT(Box2I(0, 0, 5, 8) == b1);
                FTK_ASSERT(Box2I(0, 1, 5, 6) == b2);
                FTK_ASSERT(Box2I(0, 0, 5, 8) == b3);
                FTK_ASSERT(Box2I(0, 0, 7, 10) == b4);
            }
            {
                FTK_ASSERT(Box2F(1.F, 2.F, 3.F, 4.F) == convert(Box2I(1, 2, 3, 4)));
            }
        }
        
        void BoxTest::_operators()
        {
            {
                const Box2I b = Box2I(1, 2, 3, 4) + V2I(5, 6);
                FTK_ASSERT(b == Box2I(6, 8, 3, 4));
            }
            {
                const Box2I b = Box2I(1, 2, 3, 4) - V2I(5, 6);
                FTK_ASSERT(b == Box2I(-4, -4, 3, 4));
            }
            {
                const Box2I b(1, 2, 3, 4);
                FTK_ASSERT(b == Box2I(1, 2, 3, 4));
                FTK_ASSERT(b != Box2I(0, 1, 2, 3));
            }
        }
        
        void BoxTest::_serialize()
        {
            {
                const Box2I b(1, 2, 3, 4);
                const std::string tmp = to_string(b);
                Box2I b1;
                FTK_ASSERT(from_string(tmp, b1));
                FTK_ASSERT(b == b1);
            }
            {
                const Box2F b(1.F, 2.F, 3.F, 4.F);
                const std::string tmp = to_string(b);
                Box2F b1;
                FTK_ASSERT(from_string(tmp, b1));
                FTK_ASSERT(b == b1);
            }
            {
                const Box3F b(1.F, 2.F, 3.F, 4.F, 5.F, 6.F);
                const std::string tmp = to_string(b);
                Box3F b1;
                FTK_ASSERT(from_string(tmp, b1));
                FTK_ASSERT(b == b1);
            }
            {
                const Box2I b(1, 2, 3, 4);
                nlohmann::json json;
                to_json(json, b);
                Box2I b2;
                from_json(json, b2);
                FTK_ASSERT(b == b2);
            }
            {
                const Box2F b(1.F, 2.F, 3.F, 4.F);
                nlohmann::json json;
                to_json(json, b);
                Box2F b2;
                from_json(json, b2);
                FTK_ASSERT(b == b2);
            }
            {
                const Box3F b(1.F, 2.F, 3.F, 4.F, 5.F, 6.F);
                nlohmann::json json;
                to_json(json, b);
                Box3F b2;
                from_json(json, b2);
                FTK_ASSERT(b == b2);
            }
            {
                const Box2I b(1, 2, 3, 4);
                std::stringstream ss;
                ss << b;
            }
            {
                const Box2F b(1.F, 2.F, 3.F, 4.F);
                std::stringstream ss;
                ss << b;
            }
            {
                const Box3F b(1.F, 2.F, 3.F, 4.F, 5.F, 6.F);
                std::stringstream ss;
                ss << b;
            }
        }
    }
}

