// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TextEditModel.h>

namespace textedit
{
    class App;

    //! Status bar widget.
    class StatusBar : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        StatusBar() = default;

    public:
        virtual ~StatusBar();

        //! Create a new widget.
        static std::shared_ptr<StatusBar> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        void setGeometry(const ftk::Box2I&) override;
        void sizeHintEvent(const ftk::SizeHintEvent&) override;

    private:
        void _textUpdate();

        int _lineCount = 0;
        int _currentLine = 0;
        int _currentColumn = 0;

        std::shared_ptr<ftk::Label> _linesLabel;
        std::shared_ptr<ftk::HorizontalLayout> _layout;

        std::shared_ptr<ftk::Observer<std::shared_ptr<ftk::IDocument> > > _currentObserver;
        std::shared_ptr<ftk::ListObserver<std::string> > _textObserver;
        std::shared_ptr<ftk::Observer<ftk::TextEditPos> > _cursorObserver;
    };
}
