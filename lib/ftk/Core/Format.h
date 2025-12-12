// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Export.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ftk
{
    //! \name String Formatting
    ///@{
        
    //! String formatting.
    //!
    //! Example:
    //! std::string result = Format("Testing {2} {1} {0}").arg("one").arg("two").arg("three");
    //!
    //! Results in the string "Testing three two one".
    class FTK_API_TYPE Format
    {
    public:
        FTK_API Format(const std::string&);

        //! \name Arguments
        //! Replace the next argument in the string with the given value.
        //! Arguments consist of an integer enclosed by curly brackets (eg., "{0}").
        //! The argument with the smallest integer will be replaced. The
        //! object is returned so that you can chain calls together.
        ///@{

        FTK_API Format& arg(const std::string&, int width = 0);
        FTK_API Format& arg(int, int width = 0, char pad = ' ');
        FTK_API Format& arg(int8_t, int width = 0, char pad = ' ');
        FTK_API Format& arg(uint8_t, int width = 0, char pad = ' ');
        FTK_API Format& arg(int16_t, int width = 0, char pad = ' ');
        FTK_API Format& arg(uint16_t, int width = 0, char pad = ' ');
        FTK_API Format& arg(float, int precision = -1, int width = 0, char pad = ' ');
        FTK_API Format& arg(double, int precision = -1, int width = 0, char pad = ' ');
        template<typename T>
        Format& arg(T);

        ///@}
            
        //! Get the string.
        const std::string& str() const;

        //! Does the formatting have an error?
        bool hasError() const;
            
        //! Get the error.
        const std::string& getError() const;

        operator std::string() const;

    private:
        std::string _text;
        std::string _error;
    };
        
    FTK_API std::ostream& operator << (std::ostream&, const Format&);
        
    ///@}
}

#include <ftk/Core/FormatInline.h>

