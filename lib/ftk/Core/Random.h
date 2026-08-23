// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/Util.h>

#include <memory>
#include <vector>

namespace ftk
{
    //! Random numbers.
    class FTK_CORE_API_TYPE Random
    {
    public:
        FTK_CORE_API Random();

        FTK_CORE_API ~Random();

        //! Get a random number between zero and one.
        FTK_CORE_API float getF();

        //! Get a random number between zero and the given value.
        FTK_CORE_API float getF(float);

        //! Get a random number between two values.
        FTK_CORE_API float getF(float min, float max);

        //! Get a random number between zero and the given value.
        FTK_CORE_API int getI(int);

        //! Get a random number between two values.
        FTK_CORE_API int getI(int min, int max);

        //! Get a random item from a list.
        template<typename T>
        const T& getItem(const std::vector<T>&);

        //! Seed the random value generator.
        FTK_CORE_API void setSeed(unsigned int);

        //! Seed the random value generator with the current time.
        FTK_CORE_API void setSeed();

    private:
        FTK_PRIVATE();
    };
}

#include <ftk/Core/RandomInline.h>
