# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

# Text editor with multiple documents: the Python counterpart of the C++
# textedit example, and a template for structuring a feather-tk
# application in Python. The pieces and their names mirror the C++
# example so the two can be read side by side; where the languages
# differ, the difference is commented.
#
# The shape is the same in both languages: models hold the state,
# widgets observe the models, and actions are shared between the menus
# and the tool bar so their enabled and checked states stay in sync
# everywhere at once.

import ftkPy as ftk

import os
import sys
import weakref


def weak(method):
    """
    Wrap a bound method for use as a widget or action callback.

    A callback handed to the C++ side is held by a std::function, an
    edge the Python garbage collector cannot see. A bound method or a
    lambda that captures self strongly therefore forms a cycle that is
    never collected, and everything the cycle holds leaks until process
    exit.
    """
    r = weakref.WeakMethod(method)
    def call(*args, **kwargs):
        m = r()
        if m is not None:
            return m(*args, **kwargs)
    return call


class Document(ftk.IDocument):
    """
    A single text file: the text model plus the file identity.

    Deriving from ftk.IDocument is what lets the shared DocumentModel
    manage these; the interface itself is only an identity, so
    everything of substance lives here.
    """
    def __init__(self, context, path = None):
        ftk.IDocument.__init__(self)

        lines = []
        if path:
            with open(path) as f:
                lines = [line.rstrip("\n") for line in f]
        self.model = ftk.TextEditModel(context, lines)

        self._path = path
        self._name = ftk.ObservableString("")
        self._tooltip = ftk.ObservableString("")
        self._changed = ftk.ObservableBool(False)
        self._nameUpdate()

        # Any edit marks the document changed. The initial callback is
        # suppressed so that merely loading the file does not.
        selfWeak = weakref.ref(self)
        self._textObserver = ftk.StringListObserver(
            self.model.observeText(),
            lambda lines: selfWeak()._textChanged(),
            ftk.ObserverAction.Suppress)

    @property
    def path(self):
        return self._path

    def observeName(self):
        return self._name

    def observeTooltip(self):
        return self._tooltip

    def isChanged(self):
        return self._changed.get()

    def observeChanged(self):
        return self._changed

    def save(self, path = None):
        if path is not None:
            self._path = path
        with open(self._path, "w") as f:
            f.write("\n".join(self.model.text) + "\n")
        self._changed.setIfChanged(False)
        self._nameUpdate()

    def _textChanged(self):
        if self._changed.setIfChanged(True):
            self._nameUpdate()

    def _nameUpdate(self):
        name = os.path.basename(self._path) if self._path else "Untitled"
        if self._changed.get():
            name += "*"
        self._name.setIfChanged(name)
        self._tooltip.setIfChanged(self._path if self._path else "Untitled")


