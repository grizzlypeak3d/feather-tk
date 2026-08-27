// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/FileBrowser.h>

#include <ftk/UI/IButton.h>

#include <set>

namespace ftk
{
    //! File browser path widget.
    class FTK_UI_API_TYPE FileBrowserPath : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserPath();

    public:
        FTK_UI_API virtual ~FileBrowserPath();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserPath> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the path.
        FTK_UI_API const std::filesystem::path& getPath() const;

        //! Set the path.
        FTK_UI_API void setPath(const std::filesystem::path&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const std::filesystem::path&)>&);

        //! Get whether the path is ediable.
        FTK_UI_API bool isEditable() const;

        //! Set whether the path is editable.
        FTK_UI_API void setEditable(bool);

        //! Set the editable callback.
        FTK_UI_API void setEditableCallback(const std::function<void(bool)>&);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser drives widget.
    class FTK_UI_API_TYPE FileBrowserDrives : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserDrives();

    public:
        FTK_UI_API virtual ~FileBrowserDrives();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserDrives> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser shortcuts widget.
    class FTK_UI_API_TYPE FileBrowserShortcuts : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserShortcuts();

    public:
        FTK_UI_API virtual ~FileBrowserShortcuts();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserShortcuts> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser recent paths widget.
    class FTK_UI_API_TYPE FileBrowserRecent : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserRecent();

    public:
        FTK_UI_API virtual ~FileBrowserRecent();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserRecent> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the recent files model.
        FTK_UI_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_UI_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser settings widget.
    class FTK_UI_API_TYPE FileBrowserSettings : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserSettings();

    public:
        FTK_UI_API virtual ~FileBrowserSettings();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserSettings> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    //! File browser view widget.
    class FTK_UI_API_TYPE FileBrowserView : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserView();

    public:
        FTK_UI_API virtual ~FileBrowserView();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<FileBrowserView> create(
            const std::shared_ptr<Context>&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Reload the directory.
        FTK_UI_API void reload();

        //! Set the callback. Carries everything selected, so that opening
        //! one of several chosen files opens the ones chosen.
        FTK_UI_API void setCallback(const std::function<void(const std::vector<Path>&)>&);

        //! Set the selection callback. More than one path only when
        //! selecting more than one is allowed; empty when nothing is
        //! selected.
        FTK_UI_API void setSelectCallback(const std::function<void(const std::vector<Path>&)>&);

        //! Set the key focus callback. The view draws its current item;
        //! the ring that says the list itself has the keyboard is drawn
        //! around its scroll area by whoever owns it, and this is how the
        //! owner hears.
        FTK_UI_API void setKeyFocusCallback(const std::function<void(bool)>&);

        //! Get whether more than one file can be selected.
        FTK_UI_API bool isMultiple() const;

        //! Set whether more than one file can be selected. Shift extends the
        //! selection from the current item and the command key adds to it;
        //! without this a click replaces whatever was selected.
        FTK_UI_API void setMultiple(bool);

        //! Get the selection.
        FTK_UI_API std::vector<Path> getSelection() const;

        //! Get the search filter.
        FTK_UI_API const std::string& getSearch() const;

        //! Set the search filter.
        FTK_UI_API void setSearch(const std::string&);

        //! Observe the current item.
        FTK_UI_API std::shared_ptr<IObservable<int> > observeCurrent() const;

        //! Observe the number of items shown.
        FTK_UI_API std::shared_ptr<IObservable<size_t> > observeItemCount() const;

        //! Get the rectangle of an item.
        FTK_UI_API Box2I getRect(int) const;

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void tickEvent(bool, bool, const TickEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I& drawRect, const DrawEvent&) override;
        FTK_UI_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getItem(const V2I&) const;
        void _directoryUpdate();
        void _cancelThumbnails();
        void _setCurrent(int);
        void _toggleCurrent(int);
        void _selectRange(int);
        void _selectionUpdate(const std::set<int>&, bool currentChanged);
        void _clearCurrent();
        void _doubleClick(int);

        FTK_PRIVATE();
    };
}
