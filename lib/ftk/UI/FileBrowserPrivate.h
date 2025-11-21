// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/IButton.h>

namespace ftk
{
    class FileBrowserPath : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserPath();

    public:
        virtual ~FileBrowserPath();

        static std::shared_ptr<FileBrowserPath> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setPath(const std::filesystem::path&);

        void setCallback(const std::function<void(const std::filesystem::path&)>&);

        void setEdit(bool);

        void setEditCallback(const std::function<void(bool)>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    class FileBrowserDrives : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserDrives();

    public:
        virtual ~FileBrowserDrives();

        static std::shared_ptr<FileBrowserDrives> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    class FileBrowserShortcuts : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserShortcuts();

    public:
        virtual ~FileBrowserShortcuts();

        static std::shared_ptr<FileBrowserShortcuts> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    class FileBrowserRecent : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserRecent();

    public:
        virtual ~FileBrowserRecent();

        static std::shared_ptr<FileBrowserRecent> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    class FileBrowserSettings : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserSettings();

    public:
        virtual ~FileBrowserSettings();

        static std::shared_ptr<FileBrowserSettings> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    class FileBrowserPanel : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserPanel();

    public:
        virtual ~FileBrowserPanel();

        static std::shared_ptr<FileBrowserPanel> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    class FileBrowserView : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserView();

    public:
        virtual ~FileBrowserView();

        static std::shared_ptr<FileBrowserView> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void reload();

        void setCallback(const std::function<void(const Path&)>&);

        void setSelectCallback(const std::function<void(const Path&)>&);

        void setSearch(const std::string&);

        std::shared_ptr<IObservableValue<int> > observeCurrent() const;

        Box2I getRect(int) const;

        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I& drawRect, const DrawEvent&) override;
        void mouseEnterEvent(MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getItem(const V2I&) const;
        void _directoryUpdate();
        void _setCurrent(int);
        void _doubleClick(int);

        FTK_PRIVATE();
    };
}