class SettingsModel:
    """
    The settings that persist between runs.

    The values live in observables so widgets update when the settings
    tool changes them; the file itself is written once, when the
    application exits cleanly. The settings object comes from the
    application: passing an AppFiles to ftk.App is what creates it,
    along with the log file and the -settingsFile and -logFile options.
    """
    def __init__(self, app):
        self._settings = app.settings

        # The getters return (found, value); _get folds in the default.
        def _get(getter, key, default):
            found, value = getter(key)
            return value if found else default

        self.recentFiles = _get(self._settings.getStringList, "/RecentFiles", [])
        self.windowSplit = _get(self._settings.getDouble, "/Window/Split", .7)
        self.settingsVisible = ftk.ObservableBool(
            _get(self._settings.getBool, "/Window/Settings", False))

        # Nested struct properties come back as copies, so a field is
        # changed by taking the struct out, changing it, and putting it
        # back.
        options = ftk.TextEditOptions()
        fontInfo = options.fontInfo
        fontInfo.name = _get(self._settings.getString, "/TextEdit/Font", fontInfo.name)
        fontInfo.size = _get(self._settings.getInt, "/TextEdit/FontSize", fontInfo.size)
        options.fontInfo = fontInfo
        # Options types without observable bindings use plain Python
        # callbacks; they serve the same purpose for a type used in one
        # place.
        self._textEditOptions = options
        self._textEditOptionsCallbacks = []

        modelOptions = ftk.TextEditModelOptions()
        modelOptions.tabSpaces = _get(
            self._settings.getInt, "/TextEdit/TabSpaces", modelOptions.tabSpaces)
        self._textEditModelOptions = modelOptions
        self._textEditModelOptionsCallbacks = []

        self.colorStyle = _get(self._settings.getString, "/Style/ColorStyle", "Dark")
        self.displayScale = _get(
            self._settings.getDouble, "/Style/DisplayScale", app.defaultDisplayScale)

    def getTextEditOptions(self):
        return self._textEditOptions

    def setTextEditOptions(self, value):
        self._textEditOptions = value
        for callback in self._textEditOptionsCallbacks:
            callback(value)

    def observeTextEditOptions(self, callback):
        self._textEditOptionsCallbacks.append(callback)

    def getTextEditModelOptions(self):
        return self._textEditModelOptions

    def setTextEditModelOptions(self, value):
        self._textEditModelOptions = value
        for callback in self._textEditModelOptionsCallbacks:
            callback(value)

    def observeTextEditModelOptions(self, callback):
        self._textEditModelOptionsCallbacks.append(callback)

    def save(self):
        self._settings.setStringList("/RecentFiles", self.recentFiles)
        self._settings.setDouble("/Window/Split", self.windowSplit)
        self._settings.setBool("/Window/Settings", self.settingsVisible.get())
        self._settings.setString("/TextEdit/Font", self._textEditOptions.fontInfo.name)
        self._settings.setInt("/TextEdit/FontSize", self._textEditOptions.fontInfo.size)
        self._settings.setInt("/TextEdit/TabSpaces", self._textEditModelOptions.tabSpaces)
        self._settings.setString("/Style/ColorStyle", self.colorStyle)
        self._settings.setDouble("/Style/DisplayScale", self.displayScale)


