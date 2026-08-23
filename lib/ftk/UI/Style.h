// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/Core/Color.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/ObservableMap.h>

namespace ftk
{
    //! \name Style
    ///@{

    //! Size roles.
    enum class FTK_UI_API_TYPE SizeRole
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
        Thumbnail,
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
        CornerRadius,

        Count,
        First = None
    };
    FTK_ENUM(FTK_UI_API, SizeRole);

    //! Get the default size roles.
    FTK_UI_API std::map<SizeRole, int> getDefaultSizeRoles();

    //! Color roles.
    enum class FTK_UI_API_TYPE ColorRole
    {
        None,

        Window,
        Base,
        Well,
        Header,
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
    FTK_ENUM(FTK_UI_API, ColorRole);

    //! Get default color roles.
    FTK_UI_API std::map<ColorRole, Color4F> getDefaultColorRoles();

    //! Get light color roles.
    FTK_UI_API std::map<ColorRole, Color4F> getLightColorRoles();

    //! Color controls.
    struct FTK_UI_API_TYPE ColorControls
    {
        float brightness = 1.F;
        float contrast = 1.F;

        //! Alpha multiplier applied by getColorRole(ColorRole, bool enabled)
        //! when a widget is disabled. Since widgets are alpha-composited,
        //! this dims toward the underlying surface.
        float disabledAlpha = .5F;

        FTK_UI_API bool operator == (const ColorControls&) const;
        FTK_UI_API bool operator != (const ColorControls&) const;
    };

    //! Get the default color controls.
    FTK_UI_API ColorControls getDefaultColorControls();

    //! Get color controls for the light style.
    FTK_UI_API ColorControls getLightColorControls();


    //! Style.
    class FTK_UI_API_TYPE Style : public std::enable_shared_from_this<Style>
    {
        FTK_NON_COPYABLE(Style);

    protected:
        void _init(const std::shared_ptr<Context>&);

        Style();

    public:
        FTK_UI_API ~Style();

        //! Create a new style.
        FTK_UI_API static std::shared_ptr<Style> create(
            const std::shared_ptr<Context>&);

        //! \name Size Roles
        ///@{

        FTK_UI_API const std::map<SizeRole, int>& getSizeRoles() const;
        FTK_UI_API std::shared_ptr<IObservableMap<SizeRole, int> > observeSizeRoles() const;
        FTK_UI_API void setSizeRoles(const std::map<SizeRole, int>&);

        FTK_UI_API int getSizeRole(SizeRole, float scale) const;

        ///@}

        //! \name Color Roles
        ///@{

        FTK_UI_API const std::map<ColorRole, Color4F>& getColorRoles() const;
        FTK_UI_API std::shared_ptr<IObservableMap<ColorRole, Color4F> > observeColorRoles() const;
        FTK_UI_API void setColorRoles(const std::map<ColorRole, Color4F>&);

        FTK_UI_API Color4F getColorRole(ColorRole) const;

        //! Get a color role for a widget state. When enabled is false the
        //! color's alpha is multiplied by ColorControls::disabledAlpha,
        //! dimming it toward the surface it is drawn over.
        FTK_UI_API Color4F getColorRole(ColorRole, bool enabled) const;

        ///@}

        //! \name Color Controls
        ///@{

        FTK_UI_API const ColorControls& getColorControls() const;
        FTK_UI_API std::shared_ptr<IObservable<ColorControls> > observeColorControls() const;
        FTK_UI_API void setColorControls(const ColorControls&);

        ///@}

        //! \name Fonts
        ///@{

        FTK_UI_API const std::map<FontType, std::string>& getFonts() const;
        FTK_UI_API std::shared_ptr<IObservableMap<FontType, std::string> > observeFonts() const;
        FTK_UI_API void setFonts(const std::map<FontType, std::string>&);

        FTK_UI_API FontInfo getFont(FontType, float scale) const;
        FTK_UI_API FontInfo getFont(FontType, int size, float scale) const;

        ///@}

    private:
        void _colorUpdate();

        FTK_PRIVATE();
    };

    FTK_UI_API void to_json(nlohmann::json&, const ColorControls&);

    FTK_UI_API void from_json(const nlohmann::json&, ColorControls&);

    ///@}
}
