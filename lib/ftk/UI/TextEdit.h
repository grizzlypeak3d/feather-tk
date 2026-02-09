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

        //! \name Text
        ///@{

        FTK_API const std::vector<std::string>& getText() const;
        FTK_API void setText(const std::vector<std::string>&);
        FTK_API void clearText();
        FTK_API void setCallback(const std::function<void(const std::vector<std::string>&)>&);

        ///@}

        //! \name Selection
        ///@{

        FTK_API void selectAll();
        FTK_API void clearSelection();

        ///@}

        //! \name Options
        ///@{

        FTK_API const TextEditOptions& getOptions() const;
        FTK_API std::shared_ptr<IObservable<TextEditOptions> > observeOptions() const;
        FTK_API void setOptions(const TextEditOptions&);

        ///@}

        //! \name Margin
        ///@{

        FTK_API SizeRole getMarginRole() const;
        FTK_API void setMarginRole(SizeRole);

        ///@}

        FTK_API void takeKeyFocus() override;
        FTK_API Size2I getSizeHint() const override;
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