class Actions:
    """
    Every action, keyed "Menu/Name" the way the C++ example keys them.

    The same action objects go into the menu bar and the tool bar, so
    enabling or checking one place is enabling or checking them all.
    """
    def __init__(self, context, app, mainWindow):
        self.actions = {}
        appWeak = weakref.ref(app)
        mainWindowWeak = weakref.ref(mainWindow)

        # File actions.
        self.actions["File/New"] = ftk.Action(
            "New",
            "FileNew",
            ftk.KeyShortcut(ftk.Key.N, ftk.commandKeyModifier),
            lambda: appWeak().newDocument())
        self.actions["File/New"].tooltip = "Create a new file"
        self.actions["File/Open"] = ftk.Action(
            "Open",
            "FileOpen",
            ftk.KeyShortcut(ftk.Key.O, ftk.commandKeyModifier),
            lambda: appWeak().openDialog())
        self.actions["File/Open"].tooltip = "Open a file"
        self.actions["File/Close"] = ftk.Action(
            "Close",
            "FileClose",
            ftk.KeyShortcut(ftk.Key.E, ftk.commandKeyModifier),
            lambda: appWeak().close(appWeak().documentModel.currentIndex))
        self.actions["File/Close"].tooltip = "Close the current file"
        self.actions["File/CloseAll"] = ftk.Action(
            "Close All",
            "FileCloseAll",
            ftk.KeyShortcut(ftk.Key.E, ftk.KeyModifier.Shift, ftk.commandKeyModifier),
            lambda: appWeak().closeAll())
        self.actions["File/CloseAll"].tooltip = "Close all files"
        self.actions["File/Save"] = ftk.Action(
            "Save",
            "FileSave",
            ftk.KeyShortcut(ftk.Key.S, ftk.commandKeyModifier),
            lambda: appWeak().save())
        self.actions["File/Save"].tooltip = "Save the current file"
        self.actions["File/SaveAs"] = ftk.Action(
            "Save As",
            ftk.KeyShortcut(ftk.Key.S, ftk.KeyModifier.Shift, ftk.commandKeyModifier),
            lambda: appWeak().saveAs())
        self.actions["File/SaveAs"].tooltip = "Save the current file with a new name"
        self.actions["File/Exit"] = ftk.Action(
            "Exit",
            ftk.KeyShortcut(ftk.Key.Q, ftk.commandKeyModifier),
            lambda: appWeak().exit())

        # Edit actions, forwarded to the current document's model.
        def current():
            return appWeak().documentModel.getCurrent()
        self.actions["Edit/Undo"] = ftk.Action(
            "Undo",
            "Undo",
            ftk.KeyShortcut(ftk.Key.Z, ftk.commandKeyModifier),
            lambda: current() and current().model.undo())
        self.actions["Edit/Redo"] = ftk.Action(
            "Redo",
            "Redo",
            ftk.KeyShortcut(ftk.Key.Y, ftk.commandKeyModifier),
            lambda: current() and current().model.redo())
        self.actions["Edit/Cut"] = ftk.Action(
            "Cut",
            "Cut",
            ftk.KeyShortcut(ftk.Key.X, ftk.commandKeyModifier),
            lambda: current() and current().model.cut())
        self.actions["Edit/Copy"] = ftk.Action(
            "Copy",
            "Copy",
            ftk.KeyShortcut(ftk.Key.C, ftk.commandKeyModifier),
            lambda: current() and current().model.copy())
        self.actions["Edit/Paste"] = ftk.Action(
            "Paste",
            "Paste",
            ftk.KeyShortcut(ftk.Key.V, ftk.commandKeyModifier),
            lambda: current() and current().model.paste())
        self.actions["Edit/SelectAll"] = ftk.Action(
            "Select All",
            ftk.KeyShortcut(ftk.Key.A, ftk.commandKeyModifier),
            lambda: current() and current().model.selectAll())
        self.actions["Edit/ClearSelection"] = ftk.Action(
            "Clear Selection",
            ftk.KeyShortcut(ftk.Key.A, ftk.KeyModifier.Shift, ftk.commandKeyModifier),
            lambda: current() and current().model.clearSelection())

        # Checkable actions take the callback as a keyword: a positional
        # callable binds to the plain callback overload and the checked
        # state never arrives.
        self.actions["Edit/Settings"] = ftk.Action(
            "Settings",
            "Settings",
            checkedCallback =
                lambda value: appWeak().settingsModel.settingsVisible.setIfChanged(value))
        self.actions["Edit/Settings"].tooltip = "Toggle the settings"
        self.actions["Window/FullScreen"] = ftk.Action(
            "Full Screen",
            "WindowFullScreen",
            ftk.KeyShortcut(ftk.Key.U, ftk.commandKeyModifier),
            checkedCallback =
                lambda value: setattr(mainWindowWeak(), "fullScreen", value))
        self.actions["Window/FullScreen"].tooltip = "Toggle full screen mode"

        selfWeak = weakref.ref(self)
        self._settingsVisibleObserver = ftk.BoolObserver(
            app.settingsModel.settingsVisible,
            lambda value: setattr(
                selfWeak().actions["Edit/Settings"], "checked", value))
        self._fullScreenObserver = ftk.BoolObserver(
            mainWindow.observeFullScreen,
            lambda value: setattr(
                selfWeak().actions["Window/FullScreen"], "checked", value))

        # The enabled state of an action depends on the current document
        # and its selection, undo, and changed state; observing the
        # current document re-observes those, so any of them changing
        # funnels into one update.
        self._appWeak = appWeak
        self._currentObserver = ftk.IDocumentObserver(
            app.documentModel.observeCurrent(),
            lambda doc: selfWeak()._currentUpdate(doc))

    def _currentUpdate(self, doc):
        selfWeak = weakref.ref(self)
        if doc is not None:
            self._changedObserver = ftk.BoolObserver(
                doc.observeChanged(),
                lambda value: selfWeak()._actionsUpdate())
            self._selectionObserver = ftk.TextEditSelectionObserver(
                doc.model.observeSelection(),
                lambda value: selfWeak()._actionsUpdate())
            self._hasUndoObserver = ftk.BoolObserver(
                doc.model.observeHasUndo(),
                lambda value: selfWeak()._actionsUpdate())
            self._hasRedoObserver = ftk.BoolObserver(
                doc.model.observeHasRedo(),
                lambda value: selfWeak()._actionsUpdate())
        else:
            self._changedObserver = None
            self._selectionObserver = None
            self._hasUndoObserver = None
            self._hasRedoObserver = None
            self._actionsUpdate()

    def _actionsUpdate(self):
        doc = self._appWeak().documentModel.getCurrent()
        current = doc is not None
        selection = doc.model.selection if doc else ftk.TextEditSelection()
        self.actions["File/Close"].enabled = current
        self.actions["File/CloseAll"].enabled = current
        self.actions["File/Save"].enabled = doc.isChanged() if doc else False
        self.actions["File/SaveAs"].enabled = current
        self.actions["Edit/Undo"].enabled = current and doc.model.observeHasUndo().get()
        self.actions["Edit/Redo"].enabled = current and doc.model.observeHasRedo().get()
        self.actions["Edit/Cut"].enabled = current and selection.isValid()
        self.actions["Edit/Copy"].enabled = current and selection.isValid()
        self.actions["Edit/Paste"].enabled = current
        self.actions["Edit/SelectAll"].enabled = current
        self.actions["Edit/ClearSelection"].enabled = current and selection.isValid()


