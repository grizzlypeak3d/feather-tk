// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Size.h>

namespace ftk
{
    //! \name Vectors
    ///@{
        
    //! Base class for vectors.
    template<int C, typename T>
    class Vector
    {
    public:
        constexpr Vector();
        explicit constexpr Vector(const Size<C, T>&);
            
        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();

        std::array<T, C> e;
    };

    //! Two-dimensional vector.
    template<typename T>
    class Vector<2, T>
    {
    public:
        constexpr Vector();
        constexpr Vector(T, T);
        explicit constexpr Vector(const Size<2, T>&);
        constexpr Vector(const Vector<2, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();

        std::array<T, 2> e;
        T& x;
        T& y;
            
        constexpr Vector<2, T>& operator = (const Vector<2, T>&);
    };

    //! Three-dimensional vector.
    template<typename T>
    class Vector<3, T>
    {
    public:
        constexpr Vector();
        constexpr Vector(T, T, T);
        explicit constexpr Vector(const Size<3, T>&);
        constexpr Vector(const Vector<3, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();

        std::array<T, 3> e;
        T& x;
        T& y;
        T& z;

        constexpr Vector<3, T>& operator = (const Vector<3, T>&);
    };

    //! Four-dimensional vector.
    template<typename T>
    class Vector<4, T>
    {
    public:
        constexpr Vector();
        constexpr Vector(T, T, T, T = T(1));
        constexpr Vector(const Vector<4, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);

        constexpr const T* data() const;
        constexpr T* data();

        std::array<T, 4> e;
        T& x;
        T& y;
        T& z;
        T& w;

        constexpr Vector<4, T>& operator = (const Vector<4, T>&);
    };

    typedef Vector<2, int> V2I;
    typedef Vector<2, float> V2F;
    typedef Vector<3, float> V3F;
    typedef Vector<4, float> V4F;
                
    //! Get the length of a vector.
    template<int C, typename T>
    T length(const Vector<C, T>&);
        
    //! Normalize the given vector.
    template<int C, typename T>
    Vector<C, T> normalize(const Vector<C, T>&);        
        
    //! Get the vector dot product.
    template<int C, typename T>
    constexpr T dot(const Vector<C, T>&, const Vector<C, T>&);
        
    //! Get the vector cross product.
    template<typename T>
    constexpr Vector<3, T> cross(const Vector<3, T>&, const Vector<3, T>&);

    //! Get a vector perpindicular to the given vector in the clockwise
    //! direction.
    template<typename T>
    constexpr Vector<2, T> perpCW(const Vector<2, T>&);

    //! Get a vector perpindicular to the given vector in the
    //! counter-clockwise direction.
    template<typename T>
    constexpr Vector<2, T> perpCCW(const Vector<2, T>&);

    //! Convert vector types.
    constexpr Vector<2, float> convert(const Vector<2, int>&);

    //! Round vector components.
    Vector<2, float> round(const Vector<2, float>&);

    //! Round vector components.
    Vector<2, float> floor(const Vector<2, float>&);

    //! Round vector components.
    Vector<2, float> ceil(const Vector<2, float>&);

    std::string to_string(const V2I&);
    std::string to_string(const V2F&);
    std::string to_string(const V3F&);
    std::string to_string(const V4F&);

    bool from_string(const std::string&, V2I&);
    bool from_string(const std::string&, V2F&);
    bool from_string(const std::string&, V3F&);
    bool from_string(const std::string&, V4F&);

    void to_json(nlohmann::json&, const V2I&);
    void to_json(nlohmann::json&, const V2F&);
    void to_json(nlohmann::json&, const V3F&);
    void to_json(nlohmann::json&, const V4F&);

    void from_json(const nlohmann::json&, V2I&);
    void from_json(const nlohmann::json&, V2F&);
    void from_json(const nlohmann::json&, V3F&);
    void from_json(const nlohmann::json&, V4F&);
        
    template<int C, typename T>
    constexpr Vector<C, T> operator + (const Vector<C, T>&, const Vector<C, T>&);
    template<int C, typename T>
    constexpr Vector<C, T> operator + (const Vector<C, T>&, T);
        
    template<int C, typename T>
    constexpr Vector<C, T> operator - (const Vector<C, T>&, const Vector<C, T>&);
    template<int C, typename T>
    constexpr Vector<C, T> operator - (const Vector<C, T>&);
    template<int C, typename T>
    constexpr Vector<C, T> operator - (const Vector<C, T>&, T);

    template<int C, typename T>
    constexpr Vector<C, T> operator * (const Vector<C, T>&, float);
    template<int C, typename T>
    constexpr Vector<C, T> operator / (const Vector<C, T>&, float);
        
    template<int C, typename T>
    constexpr bool operator == (const Vector<C, T>&, const Vector<C, T>&);
    template<int C, typename T>
    constexpr bool operator != (const Vector<C, T>&, const Vector<C, T>&);

    template<int C, typename T>
    std::ostream& operator << (std::ostream&, const Vector<C, T>&);

    ///@}
}

#include <ftk/Core/VectorInline.h>

