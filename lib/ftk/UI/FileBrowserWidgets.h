// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/IButton.h>

namespace ftk
{
    //! File browser path widget.
    class FTK_API_TYPE FileBrowserPath : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserPath();

    public:
        virtual ~FileBrowserPath();

        //! Create a new widget.
        static std::shared_ptr<FileBrowserPath> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the path.
        const std::filesystem::path& getPath() const;

        //! Set the path.
        void setPath(const std::filesystem::path&);

        //! Set the callback.
        void setCallback(const std::function<void(const std::filesystem::path&)>&);

        //! Get whether the path is ediable.
        bool isEditable() const;

        //! Set whether the path is editable.
        void setEditable(bool);

        //! Set the editable callback.
        void setEditableCallback(const std::function<void(bool)>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser drives widget.
    class FTK_API_TYPE FileBrowserDrives : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserDrives();

    public:
        virtual ~FileBrowserDrives();

        //! Create a new widget.
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

    //! File browser shortcuts widget.
    class FTK_API_TYPE FileBrowserShortcuts : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserShortcuts();

    public:
        virtual ~FileBrowserShortcuts();

        //! Create a new widget.
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

    //! File browser recent paths widget.
    class FTK_API_TYPE FileBrowserRecent : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserRecent();

    public:
        virtual ~FileBrowserRecent();

        //! Create a new widget.
        static std::shared_ptr<FileBrowserRecent> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the recent files model.
        const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser settings widget.
    class FTK_API_TYPE FileBrowserSettings : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserSettings();

    public:
        virtual ~FileBrowserSettings();

        //! Create a new widget.
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

    //! File browser view widget.
    class FTK_API_TYPE FileBrowserView : public IMouseWidget
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

        //! Create a new widget.
        static std::shared_ptr<FileBrowserView> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Reload the directory.
        void reload();

        //! Set the callback.
        void setCallback(const std::function<void(const Path&)>&);

        //! Set the selection callback.
        void setSelectCallback(const std::function<void(const Path&)>&);

        //! Get the search filter.
        const std::string& getSearch() const;

        //! Set the search filter.
        void setSearch(const std::string&);

        //! Observe the current item.
        std::shared_ptr<IObservable<int> > observeCurrent() const;

        //! Get the rectangle of an item.
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
