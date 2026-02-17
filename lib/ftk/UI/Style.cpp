// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Style.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        SizeRole,
        "None",
        "Margin",
        "Margin Small",
        "Margin Large",
        "Margin Inside",
        "Margin Dialog",
        "Spacing",
        "Spacing Small",
        "Spacing Large",
        "Spacing Tool",
        "Icon",
        "Border",
        "Key Focus",
        "Label Pad",
        "Scroll Area",
        "Scroll Area Small",
        "Slider",
        "Handle",
        "Swatch",
        "Swatch Large",
        "Shadow",
        "Drag Length");

    std::map<SizeRole, int> getDefaultSizeRoles()
    {
        std::map<SizeRole, int> out;
        out[SizeRole::Margin] = 10;
        out[SizeRole::MarginSmall] = 5;
        out[SizeRole::MarginLarge] = 20;
        out[SizeRole::MarginInside] = 2;
        out[SizeRole::MarginDialog] = 40;
        out[SizeRole::Spacing] = 10;
        out[SizeRole::SpacingSmall] = 5;
        out[SizeRole::SpacingLarge] = 20;
        out[SizeRole::SpacingTool] = 2;
        out[SizeRole::Icon] = 20;
        out[SizeRole::Border] = 1;
        out[SizeRole::KeyFocus] = 2;
        out[SizeRole::LabelPad] = 4;
        out[SizeRole::ScrollArea] = 200;
        out[SizeRole::ScrollAreaSmall] = 100;
        out[SizeRole::Slider] = 100;
        out[SizeRole::Handle] = 8;
        out[SizeRole::Swatch] = 20;
        out[SizeRole::SwatchLarge] = 40;
        out[SizeRole::Shadow] = 15;
        out[SizeRole::DragLength] = 10;
        return out;
    }

    FTK_ENUM_IMPL(
        ColorRole,
        "None",

        "Window",
        "Base",
        "Button",
        "Text",
        "Text Disabled",
        "Border",
        "Hover",
        "Pressed",
        "Checked",
        "Key Focus",
        "Overlay",
        "Tooltip Window",
        "Tooltip Text",

        "Red",
        "Green",
        "Blue",
        "Cyan",
        "Magenta",
        "Yellow");

    std::map<ColorRole, Color4F> getDefaultColorRoles()
    {
        std::map<ColorRole, Color4F> out;
        out[ColorRole::None] = Color4F();

        out[ColorRole::Window] = Color4F(.15F, .15F, .15F);
        out[ColorRole::Base] = Color4F(.12F, .12F, .12F);
        out[ColorRole::Button] = Color4F(.2F, .2F, .2F);
        out[ColorRole::Text] = Color4F(1.F, 1.F, 1.F);
        out[ColorRole::TextDisabled] = Color4F(.5F, .5F, .5F);
        out[ColorRole::Border] = Color4F(0.F, 0.F, 0.F);
        out[ColorRole::Hover] = Color4F(1.F, 1.F, 1.F, .1F);
        out[ColorRole::Pressed] = Color4F(1.F, 1.F, 1.F, .2F);
        out[ColorRole::Checked] = Color4F(.56F, .36F, .15F);
        out[ColorRole::KeyFocus] = Color4F(.8F, .6F, .3F);
        out[ColorRole::Overlay] = Color4F(0.F, 0.F, 0.F, .5F);
        out[ColorRole::TooltipWindow] = Color4F(1.F, .95F, .7F);
        out[ColorRole::TooltipText] = Color4F(0.F, 0.F, 0.F);

        out[ColorRole::Red] = Color4F(.6F, .3F, .3F);
        out[ColorRole::Green] = Color4F(.3F, .6F, .3F);
        out[ColorRole::Blue] = Color4F(.3F, .3F, .6F);
        out[ColorRole::Cyan] = Color4F(.3F, .6F, .6F);
        out[ColorRole::Magenta] = Color4F(.6F, .3F, .6F);
        out[ColorRole::Yellow] = Color4F(.6F, .6F, .3F);

        return out;
    }

    std::map<ColorRole, Color4F> getLightColorRoles()
    {
        std::map<ColorRole, Color4F> out = getDefaultColorRoles();

        out[ColorRole::Window] = Color4F(.85F, .85F, .85F);
        out[ColorRole::Base] = Color4F(.95F, .95F, .95F);
        out[ColorRole::Button] = Color4F(.75F, .75F, .75F);
        out[ColorRole::Text] = Color4F(0.F, 0.F, 0.F);
        out[ColorRole::TextDisabled] = Color4F(.4F, .4F, .4F);
        out[ColorRole::Border] = Color4F(.65F, .65F, .65F);
        out[ColorRole::Hover] = Color4F(0.F, 0.F, 0.F, .1F);
        out[ColorRole::Pressed] = Color4F(0.F, 0.F, 0.F, .2F);
        out[ColorRole::Checked] = Color4F(.6F, .7F, .85F);
        out[ColorRole::KeyFocus] = Color4F(0.F, .5F, 1.F);
        out[ColorRole::Overlay] = Color4F(1.F, 1.F, 1.F, .5F);

        return out;
    }

    bool ColorControls::operator == (const ColorControls& other) const
    {
        return
            brightness == other.brightness &&
            contrast == other.contrast;
    }

    bool ColorControls::operator != (const ColorControls& other) const
    {
        return !(*this == other);
    }

    FTK_ENUM_IMPL(
        FontRole,
        "None",
        "Label",
        "Mono",
        "Title");

    std::map<FontRole, FontInfo> getDefaultFontRoles()
    {
        std::map<FontRole, FontInfo> out;
        out[FontRole::None]  = FontInfo(std::string(), 0);
        out[FontRole::Label] = FontInfo(getFont(Font::Regular), 12);
        out[FontRole::Mono]  = FontInfo(getFont(Font::Mono),    12);
        out[FontRole::Title] = FontInfo(getFont(Font::Bold),    12);
        return out;
    }

    struct Style::Private
    {
        std::weak_ptr<Context> context;
        std::map<SizeRole, int> sizeRoles;
        std::map<ColorRole, Color4F> colorRoles;
        ColorControls colorControls;
        M44F colorMatrix;
        std::map<FontRole, FontInfo> fontRoles;
        std::shared_ptr<Observable<bool> > changed;
    };

    void Style::_init(
        const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.context = context;
        p.sizeRoles = getDefaultSizeRoles();
        p.colorRoles = getDefaultColorRoles();
        p.fontRoles = getDefaultFontRoles();
        p.changed = Observable<bool>::create();

        _colorUpdate();
    }

    Style::Style() :
        _p(new Private)
    {}

    Style::~Style()
    {}

    std::shared_ptr<Style> Style::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Style>(new Style);
        out->_init(context);
        return out;
    }

    int Style::getSizeRole(SizeRole role, float scale) const
    {
        FTK_P();
        const auto i = p.sizeRoles.find(role);
        return i != p.sizeRoles.end() ? (i->second * scale) : 0;
    }

    void Style::setSizeRole(SizeRole role, int value)
    {
        FTK_P();
        if (value == p.sizeRoles[role])
            return;
        p.sizeRoles[role] = value;
        p.changed->setAlways(true);
    }

    void Style::setSizeRoles(const std::map<SizeRole, int>& value)
    {
        FTK_P();
        if (value == p.sizeRoles)
            return;
        p.sizeRoles = value;
        p.changed->setAlways(true);
    }

    Color4F Style::getColorRole(ColorRole role) const
    {
        FTK_P();
        const auto i = p.colorRoles.find(role);
        Color4F c;
        if (i != p.colorRoles.end())
        {
            c = i->second;
        }
        const V3F v = p.colorMatrix * V3F(c.r, c.g, c.b);
        return Color4F(v.x, v.y, v.z, c.a);
    }

    void Style::setColorRole(ColorRole role, const Color4F& value)
    {
        FTK_P();
        if (value == p.colorRoles[role])
            return;
        p.colorRoles[role] = value;
        p.changed->setAlways(true);
    }

    void Style::setColorRoles(const std::map<ColorRole, Color4F>& value)
    {
        FTK_P();
        if (value == p.colorRoles)
            return;
        p.colorRoles = value;
        p.changed->setAlways(true);
    }

    const ColorControls& Style::getColorControls() const
    {
        return _p->colorControls;
    }

    void Style::setColorControls(const ColorControls& value)
    {
        FTK_P();
        if (value == p.colorControls)
            return;
        p.colorControls = value;
        _colorUpdate();
        p.changed->setAlways(true);
    }

    FontInfo Style::getFontRole(FontRole role, float scale) const
    {
        FTK_P();
        FontInfo out;
        const auto i = p.fontRoles.find(role);
        if (i != p.fontRoles.end())
        {
            out = i->second;
            out.size *= scale;
        }
        return out;
    }

    void Style::setFontRole(FontRole role, const FontInfo& value)
    {
        FTK_P();
        if (value == p.fontRoles[role])
            return;
        p.fontRoles[role] = value;
        p.changed->setAlways(true);
    }

    void Style::setFontRoles(const std::map<FontRole, FontInfo>& value)
    {
        FTK_P();
        if (value == p.fontRoles)
            return;
        p.fontRoles = value;
        p.changed->setAlways(true);
    }

    std::shared_ptr<IObservable<bool> > Style::observeChanged() const
    {
        return _p->changed;
    }

    void Style::_colorUpdate()
    {
        FTK_P();
        p.colorMatrix =
            brightness(V3F(p.colorControls.brightness, p.colorControls.brightness, p.colorControls.brightness)) *
            contrast(V3F(p.colorControls.contrast, p.colorControls.contrast, p.colorControls.contrast));
    }

    void to_json(nlohmann::json& json, const ColorControls& value)
    {
        json["Brightness"] = value.brightness;
        json["Contrast"] = value.contrast;
    }

    void from_json(const nlohmann::json& json, ColorControls& out)
    {
        json.at("Brightness").get_to(out.brightness);
        json.at("Contrast").get_to(out.contrast);
    }
}