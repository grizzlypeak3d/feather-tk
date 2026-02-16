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

#include <algorithm>
#include <array>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        DiagData,
        "GLBuffers",
        "GLBuffersMB"
        "GLMeshes"
        "GLMeshesMB",
        "GLShaders",
        "GLTextures"
        "GLTexturesMB",
        "Images"
        "ImagesMB",
        "Widgets");

    struct DiagModel::Private
    {
        std::shared_ptr<Observable<size_t> > samplesMax;
        std::shared_ptr<ObservableMap<DiagData, std::vector<int64_t> > > samples;
        std::shared_ptr<ObservableMap<DiagData, int64_t> > samplesInc;
        std::shared_ptr<Timer> timer;
    };

    void DiagModel::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.samplesMax = Observable<size_t>::create(100);
        p.samples = ObservableMap<DiagData, std::vector<int64_t> >::create();
        p.samplesInc = ObservableMap<DiagData, int64_t>::create();

        p.timer = Timer::create(context);
        p.timer->setRepeating(true);
        p.timer->start(
            std::chrono::seconds(3),
            [this]
            {
                FTK_P();
                std::map<DiagData, int64_t> samplesInc;
                samplesInc[DiagData::GLBuffers] = gl::OffscreenBuffer::getObjectCount();
                samplesInc[DiagData::GLBuffersMB] = gl::OffscreenBuffer::getTotalByteCount() / megabyte;
                samplesInc[DiagData::GLMeshes] = gl::VBO::getObjectCount();
                samplesInc[DiagData::GLMeshesMB] = gl::VBO::getTotalByteCount() / megabyte;
                samplesInc[DiagData::GLShaders] = gl::Shader::getObjectCount();
                samplesInc[DiagData::GLTextures] = gl::Texture::getObjectCount();
                samplesInc[DiagData::GLTexturesMB] = gl::Texture::getTotalByteCount() / megabyte;
                samplesInc[DiagData::Images] = Image::getObjectCount();
                samplesInc[DiagData::ImagesMB] = Image::getTotalByteCount() / megabyte;
                samplesInc[DiagData::Widgets] = IWidget::getObjectCount();
                p.samplesInc->setAlways(samplesInc);

                std::map<DiagData, std::vector<int64_t> > samples = p.samples->get();
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

    const std::map<DiagData, std::vector<int64_t> >& DiagModel::getSamples() const
    {
        return _p->samples->get();
    }

    std::shared_ptr<IObservableMap<DiagData, std::vector<int64_t> > > DiagModel::observeSamples() const
    {
        return _p->samples;
    }

    std::shared_ptr<IObservableMap<DiagData, int64_t> > DiagModel::observeSamplesInc() const
    {
        return _p->samplesInc;
    }
}
