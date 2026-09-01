// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>
#include <ftk/UI/TextEditModel.h>

namespace ftk
{
    class ScrollWidget;

    //! \name Text Widgets
    ///@{

    //! Text edit options.
    struct FTK_UI_API_TYPE TextEditOptions
    {
        FontInfo fontInfo;
        float    cursorBlink       = .5F;
        float    autoScrollTimeout = .05F;

        FTK_UI_API bool operator == (const TextEditOptions&) const;
        FTK_UI_API bool operator != (const TextEditOptions&) const;
    };
        
    //! Text edit widget.
    //! 
    //! \todo Double-click to select text.
    class FTK_UI_API_TYPE TextEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        TextEdit();

    public:
        FTK_UI_API virtual ~TextEdit();

        //! Create a new widget
        FTK_UI_API static std::shared_ptr<TextEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget
        FTK_UI_API static std::shared_ptr<TextEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<TextEditModel>& getModel() const;

        //! Get the scroll widget.
        FTK_UI_API const std::shared_ptr<ScrollWidget>& getScrollWidget() const;

        //! \name Text
        ///@{

        FTK_UI_API const std::vector<std::string>& getText() const;
        FTK_UI_API void setText(const std::vector<std::string>&);
        FTK_UI_API void clearText();
        FTK_UI_API void setCallback(const std::function<void(const std::vector<std::string>&)>&);

        //! Set a callback for when the editing widget inside gains or
        //! loses the key focus.
        FTK_UI_API void setFocusCallback(const std::function<void(bool)>&);

        //! Key focus goes to the editing widget inside, the same as
        //! clicking into the text.
        FTK_UI_API void takeKeyFocus() override;

        FTK_UI_API bool isReadOnly() const;
        FTK_UI_API void setReadOnly(bool);

        ///@}

        //! \name Selection
        ///@{

        FTK_UI_API void selectAll();
        FTK_UI_API void clearSelection();

        ///@}

        //! \name Options
        ///@{

        FTK_UI_API const TextEditOptions& getOptions() const;
        FTK_UI_API std::shared_ptr<IObservable<TextEditOptions> > observeOptions() const;
        FTK_UI_API void setOptions(const TextEditOptions&);

        ///@}

        //! \name Margin
        ///@{

        FTK_UI_API SizeRole getSizeHintRole() const;
        FTK_UI_API void setSizeHintRole(SizeRole);

        FTK_UI_API SizeRole getMarginRole() const;
        FTK_UI_API void setMarginRole(SizeRole);

        ///@}

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    FTK_UI_API void to_json(nlohmann::json&, const TextEditOptions&);

    FTK_UI_API void from_json(const nlohmann::json&, TextEditOptions&);
        
    ///@}
}
