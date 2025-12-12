// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IButton.h>

namespace ftk
{
    //! \name Buttons
    ///@{
        
    //! Check box.
    class FTK_API_TYPE CheckBox : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        CheckBox();

    public:
        FTK_API virtual ~CheckBox();

        //! Create a new widget.
        FTK_API static std::shared_ptr<CheckBox> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<CheckBox> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API void setText(const std::string&) override;
        FTK_API void setFontRole(FontRole) override;

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
