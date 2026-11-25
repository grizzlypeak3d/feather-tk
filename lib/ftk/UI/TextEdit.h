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
        //! The font. Its size is scaled by the display scale.
        FontInfo fontInfo;
        //! How long, in seconds, the cursor is shown and then hidden as it
        //! blinks.
        float    cursorBlink       = .5F;
        //! The time, in seconds, between the steps of the scrolling while a
        //! selection is dragged past the edge.
        float    autoScrollTimeout = .05F;

        bool operator == (const TextEditOptions&) const = default;
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

        //! Get the text, one string for each line.
        FTK_UI_API const std::vector<std::string>& getText() const;
        //! Set the text, one string for each line. The cursor moves to the
        //! start and the selection is cleared.
        FTK_UI_API void setText(const std::vector<std::string>&);
        //! Clear the text.
        FTK_UI_API void clearText();
        //! Set the callback, called with the text whenever it changes: on every
        //! edit, and from setting the text.
        FTK_UI_API void setCallback(const std::function<void(const std::vector<std::string>&)>&);

        //! Set a callback for when the editing widget inside gains or
        //! loses the key focus.
        FTK_UI_API void setFocusCallback(const std::function<void(bool)>&);

        //! Key focus goes to the editing widget inside, the same as
        //! clicking into the text.
        FTK_UI_API void takeKeyFocus() override;

        //! Get whether the text is read only. Read only text can still be
        //! selected and copied.
        FTK_UI_API bool isReadOnly() const;
        //! Set whether the text is read only.
        FTK_UI_API void setReadOnly(bool);

        ///@}

        //! \name Selection
        ///@{

        //! Select all of the text.
        FTK_UI_API void selectAll();
        //! Clear the selection.
        FTK_UI_API void clearSelection();

        ///@}

        //! \name Options
        ///@{

        //! Get the options.
        FTK_UI_API const TextEditOptions& getOptions() const;
        //! Observe the options.
        FTK_UI_API std::shared_ptr<IObservable<TextEditOptions> > observeOptions() const;
        //! Set the options.
        FTK_UI_API void setOptions(const TextEditOptions&);

        ///@}

        //! \name Margin
        ///@{

        //! Get the size hint role, the size the text is given to scroll in.
        //! "None" asks for the height of the text instead, so that the edit
        //! does not scroll vertically.
        FTK_UI_API SizeRole getSizeHintRole() const;
        //! Set the size hint role.
        FTK_UI_API void setSizeHintRole(SizeRole);

        //! Get the margin role, the room kept around the edit outside its
        //! border. The default is none.
        FTK_UI_API SizeRole getMarginRole() const;
        //! Set the margin role.
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