class MenuBar(ftk.MenuBar):
    def __init__(self, context, app, actions):
        ftk.MenuBar.__init__(self, context)

        fileMenu = ftk.Menu(context)
        for key in ("File/New", "File/Open", "File/Close", "File/CloseAll",
                    "File/Save", "File/SaveAs"):
            fileMenu.addAction(actions.actions[key])
        fileMenu.addDivider()
        self._recentMenu = fileMenu.addSubMenu("Recent")
        fileMenu.addDivider()
        fileMenu.addAction(actions.actions["File/Exit"])
        self.addMenu("File", fileMenu)

        editMenu = ftk.Menu(context)
        for key in ("Edit/Undo", "Edit/Redo"):
            editMenu.addAction(actions.actions[key])
        editMenu.addDivider()
        for key in ("Edit/Cut", "Edit/Copy", "Edit/Paste"):
            editMenu.addAction(actions.actions[key])
        editMenu.addDivider()
        for key in ("Edit/SelectAll", "Edit/ClearSelection"):
            editMenu.addAction(actions.actions[key])
        editMenu.addDivider()
        editMenu.addAction(actions.actions["Edit/Settings"])
        self.addMenu("Edit", editMenu)

        windowMenu = ftk.Menu(context)
        windowMenu.addAction(actions.actions["Window/FullScreen"])
        self.addMenu("Window", windowMenu)

        # Rebuild the recent files menu whenever the model changes,
        # newest first.
        appWeak = weakref.ref(app)
        selfWeak = weakref.ref(self)
        def recentUpdate(paths):
            menu = selfWeak()._recentMenu
            menu.clear()
            for path in reversed([p.get() for p in paths]):
                menu.addAction(ftk.Action(
                    os.path.basename(path),
                    lambda path = path: appWeak().open(path)))
        self._recentObserver = ftk.PathListObserver(
            app.recentFilesModel.observeRecent,
            recentUpdate)


class ToolBar(ftk.IContainer):
    def __init__(self, context, actions, parent = None):
        ftk.IContainer.__init__(self, context, "textedit.ToolBar", parent)
        layout = ftk.HorizontalLayout(context)
        layout.spacingRole = ftk.SizeRole.SpacingSmall
        self._setWidget(layout)
        for keys in (
            ("File/New", "File/Open", "File/Close", "File/CloseAll", "File/Save"),
            ("Edit/Undo", "Edit/Redo", "Edit/Cut", "Edit/Copy", "Edit/Paste"),
            ("Window/FullScreen",)):
            toolBar = ftk.ToolBar(context, ftk.Orientation.Horizontal, layout)
            for key in keys:
                toolBar.addAction(actions.actions[key])
            if keys[0] != "Window/FullScreen":
                ftk.Divider(context, ftk.Orientation.Horizontal, layout)


