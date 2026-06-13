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
        "Drag Length",
        "Corner Radius");

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
        out[SizeRole::CornerRadius] = 4;
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
        "Yellow",

        "Well");

    std::map<ColorRole, Color4F> getDefaultColorRoles()
    {
        std::map<ColorRole, Color4F> out;
        out[ColorRole::None] = Color4F();

        out[ColorRole::Window] = Color4F(.15F, .15F, .15F);
        out[ColorRole::Base] = Color4F(.12F, .12F, .12F);
        out[ColorRole::Well] = Color4F(.09F, .09F, .09F);
        out[ColorRole::Button] = Color4F(.2F, .2F, .2F);
        out[ColorRole::Text] = Color4F(.85F, .85F, .85F);
        out[ColorRole::TextDisabled] = Color4F(.5F, .5F, .5F);
        out[ColorRole::Border] = Color4F(.3F, .3F, .3F);
        out[ColorRole::Hover] = Color4F(1.F, 1.F, 1.F, .1F);
        out[ColorRole::Pressed] = Color4F(1.F, 1.F, 1.F, .2F);
        out[ColorRole::Checked] = Color4F(.56F, .36F, .15F);
        out[ColorRole::KeyFocus] = Color4F(.8F, .6F, .3F);
        out[ColorRole::Overlay] = Color4F(0.F, 0.F, 0.F, .5F);
        out[ColorRole::TooltipWindow] = Color4F(.2F, .2F, .2F);
        out[ColorRole::TooltipText] = Color4F(.9F, .9F, .9F);

        out[ColorRole::Red]     = Color4F(.835F, .369F, 0.F);    // Vermillion
        out[ColorRole::Green]   = Color4F(0.F,   .620F, .451F);  // Bluish green
        out[ColorRole::Blue]    = Color4F(0.F,   .447F, .698F);  // Blue
        out[ColorRole::Cyan]    = Color4F(.337F, .706F, .914F);  // Sky blue
        out[ColorRole::Magenta] = Color4F(.8F,   .475F, .655F);  // Reddish purple
        out[ColorRole::Yellow]  = Color4F(.941F, .894F, .259F);  // Yellow

        return out;
    }

    ColorControls getDefaultColorControls()
    {
        ColorControls out;
        out.disabledAlpha = .5F;
        return out;
    }

    ColorControls getLightColorControls()
    {
        ColorControls out;
        // Light styles need more dimming; black-on-white has further to fade.
        out.disabledAlpha = .6F;
        return out;
    }

    std::map<ColorRole, Color4F> getLightColorRoles()
    {
        std::map<ColorRole, Color4F> out = getDefaultColorRoles();

        out[ColorRole::Window] = Color4F(.95F, .95F, .95F);
        out[ColorRole::Base] = Color4F(1.F, 1.F, 1.F);
        out[ColorRole::Well] = Color4F(1.F, 1.F, 1.F);
        out[ColorRole::Button] = Color4F(.85F, .85F, .85F);
        out[ColorRole::Text] = Color4F(0.F, 0.F, 0.F);
        out[ColorRole::TextDisabled] = Color4F(.4F, .4F, .4F);
        out[ColorRole::Border] = Color4F(.72F, .72F, .72F);
        out[ColorRole::Hover] = Color4F(0.F, 0.F, 0.F, .1F);
        out[ColorRole::Pressed] = Color4F(0.F, 0.F, 0.F, .2F);
        out[ColorRole::Checked]  = Color4F(.62F, .42F, .12F);
        out[ColorRole::KeyFocus] = Color4F(.74F, .50F, .13F);
        out[ColorRole::Overlay] = Color4F(1.F, 1.F, 1.F, .5F);
        out[ColorRole::TooltipWindow] = Color4F(1.F, 1.F, 1.F);
        out[ColorRole::TooltipText] = Color4F(0.F, 0.F, 0.F);

        return out;
    }

    bool ColorControls::operator == (const ColorControls& other) const
    {
        return
            brightness == other.brightness &&
            contrast == other.contrast &&
            disabledAlpha == other.disabledAlpha;
    }

    bool ColorControls::operator != (const ColorControls& other) const
    {
        return !(*this == other);
    }

    struct Style::Private
    {
        std::weak_ptr<Context> context;
        std::shared_ptr<ObservableMap<SizeRole, int> > sizeRoles;
        std::shared_ptr<ObservableMap<ColorRole, Color4F> > colorRoles;
        std::shared_ptr<Observable<ColorControls> > colorControls;
        M44F colorMatrix;
        std::shared_ptr<ObservableMap<FontType, std::string> > fonts;
    };

    void Style::_init(
        const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.context = context;
        p.sizeRoles = ObservableMap<SizeRole, int>::create(getDefaultSizeRoles());
        p.colorRoles = ObservableMap<ColorRole, Color4F>::create(getDefaultColorRoles());
        p.colorControls = Observable<ColorControls>::create();
        std::map<FontType, std::string> fonts;
        for (const auto font : getFontTypeEnums())
        {
            fonts[font] = getDefaultFont(font);
        }
        p.fonts = ObservableMap<FontType, std::string>::create(fonts);

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

    const std::map<SizeRole, int>& Style::getSizeRoles() const
    {
        return _p->sizeRoles->get();
    }

    FTK_API std::shared_ptr<IObservableMap<SizeRole, int> > Style::observeSizeRoles() const
    {
        return _p->sizeRoles;
    }

    void Style::setSizeRoles(const std::map<SizeRole, int>& value)
    {
        _p->sizeRoles->setIfChanged(value);
    }

    int Style::getSizeRole(SizeRole role, float scale) const
    {
        FTK_P();
        const auto& sizeRoles = p.sizeRoles->get();
        const auto i = sizeRoles.find(role);
        return i != sizeRoles.end() ? (i->second * scale) : 0;
    }

    const std::map<ColorRole, Color4F>& Style::getColorRoles() const
    {
        return _p->colorRoles->get();
    }

    std::shared_ptr<IObservableMap<ColorRole, Color4F> > Style::observeColorRoles() const
    {
        return _p->colorRoles;
    }

    void Style::setColorRoles(const std::map<ColorRole, Color4F>& value)
    {
        _p->colorRoles->setIfChanged(value);
    }

    Color4F Style::getColorRole(ColorRole role) const
    {
        FTK_P();
        const auto& colorRoles = p.colorRoles->get();
        const auto i = colorRoles.find(role);
        Color4F c;
        if (i != colorRoles.end())
        {
            c = i->second;
        }
        const V3F v = p.colorMatrix * V3F(c.r, c.g, c.b);
        return Color4F(v.x, v.y, v.z, c.a);
    }

    Color4F Style::getColorRole(ColorRole role, bool enabled) const
    {
        FTK_P();
        Color4F out = getColorRole(role);
        if (!enabled)
        {
            out.a *= p.colorControls->get().disabledAlpha;
        }
        return out;
    }

    const ColorControls& Style::getColorControls() const
    {
        return _p->colorControls->get();
    }

    std::shared_ptr<IObservable<ColorControls> > Style::observeColorControls() const
    {
        return _p->colorControls;
    }

    void Style::setColorControls(const ColorControls& value)
    {
        FTK_P();
        if (p.colorControls->setIfChanged(value))
        {
            _colorUpdate();
        }
    }

    const std::map<FontType, std::string>& Style::getFonts() const
    {
        return _p->fonts->get();
    }

    std::shared_ptr<IObservableMap<FontType, std::string> > Style::observeFonts() const
    {
        return _p->fonts;
    }

    void Style::setFonts(const std::map<FontType, std::string>& value)
    {
        _p->fonts->setIfChanged(value);
    }

    FontInfo Style::getFont(FontType font, float scale) const
    {
        FTK_P();
        FontInfo out;
        const auto& fonts = p.fonts->get();
        const auto i = fonts.find(font);
        if (i != fonts.end())
        {
            out.name = i->second;
            out.size *= scale;
        }
        return out;
    }

    FontInfo Style::getFont(FontType font, int size, float scale) const
    {
        FTK_P();
        FontInfo out;
        const auto& fonts = p.fonts->get();
        const auto i = fonts.find(font);
        if (i != fonts.end())
        {
            out.name = i->second;
            out.size = size * scale;
        }
        return out;
    }

    void Style::_colorUpdate()
    {
        FTK_P();
        const auto& colorControls = p.colorControls->get();
        p.colorMatrix =
            brightness(V3F(colorControls.brightness, colorControls.brightness, colorControls.brightness)) *
            contrast(V3F(colorControls.contrast, colorControls.contrast, colorControls.contrast));
    }

    void to_json(nlohmann::json& json, const ColorControls& value)
    {
        json["Brightness"] = value.brightness;
        json["Contrast"] = value.contrast;
        json["DisabledAlpha"] = value.disabledAlpha;
    }

    void from_json(const nlohmann::json& json, ColorControls& out)
    {
        json.at("Brightness").get_to(out.brightness);
        json.at("Contrast").get_to(out.contrast);
        json.at("DisabledAlpha").get_to(out.disabledAlpha);
    }
}
