// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! RGB color widget.
    //!
    //! \todo Add support for displaying pixel types like U8, U16, etc.?
    class FTK_UI_API_TYPE RGBColorWidget : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        RGBColorWidget();

    public:
        FTK_UI_API virtual ~RGBColorWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<RGBColorWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color.
        FTK_UI_API void setColor(const Color4F&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        //! Set the callback with a flag for whether the widget is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const Color4F&, bool)>&);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);


    private:
        void _colorUpdate();

        FTK_PRIVATE();
    };

    //! HSV color widget.
    class FTK_UI_API_TYPE HSVColorWidget : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        HSVColorWidget();

    public:
        FTK_UI_API virtual ~HSVColorWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<HSVColorWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color.
        FTK_UI_API void setColor(const Color4F&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        //! Set the callback with a flag for whether the widget is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const Color4F&, bool)>&);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);


    private:
        void _colorUpdate();

        FTK_PRIVATE();
    };

    //! Palette color widget: a grid of preset colors for fast, consistent
    //! selection.
    class FTK_UI_API_TYPE PaletteColorWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        PaletteColorWidget();

    public:
        FTK_UI_API virtual ~PaletteColorWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<PaletteColorWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color. A color in the palette is shown selected.
        FTK_UI_API void setColor(const Color4F&);

        //! Set the palette colors.
        FTK_UI_API void setColors(const std::vector<Color4F>&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;

    private:
        Box2I _cellRect(size_t index) const;

        FTK_PRIVATE();
    };

    //! Color widget mode.
    enum class FTK_UI_API_TYPE ColorWidgetMode
    {
        RGB,
        HSV,
        Palette,

        Count,
        First = RGB
    };
    FTK_ENUM(FTK_UI_API, ColorWidgetMode);

    //! Color widget.
    //!
    //! \todo Add support for displaying pixel types like U8, U16, etc.?
    class FTK_UI_API_TYPE ColorWidget : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ColorWidget();

    public:
        FTK_UI_API virtual ~ColorWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ColorWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the color.
        FTK_UI_API const Color4F& getColor() const;

        //! Set the color.
        FTK_UI_API void setColor(const Color4F&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const Color4F&)>&);

        //! Set the callback with a flag for whether the widget is pressed.
        FTK_UI_API void setPressedCallback(const std::function<void(const Color4F&, bool)>&);

        //! Get the widget mode.
        FTK_UI_API ColorWidgetMode getMode() const;

        //! Set the widget mode.
        FTK_UI_API void setMode(ColorWidgetMode);


    private:
        void _modeUpdate();
        void _colorUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