class DocumentTabs(ftk.IContainer):
    """
    A tab per document, holding a text editor on the document's model.

    The tabs widget only mirrors the document model: adding, closing,
    and choosing documents all go through the model, and this observes
    the results. That is what keeps a second view -- the window title,
    the status bar -- in agreement for free.
    """
    def __init__(self, context, app, parent = None):
        ftk.IContainer.__init__(self, context, "textedit.DocumentTabs", parent)

        self._tabWidget = ftk.TabWidget(context)
        self._tabWidget.closable = True
        self._setWidget(self._tabWidget)
        self._textEdits = {}
        self._nameObservers = {}

        appWeak = weakref.ref(app)
        self._appWeak = appWeak
        self._tabWidget.setCallback(
            lambda index: setattr(appWeak().documentModel, "currentIndex", index))
        self._tabWidget.setCloseCallback(
            lambda index: appWeak().close(index))

        # The C++ example observes documents being added and closed;
        # those observables carry weak pointers, which do not cross into
        # Python, so this reconciles the tabs against the document list.
        selfWeak = weakref.ref(self)
        self._listObserver = ftk.IDocumentListObserver(
            app.documentModel.observeList(),
            lambda docs: selfWeak()._reconcile(docs))
        self._currentObserver = ftk.IntObserver(
            app.documentModel.observeCurrentIndex(),
            lambda index: setattr(selfWeak()._tabWidget, "current", index))
        app.settingsModel.observeTextEditOptions(
            weak(self._optionsUpdate))
        app.settingsModel.observeTextEditModelOptions(
            weak(self._modelOptionsUpdate))

    def _reconcile(self, docs):
        app = self._appWeak()
        for doc in [d for d in self._textEdits if d not in docs]:
            self._tabWidget.removeTab(self._textEdits.pop(doc))
            self._nameObservers.pop(doc, None)
        selfWeak = weakref.ref(self)
        for doc in docs:
            if doc in self._textEdits:
                continue
            textEdit = ftk.TextEdit(self.context, doc.model)
            textEdit.options = app.settingsModel.getTextEditOptions()
            doc.model.options = app.settingsModel.getTextEditModelOptions()
            textEdit.marginRole = ftk.SizeRole.MarginSmall
            self._textEdits[doc] = textEdit
            self._tabWidget.addTab("", textEdit)
            textEdit.takeKeyFocus()
            self._nameObservers[doc] = ftk.StringObserver(
                doc.observeName(),
                lambda value, textEdit = textEdit:
                    selfWeak()._tabWidget.setTabText(textEdit, value))

    def _optionsUpdate(self, value):
        for textEdit in self._textEdits.values():
            textEdit.options = value

    def _modelOptionsUpdate(self, value):
        for doc in self._textEdits:
            doc.model.options = value


class StatusBar(ftk.IContainer):
    def __init__(self, context, app, parent = None):
        ftk.IContainer.__init__(self, context, "textedit.StatusBar", parent)

        self._label = ftk.Label(context)
        layout = ftk.HorizontalLayout(context)
        layout.marginRole = ftk.SizeRole.MarginInside
        layout.addSpacer(ftk.Stretch.Expanding)
        self._label.parent = layout
        self._setWidget(layout)

        self._line = 0
        self._column = 0
        self._lineCount = 0

        selfWeak = weakref.ref(self)
        self._currentObserver = ftk.IDocumentObserver(
            app.documentModel.observeCurrent(),
            lambda doc: selfWeak()._currentUpdate(doc))

    def _currentUpdate(self, doc):
        selfWeak = weakref.ref(self)
        if doc is not None:
            self._textObserver = ftk.StringListObserver(
                doc.model.observeText(),
                lambda lines: selfWeak()._set(lineCount = len(lines)))
            self._cursorObserver = ftk.TextEditPosObserver(
                doc.model.observeCursor(),
                lambda pos: selfWeak()._set(line = pos.line + 1, column = pos.chr + 1))
        else:
            self._textObserver = None
            self._cursorObserver = None
            self._set(line = 0, column = 0, lineCount = 0)

    def _set(self, line = None, column = None, lineCount = None):
        if line is not None:
            self._line = line
        if column is not None:
            self._column = column
        if lineCount is not None:
            self._lineCount = lineCount
        self._label.text = "Line: {0} Column: {1} Line count: {2}".format(
            self._line, self._column, self._lineCount)


