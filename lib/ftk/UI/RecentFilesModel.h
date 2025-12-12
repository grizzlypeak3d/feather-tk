// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>

#include <filesystem>

namespace ftk
{
    class Context;

    //! \name File Widgets
    ///@{

    //! Recent files model.
    class FTK_API_TYPE RecentFilesModel : public std::enable_shared_from_this<RecentFilesModel>
    {
        FTK_NON_COPYABLE(RecentFilesModel);

    protected:
        void _init(const std::shared_ptr<Context>&);

        RecentFilesModel();

    public:
        FTK_API ~RecentFilesModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<RecentFilesModel> create(
            const std::shared_ptr<Context>&);

        //! Get the maximum number of recent files.
        FTK_API size_t getRecentMax() const;

        //! Observe the maximum number of recent files.
        FTK_API std::shared_ptr<IObservable<size_t> > observeRecentMax() const;

        //! Set the maximum number of recent files.
        FTK_API void setRecentMax(size_t);

        //! Get the list of recent files.
        FTK_API const std::vector<std::filesystem::path>& getRecent() const;

        //! Observe the list of recent files.
        FTK_API std::shared_ptr<IObservableList<std::filesystem::path> > observeRecent() const;

        //! Set the recent files.
        FTK_API void setRecent(const std::vector<std::filesystem::path>&);

        //! Add a recent file.
        FTK_API void addRecent(const std::filesystem::path&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
