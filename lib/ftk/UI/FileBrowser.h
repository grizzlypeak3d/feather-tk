// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IDialog.h>

#include <ftk/Core/ISystem.h>
#include <ftk/Core/Path.h>

#include <filesystem>

namespace ftk
{
    class FileBrowserView;
    class RecentFilesModel;

    //! \name File Widgets
    ///@{

    //! File browser mode.
    enum class FTK_API_TYPE FileBrowserMode
    {
        File,
        Dir,

        Count,
        First = File
    };
    FTK_ENUM(FileBrowserMode);

    //! File browser options.
    struct FTK_API_TYPE FileBrowserOptions
    {
        DirListOptions              dirList;
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

    private:
        FTK_PRIVATE();
    };

    //! File browser widget.
    class FTK_API_TYPE FileBrowserWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
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
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::File,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the title.
        FTK_API void setTitle(const std::string&);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const Path&)>&);

        //! Set the cancel callback.
        FTK_API void setCancelCallback(const std::function<void(void)>&);

        //! Get the file browser model.
        FTK_API const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        //! Get the file browser view.
        FTK_API std::shared_ptr<FileBrowserView> getView() const;

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        void _accept(const std::string&);

        void _optionsUpdate();
        void _extsUpdate();

        FTK_PRIVATE();
    };

    //! File browser dialog.
    class FTK_API_TYPE FileBrowser : public IDialog
    {
    protected:
        void _init(
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
            FileBrowserMode = FileBrowserMode::File,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the title.
        FTK_API void setTitle(const std::string&);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const Path&)>&);

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
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::File);

        //! Get whether the native file dialog is used.
        FTK_API bool isNativeFileDialog() const;

        //! Set whether the native file dialog is used.
        FTK_API void setNativeFileDialog(bool);

        //! Get the file browser model.
        FTK_API const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        FTK_API const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        FTK_API void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

    private:
        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const FileBrowserOptions&);

    FTK_API void from_json(const nlohmann::json&, FileBrowserOptions&);

    ///@}
}