class SettingsWidget(ftk.IContainer):
    def __init__(self, context, app, parent = None):
        ftk.IContainer.__init__(self, context, "textedit.SettingsWidget", parent)

        settingsModel = app.settingsModel

        titleLabel = ftk.Label(context, "Settings")
        titleLabel.marginRole = ftk.SizeRole.MarginSmall
        titleLabel.hStretch = ftk.Stretch.Expanding
        closeButton = ftk.ToolButton(context)
        closeButton.icon = "Close"
        closeButton.setClickedCallback(
            lambda: settingsModel.settingsVisible.setIfChanged(False))

        fontSystem = context.getSystemByName("ftk::FontSystem")
        fonts = fontSystem.fonts
        self._fontComboBox = ftk.ComboBox(context, fonts)
        self._fontComboBox.hStretch = ftk.Stretch.Expanding
        options = settingsModel.getTextEditOptions()
        if options.fontInfo.name in fonts:
            self._fontComboBox.currentIndex = fonts.index(options.fontInfo.name)
        self._fontSizeEdit = ftk.IntEdit(context)
        self._fontSizeEdit.range = ftk.RangeI(6, 64)
        self._fontSizeEdit.value = options.fontInfo.size
        self._tabSpacesEdit = ftk.IntEdit(context)
        self._tabSpacesEdit.range = ftk.RangeI(1, 8)
        self._tabSpacesEdit.value = settingsModel.getTextEditModelOptions().tabSpaces

        layout = ftk.VerticalLayout(context)
        layout.spacingRole = ftk.SizeRole._None
        hLayout = ftk.HorizontalLayout(context, layout)
        hLayout.spacingRole = ftk.SizeRole._None
        hLayout.backgroundRole = ftk.ColorRole.Header
        titleLabel.parent = hLayout
        closeButton.parent = hLayout
        formLayout = ftk.FormLayout(context, layout)
        formLayout.marginRole = ftk.SizeRole.Margin
        fontLayout = ftk.HorizontalLayout(context)
        fontLayout.spacingRole = ftk.SizeRole.SpacingSmall
        self._fontComboBox.parent = fontLayout
        self._fontSizeEdit.parent = fontLayout
        formLayout.addRow("Font:", fontLayout)
        formLayout.addRow("Tab spaces:", self._tabSpacesEdit)
        self._setWidget(layout)

        def setFont(name = None, size = None):
            options = settingsModel.getTextEditOptions()
            fontInfo = options.fontInfo
            if name is not None:
                fontInfo.name = name
            if size is not None:
                fontInfo.size = size
            options.fontInfo = fontInfo
            settingsModel.setTextEditOptions(options)
        self._fontComboBox.setIndexCallback(
            lambda index: setFont(name = fonts[index]))
        self._fontSizeEdit.setCallback(
            lambda value: setFont(size = value))
        def setTabSpaces(value):
            options = settingsModel.getTextEditModelOptions()
            options.tabSpaces = value
            settingsModel.setTextEditModelOptions(options)
        self._tabSpacesEdit.setCallback(setTabSpaces)


class MainWindow(ftk.MainWindow):
    def __init__(self, context, app, size):
        ftk.MainWindow.__init__(self, context, app, size)
        self._app = weakref.ref(app)

        self._actions = Actions(context, app, self)
        self._menuBar = MenuBar(context, app, self._actions)
        self.menuBar = self._menuBar
        self._tabs = DocumentTabs(context, app)
        self._settingsWidget = SettingsWidget(context, app)
        self._toolBar = ToolBar(context, self._actions)
        self._statusBar = StatusBar(context, app)

        layout = ftk.VerticalLayout(context)
        layout.spacingRole = ftk.SizeRole._None
        layout.setStretch(ftk.Stretch.Expanding)
        self._toolBar.parent = layout
        ftk.Divider(context, ftk.Orientation.Vertical, layout)
        self._splitter = ftk.Splitter(context, ftk.Orientation.Horizontal, layout)
        self._splitter.split = app.settingsModel.windowSplit
        self._tabs.parent = self._splitter
        self._settingsWidget.parent = self._splitter
        ftk.Divider(context, ftk.Orientation.Vertical, layout)
        self._statusBar.parent = layout
        self.widget = layout

        selfWeak = weakref.ref(self)
        self._settingsVisibleObserver = ftk.BoolObserver(
            app.settingsModel.settingsVisible,
            lambda value: selfWeak()._settingsWidget.setVisible(value))

    def saveSettings(self):
        self._app().settingsModel.windowSplit = self._splitter.split

    def dropEvent(self, event):
        # Files dragged from elsewhere arrive as text.
        event.accept = True
        if isinstance(event.data, ftk.DragDropTextData):
            for path in event.data.text:
                self._app().open(path)


