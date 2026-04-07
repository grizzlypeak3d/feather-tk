// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Color.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/ObservableMap.h>

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

        //! \name Size Roles
        ///@{

        FTK_API const std::map<SizeRole, int>& getSizeRoles() const;
        FTK_API std::shared_ptr<IObservableMap<SizeRole, int> > observeSizeRoles() const;
        FTK_API void setSizeRoles(const std::map<SizeRole, int>&);

        FTK_API int getSizeRole(SizeRole, float scale) const;

        ///@}

        //! \name Color Roles
        ///@{

        FTK_API const std::map<ColorRole, Color4F>& getColorRoles() const;
        FTK_API std::shared_ptr<IObservableMap<ColorRole, Color4F> > observeColorRoles() const;
        FTK_API void setColorRoles(const std::map<ColorRole, Color4F>&);

        FTK_API Color4F getColorRole(ColorRole) const;

        ///@}

        //! \name Color Controls
        ///@{

        FTK_API const ColorControls& getColorControls() const;
        FTK_API std::shared_ptr<IObservable<ColorControls> > observeColorControls() const;
        FTK_API void setColorControls(const ColorControls&);

        ///@}

        //! \name Fonts
        ///@{

        FTK_API const std::map<FontType, std::string>& getFonts() const;
        FTK_API std::shared_ptr<IObservableMap<FontType, std::string> > observeFonts() const;
        FTK_API void setFonts(const std::map<FontType, std::string>&);

        FTK_API FontInfo getFont(FontType, float scale) const;
        FTK_API FontInfo getFont(FontType, int size, float scale) const;

        ///@}

    private:
        void _colorUpdate();

        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const ColorControls&);

    FTK_API void from_json(const nlohmann::json&, ColorControls&);

    ///@}
}
