// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IDialog.h>

#include <ftk/Core/ISystem.h>
#include <ftk/Core/Path.h>

#include <filesystem>

namespace ftk
{
    class RecentFilesModel;

    //! \name File Widgets
    ///@{

    //! File browser mode.
    enum class FileBrowserMode
    {
        File,
        Dir,

        Count,
        First = File
    };
    FTK_ENUM(FileBrowserMode);

    //! File browser options.
    struct FileBrowserOptions
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

        bool operator == (const FileBrowserOptions&) const;
        bool operator != (const FileBrowserOptions&) const;
    };

    //! File browser model.
    class FileBrowserModel : public std::enable_shared_from_this<FileBrowserModel>
    {
    public:
        FileBrowserModel(const std::shared_ptr<Context>&);

    public:
        ~FileBrowserModel();

        //! Create a new model.
        static std::shared_ptr<FileBrowserModel> create(
            const std::shared_ptr<Context>&);

        //! Get the current path.
        const std::filesystem::path& getPath() const;

        //! Observe the current path.
        std::shared_ptr<IObservable<std::filesystem::path> > observePath() const;

        //! Set the current path.
        void setPath(const std::filesystem::path&);

        //! Go forward to the next path.
        void forward();

        //! Observe whether there is a next path.
        std::shared_ptr<IObservable<bool> > observeHasForward() const;

        //! Go back to the previous path.
        void back();

        //! Observe whether there is a previous path.
        std::shared_ptr<IObservable<bool> > observeHasBack() const;

        //! Get the options.
        const FileBrowserOptions& getOptions() const;

        //! Observe the options.
        std::shared_ptr<IObservable<FileBrowserOptions> > observeOptions() const;

        //! Set the options.
        void setOptions(const FileBrowserOptions&);

        //! Get the extensions.
        const std::vector<std::string>& getExts() const;

        //! Observe the extensions.
        std::shared_ptr<IObservableList<std::string> > observeExts() const;

        //! Set the extensions.
        void setExts(const std::vector<std::string>&);

        //! Get the current extension.
        const std::string& getExt() const;

        //! Observe the current extension.
        std::shared_ptr<IObservable<std::string> > observeExt() const;

        //! Set the current extension.
        void setExt(const std::string&);

    private:
        FTK_PRIVATE();
    };

    //! File browser widget.
    class FileBrowserWidget : public IMouseWidget
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
        virtual ~FileBrowserWidget();

        //! Create a new widget.
        static std::shared_ptr<FileBrowserWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::File,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the title.
        void setTitle(const std::string&);

        //! Set the callback.
        void setCallback(const std::function<void(const Path&)>&);

        //! Set the cancel callback.
        void setCancelCallback(const std::function<void(void)>&);

        //! Get the file browser model.
        const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _accept(const std::string&);

        void _optionsUpdate();
        void _extsUpdate();

        FTK_PRIVATE();
    };

    //! File browser dialog.
    class FileBrowser : public IDialog
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
        virtual ~FileBrowser();

        //! Create a new dialog.
        static std::shared_ptr<FileBrowser> create(
            const std::shared_ptr<Context>&,
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::File,
            const std::shared_ptr<FileBrowserModel>& model = nullptr,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the title.
        void setTitle(const std::string&);

        //! Set the callback.
        void setCallback(const std::function<void(const Path&)>&);

        //! Get the file browser model.
        const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

    private:
        FTK_PRIVATE();
    };

    //! File browser system.
    class FileBrowserSystem : public ISystem
    {
    protected:
        FileBrowserSystem(const std::shared_ptr<Context>&);

    public:
        virtual ~FileBrowserSystem();

        //! Create a new system.
        static std::shared_ptr<FileBrowserSystem> create(
            const std::shared_ptr<Context>&);

        //! Open the file browser.
        void open(
            const std::shared_ptr<IWindow>&,
            const std::function<void(const Path&)>&,
            const std::string& title = "Open",
            const std::filesystem::path& = std::filesystem::path(),
            FileBrowserMode = FileBrowserMode::File);

        //! Get whether the native file dialog is used.
        bool isNativeFileDialog() const;

        //! Set whether the native file dialog is used.
        void setNativeFileDialog(bool);

        //! Get the file browser model.
        const std::shared_ptr<FileBrowserModel>& getModel() const;

        //! Get the recent files model.
        const std::shared_ptr<RecentFilesModel>& getRecentFilesModel() const;

        //! Set the recent files model.
        void setRecentFilesModel(const std::shared_ptr<RecentFilesModel>&);

    private:
        FTK_PRIVATE();
    };

    void to_json(nlohmann::json&, const FileBrowserOptions&);

    void from_json(const nlohmann::json&, FileBrowserOptions&);

    ///@}
}