class App(ftk.App):
    """
    The application: it owns the models and the window, and the file
    operations that need error dialogs live here so every caller gets
    them.
    """
    def __init__(self, context, argv):
        self._pathsArg = ftk.CmdLineListArgString("inputs", "Input paths.", True)
        # The AppFiles argument is what gives the application settings
        # and a log file, along with -settingsFile, -logFile, and
        # -resetSettings.
        ftk.App.__init__(
            self, context, argv,
            "textedit-python", "Python text edit example",
            [self._pathsArg], [],
            ftk.AppFiles("feather-tk", "textedit-python", 1))

    def run(self):
        self.settingsModel = SettingsModel(self)
        self.documentModel = ftk.DocumentModel(self.context)
        self.recentFilesModel = ftk.RecentFilesModel(self.context)
        self.recentFilesModel.recent = \
            [ftk.Path(p) for p in self.settingsModel.recentFiles]

        fileBrowserSystem = self.context.getSystemByName("ftk::FileBrowserSystem")
        fileBrowserSystem.nativeFileDialog = False
        fileBrowserSystem.recentFilesModel = self.recentFilesModel

        self._window = MainWindow(self.context, self, ftk.Size2I(1280, 960))

        self.colorStyle = getattr(
            ftk.ColorStyle, self.settingsModel.colorStyle, ftk.ColorStyle.Dark)
        self.displayScale = self.settingsModel.displayScale

        paths = self._pathsArg.list
        if paths:
            self.open(paths)
        else:
            self.documentModel.add(Document(self.context))

        self._window.show()
        super().run()

        # Written after the run loop instead of from destructors:
        # Python's interpreter shutdown order makes destructor-time
        # saves unreliable, so a clean quit walks the state explicitly.
        self._window.saveSettings()
        self.settingsModel.recentFiles = list(self.recentFilesModel.recent)
        self.settingsModel.save()

    def newDocument(self):
        self.documentModel.add(Document(self.context))

    def open(self, paths):
        if isinstance(paths, str):
            paths = [paths]
        errors = []
        for path in paths:
            try:
                self.documentModel.add(Document(self.context, path))
                self.recentFilesModel.addRecent(ftk.Path(path))
            except Exception as e:
                errors.append(str(e))
        if errors:
            self._dialogs().message("ERROR", "\n".join(errors), self._window)

    def openDialog(self):
        selfWeak = weakref.ref(self)
        fileBrowserSystem = self.context.getSystemByName("ftk::FileBrowserSystem")
        fileBrowserSystem.open(
            self._window,
            lambda path: selfWeak().open(path.get()))

    def close(self, index):
        docs = self.documentModel.getList()
        if 0 <= index < len(docs):
            doc = docs[index]
            if doc.isChanged():
                selfWeak = weakref.ref(self)
                self._dialogs().confirm(
                    "Unsaved Changes",
                    "File has unsaved changes, are you sure you want to close it?",
                    self._window,
                    lambda value: value and selfWeak().documentModel.close(index),
                    "Yes",
                    "No")
            else:
                self.documentModel.close(index)

    def closeAll(self):
        changed = any(doc.isChanged() for doc in self.documentModel.getList())
        if changed:
            selfWeak = weakref.ref(self)
            self._dialogs().confirm(
                "Unsaved Changes",
                "Files have unsaved changes, are you sure you want to close them?",
                self._window,
                lambda value: value and selfWeak().documentModel.closeAll(),
                "Yes",
                "No")
        else:
            self.documentModel.closeAll()

    def save(self):
        doc = self.documentModel.getCurrent()
        if doc is None:
            return
        if doc.path is None:
            self.saveAs()
        else:
            try:
                doc.save()
            except Exception as e:
                self._dialogs().message("ERROR", str(e), self._window)

    def saveAs(self):
        doc = self.documentModel.getCurrent()
        if doc is None:
            return
        selfWeak = weakref.ref(self)
        fileBrowserSystem = self.context.getSystemByName("ftk::FileBrowserSystem")
        options = ftk.FileBrowserOpenOptions()
        options.title = "Save As"
        options.mode = ftk.FileBrowserMode.Save
        def callback(path, doc = doc):
            try:
                doc.save(path.get())
            except Exception as e:
                selfWeak()._dialogs().message("ERROR", str(e), selfWeak()._window)
        fileBrowserSystem.open(self._window, callback, options)

    def exit(self):
        changed = any(doc.isChanged() for doc in self.documentModel.getList())
        if changed:
            selfWeak = weakref.ref(self)
            self._dialogs().confirm(
                "Unsaved Changes",
                "Files have unsaved changes, are you sure you want to exit?",
                self._window,
                lambda value: value and ftk.App.exit(selfWeak()),
                "Yes",
                "No")
        else:
            ftk.App.exit(self)

    def _dialogs(self):
        return self.context.getSystemByName("ftk::DialogSystem")


if __name__ == "__main__":
    context = ftk.Context()
    app = App(context, sys.argv)
    if app.hasCmdLineHelp:
        sys.exit(0)
    app.run()
    app = None
