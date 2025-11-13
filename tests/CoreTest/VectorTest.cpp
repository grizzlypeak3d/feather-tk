// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/VectorTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Vector.h>

#include <sstream>

namespace ftk
{
    namespace core_test
    {
        VectorTest::VectorTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::VectorTest")
        {}

        VectorTest::~VectorTest()
        {}

        std::shared_ptr<VectorTest> VectorTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<VectorTest>(new VectorTest(context));
        }
        
        void VectorTest::run()
        {
            _members();
            _functions();
            _operators();
            _serialize();
        }
        
        void VectorTest::_members()
        {
            {
                Vector<1, int> v;
                FTK_ASSERT(0 == v[0]);
                v[0] = 1;
                FTK_ASSERT(1 == v[0]);
                FTK_ASSERT(v.data());
            }
            {
                const V2I v2;
                FTK_ASSERT(0 == v2[0]);
                FTK_ASSERT(0 == v2[1]);
                FTK_ASSERT(v2.data());
                const V3F v3;
                FTK_ASSERT(0.F == v3[0]);
                FTK_ASSERT(0.F == v3[1]);
                FTK_ASSERT(0.F == v3[2]);
                FTK_ASSERT(v3.data());
                const V4F v4;
                FTK_ASSERT(0.F == v4[0]);
                FTK_ASSERT(0.F == v4[1]);
                FTK_ASSERT(0.F == v4[2]);
                FTK_ASSERT(0.F == v4[3]);
                FTK_ASSERT(v4.data());
            }
            {
                V2I v2;
                v2[0] = 1;
                v2[1] = 2;
                FTK_ASSERT(1 == v2[0]);
                FTK_ASSERT(2 == v2[1]);
                FTK_ASSERT(v2.data());
                V3F v3;
                v3[0] = 1.F;
                v3[1] = 2.F;
                v3[2] = 3.F;
                FTK_ASSERT(1.F == v3[0]);
                FTK_ASSERT(2.F == v3[1]);
                FTK_ASSERT(3.F == v3[2]);
                FTK_ASSERT(v3.data());
                V4F v4;
                v4[0] = 1.F;
                v4[1] = 2.F;
                v4[2] = 3.F;
                v4[3] = 4.F;
                FTK_ASSERT(1.F == v4[0]);
                FTK_ASSERT(2.F == v4[1]);
                FTK_ASSERT(3.F == v4[2]);
                FTK_ASSERT(4.F == v4[3]);
                FTK_ASSERT(v4.data());
            }
            {
                const V2I v2(1, 2);
                FTK_ASSERT(1 == v2.x);
                FTK_ASSERT(2 == v2.y);
                const V3F v3(1.F, 2.F, 3.F);
                FTK_ASSERT(1.F == v3.x);
                FTK_ASSERT(2.F == v3.y);
                FTK_ASSERT(3.F == v3.z);
                const V4F v4(1.F, 2.F, 3.F, 4.F);
                FTK_ASSERT(1.F == v4.x);
                FTK_ASSERT(2.F == v4.y);
                FTK_ASSERT(3.F == v4.z);
                FTK_ASSERT(4.F == v4.w);
            }
            {
                const V2F v2(Size2F(1.F, 2.F));
                FTK_ASSERT(1.F == v2.x);
                FTK_ASSERT(2.F == v2.y);
                const V3F v3(Size3F(1.F, 2.F, 3.F));
                FTK_ASSERT(1.F == v3.x);
                FTK_ASSERT(2.F == v3.y);
                FTK_ASSERT(3.F == v3.z);
            }
            {
                V2I v2;
                v2.x = 1;
                v2.y = 2;
                FTK_ASSERT(1 == v2.x);
                FTK_ASSERT(2 == v2.y);
                V3F v3;
                v3.x = 1.F;
                v3.y = 2.F;
                v3.z = 3.F;
                FTK_ASSERT(1.F == v3.x);
                FTK_ASSERT(2.F == v3.y);
                FTK_ASSERT(3.F == v3.z);
                V4F v4;
                v4.x = 1.F;
                v4.y = 2.F;
                v4.z = 3.F;
                v4.w = 4.F;
                FTK_ASSERT(1.F == v4.x);
                FTK_ASSERT(2.F == v4.y);
                FTK_ASSERT(3.F == v4.z);
                FTK_ASSERT(4.F == v4.w);
            }
        }

