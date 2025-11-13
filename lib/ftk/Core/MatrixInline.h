// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Math.h>

#include <cmath>

namespace ftk
{
    template<int R, int C, typename T>
    constexpr Matrix<R, C, T>::Matrix()
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                _e[r * C + c] = T(0);
            }
        }            
    }

    template<int R, int C, typename T>
    constexpr T Matrix<R, C, T>::get(int row, int column) const
    {
        return _e[row * C + column];
    }

    template<int R, int C, typename T>
    constexpr void Matrix<R, C, T>::set(int row, int column, T v)
    {
        _e[row * C + column] = v;
    }

    template<int R, int C, typename T>
    constexpr T Matrix<R, C, T>::operator [] (int index) const
    {
        return _e[index];
    }

    template<int R, int C, typename T>
    constexpr T& Matrix<R, C, T>::operator [] (int index)
    {
        return _e[index];
    }
            
    template<int R, int C, typename T>
    constexpr const T* Matrix<R, C, T>::data() const
    {
        return _e.data();
    }

    template<int R, int C, typename T>
    constexpr T* Matrix<R, C, T>::data()
    {
        return _e.data();
    }

    template<typename T>
    constexpr Matrix<3, 3, T>::Matrix() :
        _e({
            T(1), T(0), T(0),
            T(0), T(1), T(0),
            T(0), T(0), T(1)
        })
    {}

    template<typename T>
    constexpr Matrix<3, 3, T>::Matrix(
        T e_0_0, T e_0_1, T e_0_2,
        T e_1_0, T e_1_1, T e_1_2,
        T e_2_0, T e_2_1, T e_2_2) :
        _e({
            e_0_0, e_0_1, e_0_2,
            e_1_0, e_1_1, e_1_2,
            e_2_0, e_2_1, e_2_2
        })
    {}

    template<typename T>
    constexpr T Matrix<3, 3, T>::get(int row, int column) const
    {
        return _e[row * 3 + column];
    }

    template<typename T>
    constexpr void Matrix<3, 3, T>::set(int row, int column, T v)
    {
        _e[row * 3 + column] = v;
    }

    template<typename T>
    constexpr T Matrix<3, 3, T>::operator [] (int index) const
    {
        return _e[index];
    }

    template<typename T>
    constexpr T& Matrix<3, 3, T>::operator [] (int index)
    {
        return _e[index];
    }
            
    template<typename T>
    constexpr const T* Matrix<3, 3, T>::data() const
    {
        return _e.data();
    }

    template<typename T>
    constexpr T* Matrix<3, 3, T>::data()
    {
        return _e.data();
    }

    template<typename T>
    constexpr Matrix<4, 4, T>::Matrix() :
        _e({
            T(1), T(0), T(0), T(0),
            T(0), T(1), T(0), T(0),
            T(0), T(0), T(1), T(0),
            T(0), T(0), T(0), T(1)
        })
    {}

    template<typename T>
    constexpr Matrix<4, 4, T>::Matrix(
        T e_0_0, T e_0_1, T e_0_2, T e_0_3,
        T e_1_0, T e_1_1, T e_1_2, T e_1_3,
        T e_2_0, T e_2_1, T e_2_2, T e_2_3,
        T e_3_0, T e_3_1, T e_3_2, T e_3_3) :
        _e({
            e_0_0, e_0_1, e_0_2, e_0_3,
            e_1_0, e_1_1, e_1_2, e_1_3,
            e_2_0, e_2_1, e_2_2, e_2_3,
            e_3_0, e_3_1, e_3_2, e_3_3
        })
    {}

    template<typename T>
    constexpr T Matrix<4, 4, T>::get(int row, int column) const
    {
        return _e[row * 4 + column];
    }

    template<typename T>
    constexpr void Matrix<4, 4, T>::set(int row, int column, T v)
    {
        _e[row * 4 + column] = v;
    }

    template<typename T>
    constexpr T Matrix<4, 4, T>::operator [] (int index) const
    {
        return _e[index];
    }

    template<typename T>
    constexpr T& Matrix<4, 4, T>::operator [] (int index)
    {
        return _e[index];
    }

    template<typename T>
    constexpr const T* Matrix<4, 4, T>::data() const
    {
        return _e.data();
    }

    template<typename T>
    constexpr T* Matrix<4, 4, T>::data()
    {
        return _e.data();
    }

    template<typename T>
    constexpr Matrix<3, 3, T> translate(const Vector<2, T>& value)
    {
        return Matrix<3, 3, T>(
            T(1), T(0), value.x,
            T(0), T(1), value.y,
            T(0), T(0), T(1));
    }

    template<typename T>
    constexpr Matrix<4, 4, T> translate(const Vector<3, T>& value)
    {
        return Matrix<4, 4, T>(
            T(1), T(0), T(0), value.x,
            T(0), T(1), T(0), value.y,
            T(0), T(0), T(1), value.z,
            T(0), T(0), T(0), T(1));
    }

    template<typename T>
    inline Matrix<4, 4, T> rotateX(T angle)
    {
        const T a = std::cos(deg2rad(angle));
        const T b = std::sin(deg2rad(angle));
        return Matrix<4, 4, T>(
            T(1), T(0), T(0), T(0),
            T(0),    a,   -b, T(0),
            T(0),    b,    a, T(0),
            T(0), T(0), T(0), T(1));
    }

    template<typename T>
    inline Matrix<4, 4, T> rotateY(T angle)
    {
        const T a = std::cos(deg2rad(angle));
        const T b = std::sin(deg2rad(angle));
        return Matrix<4, 4, T>(
               a, T(0),    b, T(0),
            T(0), T(1), T(0), T(0),
              -b, T(0),    a, T(0),
            T(0), T(0), T(0), T(1));
    }

    template<typename T>
    inline Matrix<4, 4, T> rotateZ(T angle)
    {
        const T a = std::cos(deg2rad(angle));
        const T b = std::sin(deg2rad(angle));
        return Matrix<4, 4, T>(
               a,   b,  T(0), T(0),
              -b,   a,  T(0), T(0),
            T(0), T(0), T(1), T(0),
            T(0), T(0), T(0), T(1));
    }

    template<typename T>
    constexpr Matrix<4, 4, T> scale(const Vector<3, T>& value)
    {
        return Matrix<4, 4, T>(
            value.x, T(0),    T(0),    T(0),
            T(0),    value.y, T(0),    T(0),
            T(0),    T(0),    value.z, T(0),
            T(0),    T(0),    T(0),    T(1));
    }

    template<typename T>
    constexpr Matrix<4, 4, T> ortho(T left, T right, T bottom, T top, T nearClip, T farClip)
    {
        const T a = T(2) / (right - left);
        const T b = T(2) / (top - bottom);
        const T c = T(-2) / (farClip - nearClip);
        const T x = -(right + left) / (right - left);
        const T y = -(top + bottom) / (top - bottom);
        const T z = -(farClip + nearClip) / (farClip - nearClip);
        return Matrix<4, 4, T>(
               a, T(0), T(0),    x,
            T(0),    b, T(0),    y,
            T(0), T(0),    c,    z,
            T(0), T(0), T(0), T(1));
    }

    template<typename T>
    constexpr Matrix<4, 4, T> perspective(T fov, T aspect, T nearClip, T farClip)
    {
        const T t = std::tan(deg2rad(fov) / T(2));
        const T top = nearClip * t;
        const T right = top * aspect;
        const T a = nearClip / right;
        const T b = nearClip / top;
        const T c = -(farClip + nearClip) / (farClip - nearClip);
        const T d = -(2 * farClip * nearClip) / (farClip - nearClip);
        return Matrix<4, 4, T>(
               a, T(0),  T(0),  T(0),
            T(0),    b,  T(0),  T(0),
            T(0), T(0),     c,     d,
            T(0), T(0), T(-1), T(0));
    }

    template<typename T>
    bool invert(const Matrix<4, 4, T>& m, Matrix<4, 4, T>& out)
    {
        Matrix<4, 4, T> inv;
        inv[0] =
            m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        inv[4] =
            -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        inv[8] =
            m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        inv[12] =
            -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        inv[1] =
            -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        inv[5] =
            m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        inv[9] =
            -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        inv[13] =
            m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        inv[2] =
            m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        inv[6] =
            -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        inv[10] =
            m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        inv[14] =
            -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        inv[3] =
            -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        inv[7] =
            m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        inv[11] =
            -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        inv[15] =
            m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        T det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        const bool r = det != T(0);
        if (r)
        {
            det = 1.0 / det;
            for (int i = 0; i < 16; ++i)
            {
                out[i] = inv[i] * det;
            }
        }
        return r;
    }

    template<typename T>
    inline Matrix<3, 3, T> operator * (const Matrix<3, 3, T>& a, const Matrix<3, 3, T>& b)
    {
        Matrix<3, 3, T> out;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                float tmp = 0.F;
                for (int k = 0; k < 3; ++k)
                {
                    tmp += a.get(i, k) * b.get(k, j);
                }
                out.set(i, j, tmp);
            }
        }
        return out;
    }

    template<typename T>
    inline Vector<2, T> operator * (const Matrix<3, 3, T>& m, const Vector<2, T>& v)
    {
        const T x = v[0] * m[0] + v[1] * m[1] + m[2];
        const T y = v[0] * m[3] + v[1] * m[4] + m[5];
        return Vector<2, T>(x, y);
    }

    template<typename T>
    inline Matrix<4, 4, T> operator * (const Matrix<4, 4, T>& a, const Matrix<4, 4, T>& b)
    {
        Matrix<4, 4, T> out;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                float tmp = 0.F;
                for (int k = 0; k < 4; ++k)
                {
                    tmp += a.get(i, k) * b.get(k, j);
                }
                out.set(i, j, tmp);
            }
        }
        return out;
    }

    template<typename T>
    inline Vector<3, T> operator * (const Matrix<4, 4, T>& m, const Vector<3, T>& v)
    {
        const T x = v[0] * m[ 0] + v[1] * m[ 1] + v[2] * m[ 2] + m[ 3];
        const T y = v[0] * m[ 4] + v[1] * m[ 5] + v[2] * m[ 6] + m[ 7];
        const T z = v[0] * m[ 8] + v[1] * m[ 9] + v[2] * m[10] + m[11];
        const T w = v[0] * m[12] + v[1] * m[13] + v[2] * m[14] + m[15];
        return Vector<3, T>(x / w, y / w, z / w);
    }

    template<typename T>
    inline Vector<4, T> operator * (const Matrix<4, 4, T>& m, const Vector<4, T>& v)
    {
        Vector<4, T> out;
        out.x = v[0] * m[ 0] + v[1] * m[ 1] + v[2] * m[ 2] + v[3] * m[ 3];
        out.y = v[0] * m[ 4] + v[1] * m[ 5] + v[2] * m[ 6] + v[3] * m[ 7];
        out.z = v[0] * m[ 8] + v[1] * m[ 9] + v[2] * m[10] + v[3] * m[11];
        out.w = v[0] * m[12] + v[1] * m[13] + v[2] * m[14] + v[3] * m[15];
        return out;
    }
        
    template<int R, int C, typename T>
    constexpr bool operator == (const Matrix<R, C, T>& a, const Matrix<R, C, T>& b)
    {
        bool out = true;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                out &= a.get(r, c) == b.get(r, c);
            }
        }
        return out;
    }
        
    template<int R, int C, typename T>
    constexpr bool operator != (const Matrix<R, C, T>& a, const Matrix<R, C, T>& b)
    {
        return !(a == b);
    }
        
    template<int R, int C, typename T>
    inline std::ostream& operator << (std::ostream& os, const Matrix<R, C, T>& m)
    {
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                os << m.get(r, c);
                if (r * C + c < R * C - 1)
                {
                    os << " ";
                }
            }
        }
        return os;
    }
}
