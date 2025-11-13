// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    template<int C, typename T>
    constexpr Color<C, T>::Color()
    {
        for (int c = 0; c < C; ++c)
        {
            e[c] = T(0);
        }
    }

    template<int C, typename T>
    constexpr T Color<C, T>::operator [] (int i) const
    {
        return e[i];
    }

    template<int C, typename T>
    constexpr T& Color<C, T>::operator [] (int i)
    {
        return e[i];
    }

    template<int C, typename T>
    constexpr const T* Color<C, T>::data() const
    {
        return e.data();
    }

    template<int C, typename T>
    constexpr T* Color<C, T>::data()
    {
        return e.data();
    }

    template<typename T>
    constexpr Color<1, T>::Color() :
        e({ 0 }),
        l(e[0])
    {}

    template<typename T>
    constexpr Color<1, T>::Color(T l) :
        e({ l }),
        l(e[0])
    {}

    template<typename T>
    constexpr Color<1, T>::Color(const Color<1, T>& v) :
        e(v.e),
        l(e[0])
    {}

    template<typename T>
    constexpr T Color<1, T>::operator [] (int i) const
    {
        return e[i];
    }

    template<typename T>
    constexpr T& Color<1, T>::operator [] (int i)
    {
        return e[i];
    }

    template<typename T>
    constexpr const T* Color<1, T>::data() const
    {
        return e.data();
    }

    template<typename T>
    constexpr T* Color<1, T>::data()
    {
        return e.data();
    }

    template<typename T>
    constexpr Color<1, T>& Color<1, T>::operator = (const Color<1, T>& v)
    {
        e = v.e;
        return *this;
    }

    template<typename T>
    constexpr Color<2, T>::Color() :
        e({ 0, 0 }),
        l(e[0]),
        a(e[1])
    {}

    template<typename T>
    constexpr Color<2, T>::Color(T l, T a) :
        e({ l, a }),
        l(e[0]),
        a(e[1])
    {}

    template<>
    constexpr Color<2, float>::Color(float l) :
        e({ l, 1.F }),
        l(e[0]),
        a(e[1])
    {}

    template<typename T>
    constexpr Color<2, T>::Color(const Color<2, T>& v) :
        e(v.e),
        l(e[0]),
        a(e[1])
    {}

    template<typename T>
    constexpr T Color<2, T>::operator [] (int i) const
    {
        return e[i];
    }

    template<typename T>
    constexpr T& Color<2, T>::operator [] (int i)
    {
        return e[i];
    }

    template<typename T>
    constexpr const T* Color<2, T>::data() const
    {
        return e.data();
    }

    template<typename T>
    constexpr T* Color<2, T>::data()
    {
        return e.data();
    }

    template<typename T>
    constexpr Color<2, T>& Color<2, T>::operator = (const Color<2, T>& v)
    {
        e = v.e;
        return *this;
    }

    template<typename T>
    constexpr Color<3, T>::Color() :
        e({ 0, 0, 0 }),
        r(e[0]),
        g(e[1]),
        b(e[2])
    {}

    template<typename T>
    constexpr Color<3, T>::Color(T r, T g, T b) :
        e({ r, g, b }),
        r(e[0]),
        g(e[1]),
        b(e[2])
    {}

    template<typename T>
    constexpr Color<3, T>::Color(const Color<3, T>& v) :
        e(v.e),
        r(e[0]),
        g(e[1]),
        b(e[2])
    {}

    template<typename T>
    constexpr T Color<3, T>::operator [] (int i) const
    {
        return e[i];
    }

    template<typename T>
    constexpr T& Color<3, T>::operator [] (int i)
    {
        return e[i];
    }

    template<typename T>
    constexpr const T* Color<3, T>::data() const
    {
        return e.data();
    }

    template<typename T>
    constexpr T* Color<3, T>::data()
    {
        return e.data();
    }

    template<typename T>
    constexpr Color<3, T>& Color<3, T>::operator = (const Color<3, T>& v)
    {
        e = v.e;
        return *this;
    }

    template<typename T>
    constexpr Color<4, T>::Color() :
        e({ 0, 0, 0, 0 }),
        r(e[0]),
        g(e[1]),
        b(e[2]),
        a(e[3])
    {}

    template<typename T>
    constexpr Color<4, T>::Color(T r, T g, T b, T a) :
        e({ r, g, b, a }),
        r(e[0]),
        g(e[1]),
        b(e[2]),
        a(e[3])
    {}

    template<>
    constexpr Color<4, float>::Color(float r, float g, float b) :
        e({ r, g, b, 1.F }),
        r(e[0]),
        g(e[1]),
        b(e[2]),
        a(e[3])
    {}

    template<typename T>
    constexpr Color<4, T>::Color(const Color<4, T>& v) :
        e(v.e),
        r(e[0]),
        g(e[1]),
        b(e[2]),
        a(e[3])
    {}

    template<typename T>
    constexpr T Color<4, T>::operator [] (int i) const
    {
        return e[i];
    }

    template<typename T>
    constexpr T& Color<4, T>::operator [] (int i)
    {
        return e[i];
    }

    template<typename T>
    constexpr const T* Color<4, T>::data() const
    {
        return e.data();
    }

    template<typename T>
    constexpr T* Color<4, T>::data()
    {
        return e.data();
    }

    template<typename T>
    constexpr Color<4, T>& Color<4, T>::operator = (const Color<4, T>& v)
    {
        e = v.e;
        return *this;
    }

    inline Color1F lighter(const Color1F& color, float value)
    {
        return Color1F(color.l + value);
    }

    inline Color2F lighter(const Color2F& color, float value)
    {
        return Color2F(color.l + value, color.a);
    }

    inline Color3F lighter(const Color3F& color, float value)
    {
        return Color3F(color.r + value, color.g + value, color.b + value);
    }

    inline Color4F lighter(const Color4F& color, float value)
    {
        return Color4F(color.r + value, color.g + value, color.b + value, color.a);
    }

    inline Color1F darker(const Color1F& color, float value)
    {
        return Color1F(color.l - value);
    }

    inline Color2F darker(const Color2F& color, float value)
    {
        return Color2F(color.l - value, color.a);
    }

    inline Color3F darker(const Color3F& color, float value)
    {
        return Color3F(color.r - value, color.g - value, color.b - value);
    }

    inline Color4F darker(const Color4F& color, float value)
    {
        return Color4F(color.r - value, color.g - value, color.b - value, color.a);
    }

    inline Color3F greyscale(const Color3F& value)
    {
        const float l = (value.r + value.g + value.b) / 3.F;
        return Color3F(l, l, l);
    }

    inline Color4F greyscale(const Color4F& value)
    {
        const float l = (value.r + value.g + value.b) / 3.F;
        return Color4F(l, l, l, value.a);
    }

    template<int C, typename T>
    constexpr bool operator == (const Color<C, T>& a, const Color<C, T>& b)
    {
        bool out = true;
        for (int c = 0; c < C; ++c)
        {
            out &= a[c] == b[c];
        }
        return out;
    }
        
    template<int C, typename T>
    constexpr bool operator != (const Color<C, T>& a, const Color<C, T>& b)
    {
        return !(a == b);
    }
        
    template<int C, typename T>
    inline std::ostream& operator << (std::ostream& os, const Color<C, T>& v)
    {
        os << to_string(v);
        return os;
    }
}
