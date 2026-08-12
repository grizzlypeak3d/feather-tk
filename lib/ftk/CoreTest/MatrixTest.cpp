// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/MatrixTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Matrix.h>

#include <cmath>
#include <sstream>
#include <vector>

namespace ftk
{
    namespace core_test
    {
        MatrixTest::MatrixTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::MatrixTest")
        {}

        MatrixTest::~MatrixTest()
        {}

        std::shared_ptr<MatrixTest> MatrixTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<MatrixTest>(new MatrixTest(context));
        }
        
        void MatrixTest::run()
        {
            _members();
            _functions();
            _rotateXYZ();
            _operators();
            _serialize();
        }
        
        void MatrixTest::_members()
        {
            {
                const Matrix<2, 2, float> m;
                FTK_CHECK(0.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                Matrix<2, 2, float> m;
                FTK_CHECK(0.F == m.get(0, 0));
                m.set(0, 0, 1.F);
                FTK_CHECK(1.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                const M33F m;
                FTK_CHECK(1.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                M33F m;
                FTK_CHECK(1.F == m.get(0, 0));
                m.set(0, 0, 0.F);
                FTK_CHECK(0.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                const M33F m(
                    1.F, 2.F, 3.F,
                    4.F, 5.F, 6.F,
                    7.F, 8.F, 9.F);
                FTK_CHECK(9.F == m.get(2, 2));
            }
            {
                const M44F m;
                FTK_CHECK(1.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                M44F m;
                FTK_CHECK(1.F == m.get(0, 0));
                m.set(0, 0, 0.F);
                FTK_CHECK(0.F == m.get(0, 0));
                FTK_CHECK(m.data());
            }
            {
                const M44F m(
                    1.F, 2.F, 3.F, 4.F,
                    5.F, 6.F, 7.F, 8.F,
                    9.F, 10.F, 11.F, 12.F,
                    13.F, 14.F, 15.F, 16.F);
                FTK_CHECK(16.F == m.get(3, 3));
            }
        }

        void MatrixTest::_functions()
        {
            translate(V3F(1.F, 1.F, 1.F));
            rotateX(90.F);
            rotateY(90.F);
            rotateZ(90.F);
            scale(V3F(1.F, 2.F, 3.F));
            ortho(0.F, 1.F, 0.F, 1.F, .1F, 10000.F);
            perspective(60.F, 1.F, .1F, 10000.F);
        }
        
        void MatrixTest::_rotateXYZ()
        {
            // Rotations are compared through their matrices, never angles
            // against angles: the same rotation has more than one set of
            // angles, so two sets that differ are not two rotations that
            // differ, and a test that says otherwise fails on correct
            // answers.
            const auto same = [](const M44F& a, const M44F& b) -> bool
            {
                for (int row = 0; row < 3; ++row)
                {
                    for (int column = 0; column < 3; ++column)
                    {
                        if (std::abs(a.get(row, column) -
                            b.get(row, column)) > .001F)
                        {
                            return false;
                        }
                    }
                }
                return true;
            };
            const auto close = [](float a, float b, float tolerance = .001F)
            {
                return std::abs(a - b) < tolerance;
            };

            // About each of the three axes in turn, the general one has to
            // agree with the one written out by hand. Three chances to have
            // a sign the wrong way round, and each of the three catches a
            // different pair of elements.
            for (float angle : { 30.F, -30.F, 90.F, 170.F })
            {
                FTK_CHECK(same(
                    rotate(angle, V3F(1.F, 0.F, 0.F)), rotateX(angle)));
                FTK_CHECK(same(
                    rotate(angle, V3F(0.F, 1.F, 0.F)), rotateY(angle)));
                FTK_CHECK(same(
                    rotate(angle, V3F(0.F, 0.F, 1.F)), rotateZ(angle)));
                // Length is not meant to matter, and no axis at all is the
                // identity rather than a division by zero.
                FTK_CHECK(same(
                    rotate(angle, V3F(0.F, 5.F, 0.F)), rotateY(angle)));
            }
            FTK_CHECK(rotate(90.F, V3F(0.F, 0.F, 0.F)) == M44F());

            FTK_CHECK(rotateXYZ(V3F(90.F, 0.F, 0.F)) == rotateX(90.F));
            FTK_CHECK(rotateXYZ(V3F(0.F, 90.F, 0.F)) == rotateY(90.F));
            FTK_CHECK(rotateXYZ(V3F(0.F, 0.F, 90.F)) == rotateZ(90.F));

            // Round trips: build a rotation, read the angles back, build it
            // again. Asked from where it already is, the answer is where it
            // is -- nothing reading its own angles back should see them
            // rewritten as a different description of the same pose.
            const std::vector<V3F> angles =
            {
                V3F(0.F, 0.F, 0.F),
                V3F(30.F, 0.F, 0.F),
                V3F(0.F, 30.F, 0.F),
                V3F(0.F, 0.F, 30.F),
                V3F(30.F, 40.F, 50.F),
                V3F(-30.F, -40.F, -50.F),
                V3F(170.F, 10.F, -170.F),
                V3F(10.F, 130.F, 20.F),
                V3F(-95.F, 85.F, 175.F)
            };
            for (const auto& i : angles)
            {
                const M44F m = rotateXYZ(i);
                const V3F back = getRotateXYZ(m, i);
                FTK_CHECK(same(m, rotateXYZ(back)));
                FTK_CHECK(close(back.x, i.x));
                FTK_CHECK(close(back.y, i.y));
                FTK_CHECK(close(back.z, i.z));
            }

            // A full turn about Z a degree at a time, the way a drag arrives.
            // It has to come out at 360 rather than back at zero: whatever is
            // reading these should be able to see that it has been round.
            V3F previous;
            for (int i = 1; i <= 360; ++i)
            {
                previous = getRotateXYZ(
                    rotateZ(static_cast<float>(i)), previous);
            }
            FTK_CHECK(close(previous.z, 360.F, .01F));
            for (int i = 359; i >= 0; --i)
            {
                previous = getRotateXYZ(
                    rotateZ(static_cast<float>(i)), previous);
            }
            FTK_CHECK(close(previous.z, 0.F, .01F));

            // Past where the Y angle folds. asin only reaches a quarter turn,
            // so carrying on needs the other set of angles; taking the nearer
            // one has to hold the pose steady whatever the three numbers do.
            previous = V3F(0.F, 0.F, 0.F);
            for (int i = 1; i <= 180; ++i)
            {
                const M44F m = rotateY(static_cast<float>(i));
                previous = getRotateXYZ(m, previous);
                FTK_CHECK(same(m, rotateXYZ(previous)));
            }

            // Straight up and straight down, where only the sum of the X and
            // Z rotations is decided. Any answer will do as long as it builds
            // the rotation asked for.
            for (float y : { 90.F, -90.F })
            {
                for (float z : { 0.F, 45.F, -120.F })
                {
                    const M44F m = rotateXYZ(V3F(0.F, y, z));
                    FTK_CHECK(same(m, rotateXYZ(getRotateXYZ(m, V3F(0.F, y, z)))));
                }
            }
        }

        void MatrixTest::_operators()
        {
            {
                const auto m33 = translate(V2F(-1.F, -2.F));
                const auto v2 = m33 * V2F(1.F, 2.f);
                FTK_CHECK(v2 == V2F(0.F, 0.F));
            }
            {
                const auto m44 = translate(V3F(-1.F, -2.F, -3.F));
                const auto v3 = m44 * V3F(1.F, 2.F, 3.F);
                FTK_CHECK(v3 == V3F(0.F, 0.F, 0.F));
            }
            {
                const auto m44 = rotateX(90.F);
                const auto v4 = m44 * V4F(0.F, 0.F, 1.F);
                FTK_CHECK(fuzzyCompare(v4.x, 0.F));
                FTK_CHECK(fuzzyCompare(v4.y, -1.F));
                FTK_CHECK(fuzzyCompare(v4.z, 0.F));
                FTK_CHECK(fuzzyCompare(v4.w, 1.F));
            }
            {
                const auto m44 = rotateY(90.F);
                const auto v4 = m44 * V4F(1.F, 0.F, 0.F);
                FTK_CHECK(fuzzyCompare(v4.x, 0.F));
                FTK_CHECK(fuzzyCompare(v4.y, 0.F));
                FTK_CHECK(fuzzyCompare(v4.z, -1.F));
                FTK_CHECK(fuzzyCompare(v4.w, 1.F));
            }
            {
                const auto m44 = rotateZ(90.F);
                const auto v4 = m44 * V4F(1.F, 0.F, 0.F);
                FTK_CHECK(fuzzyCompare(v4.x, 0.F));
                FTK_CHECK(fuzzyCompare(v4.y, 1.F));
                FTK_CHECK(fuzzyCompare(v4.z, 0.F));
                FTK_CHECK(fuzzyCompare(v4.w, 1.F));
            }
            {
                M44F a;
                M44F b;
                FTK_CHECK(a == b);
                b.set(0, 0, 0.F);
                FTK_CHECK(a != b);                
            }
        }
        
        void MatrixTest::_serialize()
        {
            {
                const M33F m;
                nlohmann::json json;
                to_json(json, m);
                M33F m1;
                from_json(json, m1);
                FTK_CHECK(m == m1);
            }
            {
                const M44F m;
                nlohmann::json json;
                to_json(json, m);
                M44F m1;
                from_json(json, m1);
                FTK_CHECK(m == m1);
            }
            {
                const M33F m;
                std::stringstream ss;
                ss << m;
            }
            {
                const M44F m;
                std::stringstream ss;
                ss << m;
            }
        }
    }
}

