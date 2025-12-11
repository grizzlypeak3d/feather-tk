// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>

#include <filesystem>

namespace ftk
{
    class Context;

    //! \name File Widgets
    ///@{
        
    //! File system drives model.
    class FTK_API_TYPE DrivesModel : public std::enable_shared_from_this<DrivesModel>
    {
        FTK_NON_COPYABLE(DrivesModel);

    protected:
        void _init(const std::shared_ptr<Context>&);

        DrivesModel();

    public:
        ~DrivesModel();

        //! Create a new model.
        static std::shared_ptr<DrivesModel> create(
            const std::shared_ptr<Context>&);

        //! Observe the list of drives.
        std::shared_ptr<IObservableList<std::filesystem::path> > observeDrives() const;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
