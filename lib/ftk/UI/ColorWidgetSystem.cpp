// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorWidgetSystem.h>

namespace ftk
{
    struct ColorWidgetSystem::Private
    {
        ColorWidgetMode mode = ColorWidgetMode::RGB;
        std::vector<Color4F> palette =
        {
            // Saturated hues first, then neutrals: enough to tell items
            // apart at a glance without inviting a color debate.
            Color4F(.9F, .25F, .2F, 1.F),
            Color4F(1.F, .6F, .1F, 1.F),
            Color4F(.9F, .85F, .2F, 1.F),
            Color4F(.25F, .7F, .25F, 1.F),
            Color4F(.2F, .7F, .8F, 1.F),
            Color4F(.3F, .5F, .9F, 1.F),
            Color4F(.6F, .35F, .9F, 1.F),
            Color4F(.9F, .3F, .7F, 1.F),
            Color4F(1.F, 1.F, 1.F, 1.F),
            Color4F(.5F, .5F, .5F, 1.F)
        };
    };

    ColorWidgetSystem::ColorWidgetSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::ColorWidgetSystem"),
        _p(new Private)
    {}

    ColorWidgetSystem::~ColorWidgetSystem()
    {}

    std::shared_ptr<ColorWidgetSystem> ColorWidgetSystem::create(
        const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<ColorWidgetSystem>(new ColorWidgetSystem(context));
    }

    ColorWidgetMode ColorWidgetSystem::getMode() const
    {
        return _p->mode;
    }

    void ColorWidgetSystem::setMode(ColorWidgetMode value)
    {
        _p->mode = value;
    }

    const std::vector<Color4F>& ColorWidgetSystem::getPalette() const
    {
        return _p->palette;
    }

    void ColorWidgetSystem::setPalette(const std::vector<Color4F>& value)
    {
        _p->palette = value;
    }
}
