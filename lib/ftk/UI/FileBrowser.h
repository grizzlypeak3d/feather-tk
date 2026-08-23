// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IDialog.h>

#include <ftk/Core/ISystem.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Path.h>

#include <filesystem>
#include <future>

namespace ftk
{
    class App;
    class FileBrowserView;
    class RecentFilesModel;

    //! \name File Widgets
    ///@{

    //! File browser mode.
    enum class FTK_API_TYPE FileBrowserMode
    {
        Open,
        Save,
        Dir,

        Count,
        First = Open
    };
    FTK_ENUM(FileBrowserMode);

    //! File browser thumbnails.
    //!
    //! How large the thumbnails are, and whether to ask for them at all:
    //! reading them is work, and a directory on the other end of a network
    //! is somewhere that work is felt.
    enum class FTK_API_TYPE FileBrowserThumbnails
    {
        Off,
        Small,
        Medium,
        Large,

        Count,
        First = Off
    };
    FTK_ENUM(FileBrowserThumbnails);

    //! Get the height of a thumbnail, from the style's size for one.
    FTK_API int getThumbnailHeight(FileBrowserThumbnails, int sizeRole);

    //! File browser options.
    struct FTK_API_TYPE FileBrowserOptions
    {
        DirListOptions              dirList;
        FileBrowserThumbnails       thumbnails   = FileBrowserThumbnails::Medium;
        bool                        panel        = true;
        bool                        pathEditable = false;
        std::map<std::string, bool> bellows =
        {
            { "Drives",    true },
            { "Shortcuts", true },
            { "Recent",    true },
            { "Settings",  false }
        };

        FTK_API bool operator == (const FileBrowserOptions&) const;
        FTK_API bool operator != (const FileBrowserOptions&) const;
    };

    //! File browser thumbnail request.
    struct FTK_API_TYPE FileBrowserThumbnailRequest
    {
        uint64_t id = 0;
        std::future<std::shared_ptr<Image> > future;
    };

    //! Base class for file browser thumbnails.
    //!
    //! Reading the images is not something ftk can do; an application that
    //! can registers an implementation of this with FileBrowserSystem, and
    //! the browser shows thumbnails in place of the file icons.
    class FTK_API_TYPE IFileBrowserThumbnails
    {
    public:
        FTK_API virtual ~IFileBrowserThumbnails() = 0;

        //! Get whether a thumbnail can be made for the given path. Asked of
        //! every entry in a directory, so this is meant to be as cheap as
        //! looking at the extension rather than opening the file.
        virtual bool isSupported(const Path&) const = 0;

        //! Request a thumbnail of the given height. The future carries a null
        //! image when the file turned out to have nothing to show.
        virtual FileBrowserThumbnailRequest request(const Path&, int height) = 0;

        //! Cancel requests. The browser cancels what scrolls out of view, so
        //! this is asked for often and with ids that may already be finished.
        virtual void cancelRequests(const std::vector<uint64_t>&) = 0;
    };

    //! File browser model.
    class FTK_API_TYPE FileBrowserModel : public std::enable_shared_from_this<FileBrowserModel>
    {
    public:
        FileBrowserModel(const std::shared_ptr<Context>&);

    public:
        FTK_API ~FileBrowserModel();

        //! Create a new model.
        FTK_API static std::shared_ptr<FileBrowserModel> create(
            const std::shared_ptr<Context>&);

        //! Get the current path.
        FTK_API const std::filesystem::path& getPath() const;

        //! Observe the current path.
        FTK_API std::shared_ptr<IObservable<std::filesystem::path> > observePath() const;

        //! Set the current path.
        FTK_API void setPath(const std::filesystem::path&);

        //! Set the current path.
        //!
        //! The name is UTF-8, which is what a path read back from the
        //! settings holds. Without this overload the conversion is left to
        //! the implementation, and on Windows that reads a narrow string as
        //! the active code page.
        FTK_API void setPath(const std::string&);

        //! Go forward to the next path.
        FTK_API void forward();

        //! Observe whether there is a next path.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasForward() const;

        //! Go back to the previous path.
        FTK_API void back();

        //! Observe whether there is a previous path.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasBack() const;

        //! Get the options.
        FTK_API const FileBrowserOptions& getOptions() const;

