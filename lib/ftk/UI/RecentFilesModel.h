// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>
#include <ftk/Core/Path.h>

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
        //!
        //! Paths rather than file names: a sequence's range is the path's own
        //! and is what says whether the entry was one frame or the sequence
        //! it sits in, so opening it again opens what it opened before.
        FTK_API const std::vector<Path>& getRecent() const;

        //! Observe the list of recent files.
        FTK_API std::shared_ptr<IObservableList<Path> > observeRecent() const;

        //! Set the recent files.
        FTK_API void setRecent(const std::vector<Path>&);

        //! Add a recent file.
        FTK_API void addRecent(const Path&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
