// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the tlRender project.

#pragma once

#include <ftk/Core/ISystem.h>

#include <filesystem>

namespace ftk
{
    //! File logging system.
    class FTK_API_TYPE FileLogSystem : public ISystem
    {
        FTK_NON_COPYABLE(FileLogSystem);

    protected:
        FileLogSystem(
            const std::shared_ptr<Context>&,
            const std::filesystem::path&);

    public:
        FTK_API virtual ~FileLogSystem();

        //! Create a new system.
        FTK_API static std::shared_ptr<FileLogSystem> create(
            const std::shared_ptr<Context>&,
            const std::filesystem::path&);

    private:
        FTK_PRIVATE();
    };
}
