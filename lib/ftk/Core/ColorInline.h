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
    constexpr Color<1, T>::Color(T l) :
        l(l)
    {}

    template<typename T>
    constexpr T Color<1, T>::operator [] (int) const
    {
        return l;
    }

    template<typename T>
    constexpr T& Color<1, T>::operator [] (int)
    {
        return l;
    }

    template<typename T>
    constexpr const T* Color<1, T>::data() const
    {
        return &l;
    }

    template<typename T>
    constexpr T* Color<1, T>::data()
    {
        return &l;
    }

    template<typename T>
    constexpr Color<2, T>::Color(T l, T a) :
        l(l),
        a(a)
    {}

    template<>
    constexpr Color<2, float>::Color(float l) :
        l(l),
        a(1.F)
    {}

    template<typename T>
    constexpr T Color<2, T>::operator [] (int i) const
    {
        switch (i)
        {
        case 0: return l;
        default: return a;
        }
    }

    template<typename T>
    constexpr T& Color<2, T>::operator [] (int i)
    {
        switch (i)
        {
        case 0: return l;
        default: return a;
        }
    }

    template<typename T>
    constexpr const T* Color<2, T>::data() const
    {
        return &l;
    }

    template<typename T>
    constexpr T* Color<2, T>::data()
    {
        return &l;
    }

    template<typename T>
    constexpr Color<3, T>::Color(T r, T g, T b) :
        r(r),
        g(g),
        b(b)
    {}

    template<typename T>
    constexpr T Color<3, T>::operator [] (int i) const
    {
        switch (i)
        {
        case 0: return r;
        case 1: return g;
        default: return b;
        }
    }

    template<typename T>
    constexpr T& Color<3, T>::operator [] (int i)
    {
        switch (i)
        {
        case 0: return r;
        case 1: return g;
        default: return b;
        }
    }

    template<typename T>
    constexpr const T* Color<3, T>::data() const
    {
        return &r;
    }

    template<typename T>
    constexpr T* Color<3, T>::data()
    {
        return &r;
    }

    template<typename T>
    constexpr Color<4, T>::Color(T r, T g, T b, T a) :
        r(r),
        g(g),
        b(b),
        a(a)
    {}

    template<>
    constexpr Color<4, float>::Color(float r, float g, float b) :
        r(r),
        g(g),
        b(b),
        a(1.F)
    {}

    template<typename T>
    constexpr T Color<4, T>::operator [] (int i) const
    {
        switch (i)
        {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        default: return a;
        }
    }

    template<typename T>
    constexpr T& Color<4, T>::operator [] (int i)
    {
        switch (i)
        {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        default: return a;
        }
    }

    template<typename T>
    constexpr const T* Color<4, T>::data() const
    {
        return &r;
    }

    template<typename T>
    constexpr T* Color<4, T>::data()
    {
        return &r;
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
