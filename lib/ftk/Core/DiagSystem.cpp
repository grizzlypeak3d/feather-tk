// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/DiagSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/String.h>

namespace ftk
{
    struct DiagSystem::Private
    {
        std::vector<std::pair<std::string, std::function<int64_t(void)> > > samplers;
        std::vector<std::string> groups;
        std::map<std::string, std::vector<std::string> > names;
        std::shared_ptr<Observable<size_t> > samplesMax;
        std::shared_ptr<ObservableMap<std::string, std::vector<int64_t> > > samples;
        std::shared_ptr<ObservableMap<std::string, int64_t> > samplesInc;
    };

    DiagSystem::DiagSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::DiagSystem"),
        _p(new Private)
    {
        FTK_P();
        p.samplesMax = Observable<size_t>::create(100);
        p.samples = ObservableMap<std::string, std::vector<int64_t> >::create();
        p.samplesInc = ObservableMap<std::string, int64_t>::create();

        addSampler(
            "feather-tk Memory/Images: {0}MB",
            [] { return Image::getTotalByteCount() / megabyte; });

        addSampler(
            "feather-tk Objects/Images: {0}",
            [] { return Image::getObjectCount(); });
    }

    DiagSystem::~DiagSystem()
    {}

    std::shared_ptr<DiagSystem> DiagSystem::create(
        const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<DiagSystem>(new DiagSystem(context));
    }

    void DiagSystem::addSampler(const std::string& id, const std::function<int64_t(void)>& sampler)
    {
        FTK_P();
        if (!hasSampler(id))
        {
            const auto s = ftk::split(id, '/');
            if (2 == s.size())
            {
                p.samplers.push_back(std::make_pair(id, sampler));
                auto i = std::find(p.groups.begin(), p.groups.end(), s[0]);
                if (i == p.groups.end())
                {
                    p.groups.push_back(s[0]);
                }
                p.names[s[0]].push_back(s[1]);
            }
        }
    }

    const std::vector<std::string>& DiagSystem::getGroups() const
    {
        return _p->groups;
    }

    std::vector<std::string> DiagSystem::getNames(const std::string& group) const
    {
        FTK_P();
        const auto i = p.names.find(group);
        return i != p.names.end() ? i->second : std::vector<std::string>();
    }

    bool DiagSystem::hasSampler(const std::string& id) const
    {
        FTK_P();
        const auto i = std::find_if(
            p.samplers.begin(),
            p.samplers.end(),
            [id](const std::pair<std::string, std::function<int64_t(void)> >& value)
            {
                return id == value.first;
            });
        return i != p.samplers.end();
    }

    size_t DiagSystem::getSamplesMax() const
    {
        return _p->samplesMax->get();
    }

    std::shared_ptr<IObservable<size_t> > DiagSystem::observeSamplesMax() const
    {
        return _p->samplesMax;
    }

    void DiagSystem::setSamplesMax(size_t value)
    {
        FTK_P();
        if (p.samplesMax->setIfChanged(value))
        {
            auto samples = p.samples->get();
            for (auto i : samples)
            {
                if (i.second.size() > value)
                {
                    std::list<int64_t> tmp(i.second.begin(), i.second.end());
                    while (tmp.size() > value)
                    {
                        tmp.pop_front();
                    }
                    i.second = std::vector<int64_t>(tmp.begin(), tmp.end());
                }
            }
            p.samples->setIfChanged(samples);
        }
    }

    const std::map<std::string, std::vector<int64_t> >& DiagSystem::getSamples() const
    {
        return _p->samples->get();
    }

    std::shared_ptr<IObservableMap<std::string, std::vector<int64_t> > > DiagSystem::observeSamples() const
    {
        return _p->samples;
    }

    std::shared_ptr<IObservableMap<std::string, int64_t> > DiagSystem::observeSamplesInc() const
    {
        return _p->samplesInc;
    }

    void DiagSystem::tick()
    {
        FTK_P();
        std::map<std::string, int64_t> samplesInc;
        for (const auto& i : p.samplers)
        {
            samplesInc[i.first] = 0;
        }
        for (const auto& i : p.samplers)
        {
            samplesInc[i.first] += i.second();
        }
        p.samplesInc->setAlways(samplesInc);

        std::map<std::string, std::vector<int64_t> > samples = p.samples->get();
        for (auto i : samplesInc)
        {
            samples[i.first].push_back(i.second);
        }
        const size_t max = p.samplesMax->get();
        for (auto i : samples)
        {
            if (i.second.size() > max)
            {
                std::list<int64_t> tmp(i.second.begin(), i.second.end());
                while (tmp.size() > max)
                {
                    tmp.pop_front();
                }
                i.second = std::vector<int64_t>(tmp.begin(), tmp.end());
            }
        }
        p.samples->setAlways(samples);
    }

    std::chrono::milliseconds DiagSystem::getTickTime() const
    {
        return std::chrono::seconds(3);
    }
}
