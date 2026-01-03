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
        FTK_API virtual ~FileBrowserPath();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserPath> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the path.
        FTK_API const std::filesystem::path& getPath() const;

        //! Set the path.
        FTK_API void setPath(const std::filesystem::path&);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const std::filesystem::path&)>&);

        //! Get whether the path is ediable.
        FTK_API bool isEditable() const;

        //! Set whether the path is editable.
        FTK_API void setEditable(bool);

        //! Set the editable callback.
        FTK_API void setEditableCallback(const std::function<void(bool)>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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
        FTK_API virtual ~FileBrowserDrives();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserDrives> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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
        FTK_API virtual ~FileBrowserShortcuts();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserShortcuts> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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
        FTK_API virtual ~FileBrowserRecent();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserRecent> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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
        FTK_API virtual ~FileBrowserSettings();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserSettings> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

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
        FTK_API virtual ~FileBrowserView();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserView> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Reload the directory.
        FTK_API void reload();

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const Path&)>&);

        //! Set the selection callback.
        FTK_API void setSelectCallback(const std::function<void(const Path&)>&);

        //! Get the search filter.
        FTK_API const std::string& getSearch() const;

        //! Set the search filter.
        FTK_API void setSearch(const std::string&);

        //! Observe the current item.
        FTK_API std::shared_ptr<IObservable<int> > observeCurrent() const;

        //! Get the rectangle of an item.
        FTK_API Box2I getRect(int) const;

        FTK_API Size2I getSizeHint() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I& drawRect, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getItem(const V2I&) const;
        void _directoryUpdate();
        void _setCurrent(int);
        void _doubleClick(int);

        FTK_PRIVATE();
    };
}
