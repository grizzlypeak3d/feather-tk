// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Matrix.h>

#include <array>
#include <iostream>

namespace ftk
{
    //! \name Colors
    ///@{

    //! Color.
    template<int C, typename T>
    class Color
    {
    public:
        constexpr Color();

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();

        std::array<T, C> e;
    };

    //! One channel color (luminance).
    template<typename T>
    class Color<1, T>
    {
    public:
        constexpr Color();
        explicit constexpr Color(T);
        constexpr Color(const Color<1, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();
            
        std::array<T, 1> e;
        T& l;

        constexpr Color<1, T>& operator = (const Color<1, T>&);
    };

    //! Two channel color (luminance and alpha).
    template<typename T>
    class Color<2, T>
    {
    public:
        constexpr Color();
        explicit constexpr Color(T);
        constexpr Color(T, T);
        constexpr Color(const Color<2, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();
            
        std::array<T, 2> e;
        T& l;
        T& a;

        constexpr Color<2, T>& operator = (const Color<2, T>&);
    };

    //! Three channel color (RGB).
    template<typename T>
    class Color<3, T>
    {
    public:
        constexpr Color();
        constexpr Color(T, T, T);
        constexpr Color(const Color<3, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();
            
        std::array<T, 3> e;
        T& r;
        T& g;
        T& b;

        constexpr Color<3, T>& operator = (const Color<3, T>&);
    };

    //! Four channel color (RGBA).
    template<typename T>
    class Color<4, T>
    {
    public:
        constexpr Color();
        constexpr Color(T, T, T);
        constexpr Color(T, T, T, T);
        constexpr Color(const Color<4, T>&);

        constexpr T operator [] (int) const;
        constexpr T& operator [] (int);
            
        constexpr const T* data() const;
        constexpr T* data();
            
        std::array<T, 4> e;
        T& r;
        T& g;
        T& b;
        T& a;

        constexpr Color<4, T>& operator = (const Color<4, T>&);
    };

    typedef Color<1, float> Color1F;
    typedef Color<2, float> Color2F;
    typedef Color<3, float> Color3F;
    typedef Color<4, float> Color4F;

    //! Get a lighter color.
    template<int C>
    Color<C, float> lighter(const Color<C, float>&, float);

    //! Get a darker color.
    template<int C>
    Color<C, float> darker(const Color<C, float>&, float);

    //! Convert to greyscale.
    Color3F greyscale(const Color3F&);

    //! Convert to greyscale.
    Color4F greyscale(const Color4F&);

    //! Get a brightness color matrix.
    M44F brightness(const V3F&);

    //! Get a contrast color matrix.
    M44F contrast(const V3F&);

    //! Get a saturation color matrix.
    M44F saturation(const V3F&);

    //! Get a tint color matrix.
    M44F tint(float);

    //! Convert RGB to HSV.
    void rgbToHSV(const float in[3], float out[3]);

    //! Convert HSV to RGB.
    void hsvToRGB(const float in[3], float out[3]);

    std::string to_string(const Color1F&);
    std::string to_string(const Color2F&);
    std::string to_string(const Color3F&);
    std::string to_string(const Color4F&);

    bool from_string(const std::string&, Color1F&);
    bool from_string(const std::string&, Color2F&);
    bool from_string(const std::string&, Color3F&);
    bool from_string(const std::string&, Color4F&);

    void to_json(nlohmann::json&, const Color1F&);
    void to_json(nlohmann::json&, const Color2F&);
    void to_json(nlohmann::json&, const Color3F&);
    void to_json(nlohmann::json&, const Color4F&);

    void from_json(const nlohmann::json&, Color1F&);
    void from_json(const nlohmann::json&, Color2F&);
    void from_json(const nlohmann::json&, Color3F&);
    void from_json(const nlohmann::json&, Color4F&);
        
    template<int C, typename T>
    constexpr bool operator == (const Color<C, T>&, const Color<C, T>&);
    template<int C, typename T>
    constexpr bool operator != (const Color<C, T>&, const Color<C, T>&);
                
    template<int C, typename T>
    std::ostream& operator << (std::ostream&, const Color<C, T>&);

    ///@}
}

#include <ftk/Core/ColorInline.h>

