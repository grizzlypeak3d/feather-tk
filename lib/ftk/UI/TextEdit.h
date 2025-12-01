// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>
#include <ftk/UI/TextEditModel.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Text edit options.
    struct TextEditOptions
    {
        FontInfo fontInfo          = FontInfo(getFont(Font::Mono), 12);
        float    cursorBlink       = .5F;
        float    autoScrollTimeout = .05F;

        bool operator == (const TextEditOptions&) const;
        bool operator != (const TextEditOptions&) const;
    };
        
    //! Text edit widget.
    //! 
    //! \todo Double-click to select.
    class TextEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        TextEdit();

    public:
        virtual ~TextEdit();

        //! Create a new widget
        static std::shared_ptr<TextEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>& = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        const std::shared_ptr<TextEditModel>& getModel() const;

        //! Get the text.
        const std::vector<std::string>& getText() const;

        //! Set the text.
        void setText(const std::vector<std::string>&);

        //! Clear the text.
        void clearText();

        //! Set the text callback.
        void setTextCallback(const std::function<void(const std::vector<std::string>&)>&);

        //! Select all.
        void selectAll();

        //! Clear the selection.
        void clearSelection();

        //! Get the options.
        const TextEditOptions& getOptions() const;

        //! Observe the options.
        std::shared_ptr<IObservable<TextEditOptions> > observeOptions() const;

        //! Set the options.
        void setOptions(const TextEditOptions&);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        void takeKeyFocus() override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    void to_json(nlohmann::json&, const TextEditOptions&);

    void from_json(const nlohmann::json&, TextEditOptions&);
        
    ///@}
}
