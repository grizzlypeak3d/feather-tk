// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/TextEdit.h>

namespace ftk
{
    class TextEditWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        TextEditWidget();

    public:
        virtual ~TextEditWidget();

        static std::shared_ptr<TextEditWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<TextEditModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCallback(const std::function<void(const std::vector<std::string>&)>&);
        void setFocusCallback(const std::function<void(bool)>&);

        void setOptions(const TextEditOptions&);

        Box2I getCursorBox(bool margin = false) const;

        void setGeometry(const Box2I&) override;
        void setVisible(bool) override;
        void setEnabled(bool) override;
        void tickEvent(
            bool,
            bool,
            const TickEvent&) override;
        Size2I getSizeHint() const override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;
        void textEvent(TextEvent&) override;

    private:
        TextEditPos _getCursorPos(const V2I&) const;
        void _cursorReset();

        FTK_PRIVATE();
    };
}