        //! Observe the options.
        FTK_API std::shared_ptr<IObservable<FileBrowserOptions> > observeOptions() const;

        //! Set the options.
        FTK_API void setOptions(const FileBrowserOptions&);

        //! Get the extensions.
        FTK_API const std::vector<std::string>& getExts() const;

        //! Observe the extensions.
        FTK_API std::shared_ptr<IObservableList<std::string> > observeExts() const;

        //! Set the extensions.
        FTK_API void setExts(const std::vector<std::string>&);

        //! Get the current extension.
        FTK_API const std::string& getExt() const;

        //! Observe the current extension.
        FTK_API std::shared_ptr<IObservable<std::string> > observeExt() const;

        //! Set the current extension.
        FTK_API void setExt(const std::string&);

        //! Get the extension filter.
        FTK_API const std::vector<std::string>& getExtsFilter() const;

        //! Get the label for the extension filter.
        FTK_API const std::string& getExtsFilterLabel() const;

        //! Set an extension filter, e.g. { ".djvr" }. Unlike setExts(), which
        //! only fills the extensions combo box and leaves "*.*" showing every
        //! file, a filter restricts the listing: the combo box narrows within
        //! it. The label names the filter in place of "*.*".
        FTK_API void setExtsFilter(
            const std::vector<std::string>&,
            const std::string& label = std::string());

    private:
        FTK_PRIVATE();
    };

    //! File browser widget.
    class FTK_API_TYPE FileBrowserWidget : public IMouseWidget
    {
    protected:
        FTK_API void _init(
            const std::shared_ptr<Context>&,
            const std::filesystem::path&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>& model,
            const std::shared_ptr<IWidget>& parent);

        FileBrowserWidget();

    public:
        FTK_API virtual ~FileBrowserWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<FileBrowserWidget> create(
            const std::shared_ptr<Context>&,
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::Open,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const std::vector<Path>&)>&);

        //! Set the cancel callback.
        FTK_API void setCancelCallback(const std::function<void(void)>&);

        //! Get whether more than one file can be selected.
        FTK_API bool isMultiple() const;

        //! Set whether more than one file can be selected.
        FTK_API void setMultiple(bool);

        //! Get the file browser model.
        FTK_API const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        //! Get the file browser view.
        FTK_API std::shared_ptr<FileBrowserView> getView() const;

        //! Get whether the pin is shown.
        FTK_API bool isPinVisible() const;

        //! Set whether the pin is shown.
        //!
        //! Pinning only means anything to a browser in a window of its own:
        //! a dialog that would not close covers what it was opened from
        //! for good.
        FTK_API void setPinVisible(bool);

        //! Get whether the browser is pinned.
        FTK_API bool isPinned() const;

        //! Set whether the browser is pinned.
        FTK_API void setPinned(bool);

        //! Set the pinned callback.
        FTK_API void setPinnedCallback(const std::function<void(bool)>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void clipEvent(const Box2I&, bool clipped) override;
        FTK_API void keyPressEvent(KeyEvent&) override;

    private:
        void _accept(const std::vector<Path>&);

        void _optionsUpdate();
        void _extsUpdate();

        FTK_PRIVATE();
    };

    //! File browser dialog.
    class FTK_API_TYPE FileBrowser : public IDialog
    {
    protected:
        FTK_API void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::filesystem::path&,
            FileBrowserMode,
            const std::shared_ptr<FileBrowserModel>& model,
            const std::shared_ptr<IWidget>& parent);

        FileBrowser();

    public:
        FTK_API virtual ~FileBrowser();

