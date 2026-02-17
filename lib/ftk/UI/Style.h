// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Color.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Style
    ///@{

    //! Size roles.
    enum class FTK_API_TYPE SizeRole
    {
        None,
        Margin,
        MarginSmall,
        MarginLarge,
        MarginInside,
        MarginDialog,
        Spacing,
        SpacingSmall,
        SpacingLarge,
        SpacingTool,
        Icon,
        Border,
        KeyFocus,
        LabelPad,
        ScrollArea,
        ScrollAreaSmall,
        Slider,
        Handle,
        Swatch,
        SwatchLarge,
        Shadow,
        DragLength,

        Count,
        First = None
    };
    FTK_ENUM(SizeRole);

    //! Get the default size roles.
    FTK_API std::map<SizeRole, int> getDefaultSizeRoles();

    //! Color roles.
    enum class FTK_API_TYPE ColorRole
    {
        None,

        Window,
        Base,
        Button,
        Text,
        TextDisabled,
        Border,
        Hover,
        Pressed,
        Checked,
        KeyFocus,
        Overlay,
        TooltipWindow,
        TooltipText,

        Red,
        Green,
        Blue,
        Cyan,
        Magenta,
        Yellow,

        Count,
        First = None
    };
    FTK_ENUM(ColorRole);

    //! Get default color roles.
    FTK_API std::map<ColorRole, Color4F> getDefaultColorRoles();

    //! Get light color roles.
    FTK_API std::map<ColorRole, Color4F> getLightColorRoles();

    //! Color controls.
    struct FTK_API_TYPE ColorControls
    {
        float brightness = 1.F;
        float contrast = 1.F;

        FTK_API bool operator == (const ColorControls&) const;
        FTK_API bool operator != (const ColorControls&) const;
    };

    //! Font roles.
    enum class FTK_API_TYPE FontRole
    {
        None,
        Label,
        Mono,
        Title,

        Count,
        First = None
    };
    FTK_ENUM(FontRole);

    //! Get default font roles.
    FTK_API std::map<FontRole, FontInfo> getDefaultFontRoles();

    //! Style.
    class FTK_API_TYPE Style : public std::enable_shared_from_this<Style>
    {
        FTK_NON_COPYABLE(Style);

    protected:
        void _init(const std::shared_ptr<Context>&);

        Style();

    public:
        FTK_API ~Style();

        //! Create a new style.
        FTK_API static std::shared_ptr<Style> create(
            const std::shared_ptr<Context>&);

        //! Get a size role.
        FTK_API int getSizeRole(SizeRole, float scale) const;

        //! Set a size role.
        FTK_API void setSizeRole(SizeRole, int);

        //! Set the size roles.
        FTK_API void setSizeRoles(const std::map<SizeRole, int>&);

        //! Get a color role.
        FTK_API Color4F getColorRole(ColorRole) const;

        //! Set a color role.
        FTK_API void setColorRole(ColorRole, const Color4F&);

        //! Set the color roles.
        FTK_API void setColorRoles(const std::map<ColorRole, Color4F>&);

        //! Get the color controls.
        FTK_API const ColorControls& getColorControls() const;

        //! Set the color controls.
        FTK_API void setColorControls(const ColorControls&);

        //! Get a font role.
        FTK_API FontInfo getFontRole(FontRole, float scale) const;

        //! Set a font role.
        FTK_API void setFontRole(FontRole, const FontInfo&);

        //! Set the font roles.
        FTK_API void setFontRoles(const std::map<FontRole, FontInfo>&);

        //! Observe style changes.
        FTK_API std::shared_ptr<IObservable<bool> > observeChanged() const;

    private:
        void _colorUpdate();

        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const ColorControls&);

    FTK_API void from_json(const nlohmann::json&, ColorControls&);

    ///@}
}
