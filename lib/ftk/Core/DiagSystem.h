// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/ISystem.h>
#include <ftk/Core/ObservableMap.h>

#include <string>

namespace ftk
{
    class Context;

    //! \name Debugging
    ///@{

    //! How a sample is turned into the number its format string expects.
    //!
    //! Samples are integers, which is right for counting things and wrong for
    //! timing them: the resolution wanted for a graph and the unit wanted for
    //! reading are not the same. Sample in microseconds, divide by a thousand
    //! and show one decimal, and the graph keeps its detail while the label
    //! says milliseconds.
    struct FTK_CORE_API_TYPE DiagFormat
    {
        double divisor = 1.0;

        //! Decimal places. Below zero formats the sample as the integer it is.
        int precision = -1;
    };

    //! Apply a sampler's format to a sample.
    FTK_CORE_API std::string diagText(
        const std::string& format,
        const DiagFormat&,
        int64_t value);

    //! Diagnostics system.
    class FTK_CORE_API_TYPE DiagSystem : public ISystem
    {
        FTK_NON_COPYABLE(DiagSystem);

    protected:
        DiagSystem(const std::shared_ptr<Context>&);

    public:
        FTK_CORE_API virtual ~DiagSystem();

        //! Create a new system.
        FTK_CORE_API static std::shared_ptr<DiagSystem> create(const std::shared_ptr<Context>&);

        //! Add a sampler function. The id is the group and then the name,
        //! "Group/Name: {0}", the name being the format string for the
        //! readout. An id already added is left as it is.
        FTK_CORE_API void addSampler(
            const std::string&,
            const std::function<int64_t(void)>&,
            const DiagFormat& = DiagFormat());

        //! Get how a sampler's value should be formatted.
        FTK_CORE_API DiagFormat getFormat(const std::string& id) const;

        //! Get the sampler groups, in the order they are shown.
        FTK_CORE_API const std::vector<std::string>& getGroups() const;

        //! Get the group order.
        FTK_CORE_API const std::vector<std::string>& getGroupOrder() const;

        //! Set the group order: the groups named come first, in that order,
        //! and the rest follow in the order they were added. The default
        //! puts what the application is doing -- its frames and memory --
        //! ahead of the object counts a leak is hunted with; an application
        //! puts its own groups in front of it.
        FTK_CORE_API void setGroupOrder(const std::vector<std::string>&);

        //! Get the sampler names in a group.
        FTK_CORE_API std::vector<std::string> getNames(const std::string& group) const;

        //! Get whether the sampler exists.
        FTK_CORE_API bool hasSampler(const std::string&) const;

        //! Get the maximum number of samples.
        FTK_CORE_API size_t getSamplesMax() const;

        //! Observe the maximum number of samples.
        FTK_CORE_API std::shared_ptr<IObservable<size_t> > observeSamplesMax() const;

        //! Set the maximum number of samples.
        FTK_CORE_API void setSamplesMax(size_t);

        //! Get the samples.
        FTK_CORE_API const std::map<std::string, std::vector<int64_t> >& getSamples() const;

        //! Observe the samples.
        FTK_CORE_API std::shared_ptr<IObservableMap<std::string, std::vector<int64_t> > > observeSamples() const;

        //! Observe the samples increments.
        FTK_CORE_API std::shared_ptr<IObservableMap<std::string, int64_t> > observeSamplesInc() const;

        FTK_CORE_API void tick() override;
        //! Set how often the samplers are read.
        //!
        //! The default is a few seconds, which makes the graphs a trend over
        //! minutes. An application wanting to see a hitch needs something
        //! shorter, and one sampling something expensive may want longer --
        //! sample often enough and the diagnostics become part of what they are
        //! measuring.
        FTK_CORE_API void setTickTime(const std::chrono::milliseconds&);

        FTK_CORE_API std::chrono::milliseconds getTickTime() const override;

    private:
        void _groupsUpdate();
        void _log();

        FTK_PRIVATE();
    };
}
