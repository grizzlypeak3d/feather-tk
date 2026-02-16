// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DiagModel.h>

#include <ftk/UI/IWidget.h>

#include <ftk/GL/Mesh.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#include <ftk/GL/Texture.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>
#include <ftk/Core/Timer.h>

#include <nlohmann/json.hpp>

namespace ftk
{
    struct DiagModel::Private
    {
        std::vector<std::pair<std::string, std::function<int64_t(void)> > > samplers;
        std::vector<std::string> groups;
        std::map<std::string, std::vector<std::string> > names;
        std::shared_ptr<Observable<size_t> > samplesMax;
        std::shared_ptr<ObservableMap<std::string, std::vector<int64_t> > > samples;
        std::shared_ptr<ObservableMap<std::string, int64_t> > samplesInc;
        std::shared_ptr<Timer> timer;
    };

    void DiagModel::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.samplesMax = Observable<size_t>::create(100);
        p.samples = ObservableMap<std::string, std::vector<int64_t> >::create();
        p.samplesInc = ObservableMap<std::string, int64_t>::create();

        addSampler(
            "feather-tk Memory/Images (MB)",
            [] { return Image::getTotalByteCount() / megabyte; });

        addSampler(
            "feather-tk Objects/Images",
            [] { return Image::getObjectCount(); });
        addSampler(
            "feather-tk Objects/IWidgets",
            [] { return IWidget::getObjectCount(); });

        addSampler(
            "feather-tk GL Memory/Buffers (MB)",
            [] { return gl::OffscreenBuffer::getTotalByteCount() / megabyte; });
        addSampler(
            "feather-tk GL Memory/Meshes (MB)",
            [] { return gl::VBO::getTotalByteCount() / megabyte; });
        addSampler(
            "feather-tk GL Memory/Textures (MB)",
            [] { return gl::Texture::getTotalByteCount() / megabyte; });

        addSampler(
            "feather-tk GL Objects/Buffers",
            [] { return gl::OffscreenBuffer::getObjectCount(); });
        addSampler(
            "feather-tk GL Objects/Meshes",
            [] { return gl::VBO::getObjectCount(); });
        addSampler(
            "feather-tk GL Objects/Shaders",
            [] { return gl::Shader::getObjectCount(); });
        addSampler(
            "feather-tk GL Objects/Textures",
            [] { return gl::Texture::getObjectCount(); });

        p.timer = Timer::create(context);
        p.timer->setRepeating(true);
        p.timer->start(
            std::chrono::seconds(3),
            [this]
            {
                FTK_P();
                std::map<std::string, int64_t> samplesInc;
                for (const auto& i : p.samplers)
                {
                    samplesInc[i.first] = i.second();
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
            });
    }

    DiagModel::DiagModel() :
        _p(new Private)
    {}

    DiagModel::~DiagModel()
    {}

    std::shared_ptr<DiagModel> DiagModel::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<DiagModel>(new DiagModel);
        out->_init(context);
        return out;
    }

    void DiagModel::addSampler(const std::string& id, const std::function<int64_t(void)>& sampler)
    {
        FTK_P();
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

    const std::vector<std::string>& DiagModel::getGroups() const
    {
        return _p->groups;
    }

    std::vector<std::string> DiagModel::getNames(const std::string& group) const
    {
        FTK_P();
        const auto i = p.names.find(group);
        return i != p.names.end() ? i->second : std::vector<std::string>();
    }

    size_t DiagModel::getSamplesMax() const
    {
        return _p->samplesMax->get();
    }

    std::shared_ptr<IObservable<size_t> > DiagModel::observeSamplesMax() const
    {
        return _p->samplesMax;
    }

    void DiagModel::setSamplesMax(size_t value)
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

    const std::map<std::string, std::vector<int64_t> >& DiagModel::getSamples() const
    {
        return _p->samples->get();
    }

    std::shared_ptr<IObservableMap<std::string, std::vector<int64_t> > > DiagModel::observeSamples() const
    {
        return _p->samples;
    }

    std::shared_ptr<IObservableMap<std::string, int64_t> > DiagModel::observeSamplesInc() const
    {
        return _p->samplesInc;
    }
}
