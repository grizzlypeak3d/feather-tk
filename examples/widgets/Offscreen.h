// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#if defined(FTK_GPU)
#include <ftk/GPU/OffscreenBuffer.h>
#include <ftk/GPU/Render.h>
#endif // FTK_GPU

#include <ftk/Core/Timer.h>

namespace widgets
{
    class App;

    class Offscreen : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        Offscreen() = default;

    public:
        virtual ~Offscreen();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        void drawEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;

    private:
        float _rotation = 0.F;
        std::shared_ptr<ftk::Timer> _timer;
        bool _doRender = true;
        std::shared_ptr<ftk::gl::Shader> _shader;
        std::shared_ptr<ftk::gl::OffscreenBuffer> _buffer;
#if defined(FTK_GPU)
        void _drawGPU(const ftk::DrawEvent&);

        std::shared_ptr<ftk::gpu::OffscreenBuffer> _gpuBuffer;
        std::shared_ptr<ftk::gpu::Render> _gpuRender;
#endif // FTK_GPU
    };
}
