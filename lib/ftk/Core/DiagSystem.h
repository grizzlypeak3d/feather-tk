// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>
#include <ftk/Core/ObservableMap.h>

#include <string>

namespace ftk
{
    class Context;

    //! \name Debugging
    ///@{

    //! Diagnostics system.
    class FTK_API_TYPE DiagSystem : public ISystem
    {
        FTK_NON_COPYABLE(DiagSystem);

    protected:
        DiagSystem(const std::shared_ptr<Context>&);

    public:
        FTK_API virtual ~DiagSystem();

        //! Create a new system.
        FTK_API static std::shared_ptr<DiagSystem> create(const std::shared_ptr<Context>&);

        //! Add a sampler function.
        FTK_API void addSampler(const std::string&, const std::function<int64_t(void)>&);

        //! Get the sampler groups.
        FTK_API const std::vector<std::string>& getGroups() const;

        //! Get the sampler names in a group.
        FTK_API std::vector<std::string> getNames(const std::string& group) const;

        //! Get whether the sampler exists.
        FTK_API bool hasSampler(const std::string&) const;

        //! Get the maximum number of samples.
        FTK_API size_t getSamplesMax() const;

        //! Observe the maximum number of samples.
        FTK_API std::shared_ptr<IObservable<size_t> > observeSamplesMax() const;

        //! Set the maximum number of samples.
        FTK_API void setSamplesMax(size_t);

        //! Get the samples.
        FTK_API const std::map<std::string, std::vector<int64_t> >& getSamples() const;

        //! Observe the samples.
        FTK_API std::shared_ptr<IObservableMap<std::string, std::vector<int64_t> > > observeSamples() const;

        //! Observe the samples increments.
        FTK_API std::shared_ptr<IObservableMap<std::string, int64_t> > observeSamplesInc() const;

        FTK_API void tick() override;
        FTK_API std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };
}