        void VectorTest::_functions()
        {
            {
                float l = length(V2F(0.F, 1.F));
                FTK_ASSERT(1.F == l);
                l = length(V2F(1.F, 0.F));
                FTK_ASSERT(1.F == l);
            }
            {
                V2F v = normalize(V2F(0.F, 2.F));
                FTK_ASSERT(length(v) == 1.F);
                v = normalize(V2F(2.F, 0.F));
                FTK_ASSERT(length(v) == 1.F);
            }
            {
                float d = dot(V2F(0.F, 1.F), V2F(1.F, 0.F));
                FTK_ASSERT(0.F == d);
                d = dot(V2F(0.F, 1.F), V2F(0.F, -1.F));
                FTK_ASSERT(-1.F == d);
            }
            {
                V2F v = perpCW(V2F(1.F, 0.F));
                FTK_ASSERT(V2F(0.F, -1.F) == v);
                v = perpCCW(V2F(1.F, 0.F));
                FTK_ASSERT(V2F(0.F, 1.F) == v);
            }
            {
                FTK_ASSERT(V2F(1.F, 2.F) == convert(V2I(1, 2)));
            }
        }
        
        void VectorTest::_operators()
        {
            {
                FTK_ASSERT(-V2F(1.F, 2.F) == V2F(-1.F, -2.F));
            }
            {
                V2F v = V2F(1.F, 2.F) + V2F(3.F, 4.F);
                FTK_ASSERT(V2F(4.F, 6.F) == v);
                v = V2F(1.F, 2.F) + 1.F;
                FTK_ASSERT(V2F(2.F, 3.F) == v);
            }
            {
                V2F v = V2F(3.F, 4.F) - V2F(1.F, 2.F);
                FTK_ASSERT(V2F(2.F, 2.F) == v);
                v = V2F(3.F, 4.F) - 1.F;
                FTK_ASSERT(V2F(2.F, 3.F) == v);
            }
            {
                FTK_ASSERT(V2F(1.F, 2.F) == V2F(1.F, 2.F));
                FTK_ASSERT(V2F(1.F, 2.F) != V2F(2.F, 1.F));
            }
        }
        
        void VectorTest::_serialize()
        {
            {
                const V2I v(1, 2);
                const std::string tmp = to_string(v);
                V2I v1;
                FTK_ASSERT(from_string(tmp, v1));
                FTK_ASSERT(v == v1);
            }
            {
                const V2F v(1.F, 2.F);
                const std::string tmp = to_string(v);
                V2F v1;
                FTK_ASSERT(from_string(tmp, v1));
                FTK_ASSERT(v == v1);
            }
            {
                const V3F v(1.F, 2.F, 3.F);
                const std::string tmp = to_string(v);
                V3F v1;
                FTK_ASSERT(from_string(tmp, v1));
                FTK_ASSERT(v == v1);
            }
            {
                const V4F v(1.F, 2.F, 3.F, 4.F);
                const std::string tmp = to_string(v);
                V4F v1;
                FTK_ASSERT(from_string(tmp, v1));
                FTK_ASSERT(v == v1);
            }
            {
                const V2I v(1, 2);
                nlohmann::json json;
                to_json(json, v);
                V2I v2;
                from_json(json, v2);
                FTK_ASSERT(v == v2);
            }
            {
                const V2F v(1.F, 2.F);
                nlohmann::json json;
                to_json(json, v);
                V2F v2;
                from_json(json, v2);
                FTK_ASSERT(v == v2);
            }
            {
                const V3F v(3.F, 4.F, 5.F);
                nlohmann::json json;
                to_json(json, v);
                V3F v2;
                from_json(json, v2);
                FTK_ASSERT(v == v2);
            }
            {
                const V4F v(6.F, 7.F, 8.F, 9.F);
                nlohmann::json json;
                to_json(json, v);
                V4F v2;
                from_json(json, v2);
                FTK_ASSERT(v == v2);
            }
            {
                const V2I v(1, 2);
                std::stringstream ss;
                ss << v;
            }
            {
                const V2F v(1.F, 2.F);
                std::stringstream ss;
                ss << v;
            }
            {
                const V3F v(1.F, 2.F, 3.F);
                std::stringstream ss;
                ss << v;
            }
            {
                const V4F v(1.F, 2.F, 3.F, 4.F);
                std::stringstream ss;
                ss << v;
            }
        }
    }
}

