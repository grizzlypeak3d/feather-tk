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
    struct FTK_API_TYPE TextEditOptions
    {
        FontInfo fontInfo          = FontInfo(getFont(Font::Mono), 12);
        float    cursorBlink       = .5F;
        float    autoScrollTimeout = .05F;

        bool operator == (const TextEditOptions&) const;
        bool operator != (const TextEditOptions&) const;
    };
        
    //! Text edit widget.
    //! 
    //! \todo Double-click to select text.
    class FTK_API_TYPE TextEdit : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        TextEdit();

    public:
        FTK_API virtual ~TextEdit();

        //! Create a new widget
        FTK_API static std::shared_ptr<TextEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>& = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_API const std::shared_ptr<TextEditModel>& getModel() const;

        //! Get the text.
        FTK_API const std::vector<std::string>& getText() const;

        //! Set the text.
        FTK_API void setText(const std::vector<std::string>&);

        //! Clear the text.
        FTK_API void clearText();

        //! Set the text callback.
        FTK_API void setTextCallback(const std::function<void(const std::vector<std::string>&)>&);

        //! Select all.
        FTK_API void selectAll();

        //! Clear the selection.
        FTK_API void clearSelection();

        //! Get the options.
        FTK_API const TextEditOptions& getOptions() const;

        //! Observe the options.
        FTK_API std::shared_ptr<IObservable<TextEditOptions> > observeOptions() const;

        //! Set the options.
        FTK_API void setOptions(const TextEditOptions&);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        FTK_API void takeKeyFocus() override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const TextEditOptions&);

    FTK_API void from_json(const nlohmann::json&, TextEditOptions&);
        
    ///@}
}
