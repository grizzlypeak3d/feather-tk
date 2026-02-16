// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableMap.h>

#include <iostream>
#include <string>
#include <vector>

namespace ftk
{
    class Context;

    //! \name Debugging
    ///@{

    //! Diagnostics data.
    enum class FTK_API_TYPE DiagData
    {
        GLBuffers,
        GLBuffersMB,
        GLMeshes,
        GLShaders,
        GLMeshesMB,
        GLTextures,
        GLTexturesMB,
        Images,
        ImagesMB,
        Widgets,

        Count,
        First = GLBuffers
    };
    FTK_ENUM(DiagData);

    //! Diagnostics model.
    class FTK_API_TYPE DiagModel : public std::enable_shared_from_this<DiagModel>
    {
        FTK_NON_COPYABLE(DiagModel);

    protected:
        void _init(const std::shared_ptr<Context>&);

        DiagModel();

    public:
        FTK_API ~DiagModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<DiagModel> create(
            const std::shared_ptr<Context>&);

        //! Get the maximum number of samples.
        FTK_API size_t getSamplesMax() const;

        //! Observe the maximum number of samples.
        FTK_API std::shared_ptr<IObservable<size_t> > observeSamplesMax() const;

        //! Set the maximum number of samples.
        FTK_API void setSamplesMax(size_t);

        //! Get the samples.
        FTK_API const std::map<DiagData, std::vector<int64_t> >& getSamples() const;

        //! Observe the samples.
        FTK_API std::shared_ptr<IObservableMap<DiagData, std::vector<int64_t> > > observeSamples() const;

        //! Observe the samples increments.
        FTK_API std::shared_ptr<IObservableMap<DiagData, int64_t> > observeSamplesInc() const;

    private:
        FTK_PRIVATE();
    };
}