        //! Create a new dialog.
        FTK_API static std::shared_ptr<FileBrowser> create(
            const std::shared_ptr<Context>&,
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::Open,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const std::vector<Path>&)>&);

        //! Get whether more than one file can be selected.
        FTK_API bool isMultiple() const;

        //! Set whether more than one file can be selected.
        FTK_API void setMultiple(bool);

        //! Get the file browser model.
        FTK_API const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        FTK_API virtual std::shared_ptr<IWidget> getKeyFocus() const override;

    private:
        FTK_PRIVATE();
    };

    //! File browser open options.
    struct FTK_API_TYPE FileBrowserOpenOptions
    {
        std::string           title = "Open";

        //! The directory to open at. Empty stays wherever the browser was
        //! last left.
        std::filesystem::path path;

        FileBrowserMode       mode = FileBrowserMode::Open;

        //! Restrict the listing to these extensions, e.g. { ".djvr" }. Empty
        //! shows every file.
        std::vector<std::string> extensions;

        //! Names the group of extensions: the filter group in the native
        //! dialog, and the entry shown in place of "*.*" in the built-in one.
        std::string           extensionsLabel = "Supported";

        //! Allow more than one file to be chosen. Only meaningful when
        //! opening; there is one file to save to and one directory to pick.
        bool                  multiple = false;
    };

    //! File browser system.
    class FTK_API_TYPE FileBrowserSystem : public ISystem
    {
    protected:
        FileBrowserSystem(const std::shared_ptr<Context>&);

    public:
        FTK_API virtual ~FileBrowserSystem();

        //! Create a new system.
        FTK_API static std::shared_ptr<FileBrowserSystem> create(
            const std::shared_ptr<Context>&);

        //! Open the file browser.
        FTK_API void open(
            const std::shared_ptr<IWindow>&,
            const std::function<void(const Path&)>&,
            const FileBrowserOpenOptions& = FileBrowserOpenOptions());

        //! Open the file browser for more than one file. Set
        //! FileBrowserOpenOptions::multiple for the choosing to allow it;
        //! without that this reports the one file chosen.
        FTK_API void open(
            const std::shared_ptr<IWindow>&,
            const std::function<void(const std::vector<Path>&)>&,
            const FileBrowserOpenOptions& = FileBrowserOpenOptions());

        //! Get whether the native file dialog can be used.
        //!
        //! It is not built everywhere: it means linking to the desktop's own
        //! file dialog, which is not wanted on every platform. Where it is
        //! not built, setNativeFileDialog() is remembered but has no effect.
        FTK_API bool isNativeFileDialogAvailable() const;

        //! Get whether the native file dialog is used.
        FTK_API bool isNativeFileDialog() const;

        //! Set whether the native file dialog is used.
        FTK_API void setNativeFileDialog(bool);

        //! Get whether the file browser is shown in a window of its own.
        FTK_API bool isFloating() const;

        //! Get whether the browser stays open once a file is chosen.
        FTK_API bool isPinned() const;

        //! Set whether the browser stays open once a file is chosen.
        //!
        //! Only a floating browser can be pinned. Choosing reports the file
        //! and leaves the browser up, so that a run of files can be opened
        //! one after another without asking for the browser each time.
        FTK_API void setPinned(bool);

        //! Set whether the file browser is shown in a window of its own.
        //!
        //! The dialog covers the window it was opened from, which hides
        //! whatever the choice is being made about. A floating browser
        //! leaves it visible and usable. Takes effect the next time the
        //! browser is opened, and has no bearing on the native dialog,
        //! which is a window of its own either way.
        FTK_API void setFloating(bool);

        //! Get the size a floating file browser opens at.
        //!
        //! While one is open this is the size it has been resized to, so
        //! that an application saving this gets the size the browser was
        //! left at rather than the one it was given.
        FTK_API Size2I getWindowSize() const;

        //! Set the size a floating file browser opens at.
        FTK_API void setWindowSize(const Size2I&);

        //! Close the file browser if it is floating.
        //!
        //! A window of its own is not a child of anything, so it outlives
        //! the window it was opened from unless something closes it.
        FTK_API void close();

        //! Get the file browser model.
        FTK_API const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        //! Get the thumbnails.
        FTK_API const std::shared_ptr<IFileBrowserThumbnails>& getThumbnails() const;

        //! Set the thumbnails. Without one the browser shows file icons.
        FTK_API void setThumbnails(const std::shared_ptr<IFileBrowserThumbnails>&);

    private:
        void _openDialog(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWindow>&,
            const std::function<void(const std::vector<Path>&)>&,
            const FileBrowserOpenOptions&,
            const std::shared_ptr<FileBrowserModel>&);
        void _openWindow(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const std::function<void(const std::vector<Path>&)>&,
            const FileBrowserOpenOptions&,
            const std::shared_ptr<FileBrowserModel>&);

        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const FileBrowserOptions&);

    FTK_API void from_json(const nlohmann::json&, FileBrowserOptions&);

    ///@}
}
