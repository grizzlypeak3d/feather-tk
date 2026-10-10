// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FileBrowserPrivate.h>

#include <ftk/UI/DrawUtil.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>
#include <ftk/Core/RenderUtil.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <filesystem>
#include <map>
#include <optional>

namespace ftk
{
    namespace
    {
        const float doubleClickTime = .5F;

        struct FileBrowserItem
        {
            std::shared_ptr<Image> icon;

            // Whether a thumbnail is wanted for this row. Directories, and
            // files the application cannot read, keep their icon.
            bool thumbnail = false;
            std::shared_ptr<Image> thumbnailImage;
            FileBrowserThumbnailRequest thumbnailRequest;

            // What the row gives its image, which is the thumbnail's own size
            // once one has arrived. Kept when the image is let go of, so that
            // scrolling back does not move the rows around.
            Size2I imageSize;

            std::vector<std::string> text;
            std::vector<Size2I> textSizes;

            // A second line under the name: the directory of a file that
            // is listed apart from it. It does not widen the row; it is
            // cut off where the row ends.
            std::string detail;
            Size2I detailSize;

            // The name as a tile shows it, cut down to the tile's width.
            std::string tileText;
            Size2I tileTextSize;

            Size2I size;

            // Where the item is, from the view's origin: a row of the list
            // or a tile of the grid. Everything that asks where an item is
            // -- drawing, the mouse, scrolling to it -- asks this.
            Box2I rect;
        };

        // As large as the image fits in the box. Thumbnails arrive at the
        // height they were asked for but at whatever width the file's aspect
        // gives, which is wider than the box for a scope image and narrower
        // for a portrait one.
        Size2I fitSize(const std::shared_ptr<Image>& image, const Size2I& box)
        {
            const Size2I& size = image->getSize();
            const Size2I imageSize(
                size.w * image->getInfo().pixelAspectRatio,
                size.h);
            if (imageSize.w <= 0 || imageSize.h <= 0)
                return box;
            const float scale = std::min(
                box.w / static_cast<float>(imageSize.w),
                box.h / static_cast<float>(imageSize.h));
            return Size2I(imageSize.w * scale, imageSize.h * scale);
        }

        // What a row gives its image. A row expecting a thumbnail holds the
        // largest one open until it arrives, so that the rows do not move
        // under the pointer as the images land; anything else is as big as
        // its icon and no bigger.
        Size2I itemImageSize(const FileBrowserItem& item, const Size2I& thumbnailBox)
        {
            Size2I out;
            if (item.thumbnailImage)
            {
                out = fitSize(item.thumbnailImage, thumbnailBox);
            }
            else if (item.thumbnail)
            {
                out = thumbnailBox;
            }
            else if (item.icon)
            {
                out = item.icon->getSize();
            }
            return out;
        }
    }

    struct FileBrowserView::Private
    {
        FileBrowserMode mode = FileBrowserMode::Open;
        std::shared_ptr<FileBrowserModel> model;
        FileBrowserOptions options;
        std::string search;
        std::vector<DirEntry> dirEntries;

        // The files shown in place of a directory, and what the file
        // system said about each. Asked once, when the list is set: the
        // list is rebuilt on every search key, and a file on a volume
        // that has gone away is slow to say so.
        bool hasPaths = false;
        std::vector<Path> paths;
        std::vector<DirEntry> pathEntries;

        std::shared_ptr<Observable<int> > current;
        std::shared_ptr<Observable<size_t> > itemCount;

        // Which items are selected, and where a range is measured from. The
        // current item is where the keyboard is rather than what is chosen:
        // with one selected they are the same, with several they are not.
        bool multiple = false;
        std::set<int> selection;
        int anchor = -1;

        std::vector<FileBrowserItem> items;
        std::function<void(const std::vector<Path>&)> callback;
        std::function<void(const std::vector<Path>&)> selectCallback;
        std::function<void(bool)> keyFocusCallback;

        std::shared_ptr<Observer<std::filesystem::path> > pathObserver;
        std::shared_ptr<Observer<FileBrowserOptions> > optionsObserver;
        std::shared_ptr<Observer<std::string> > extObserver;
        std::shared_ptr<Observer<std::string> > extGroupObserver;

        float iconScale = 1.F;
        std::shared_ptr<Image> directoryImage;
        std::shared_ptr<Image> fileImage;

        std::shared_ptr<IFileBrowserThumbnails> thumbnails;

        // The rows with a request outstanding: what the tick collects, and
        // what scrolling out of view cancels.
        std::set<int> thumbnailRequests;

        // The files that claimed a format but had no image in them. Learned
        // per request, and remembered across the list rebuilding -- asking
        // again on every search key holds a thumbnail's width open for a
        // row that is never getting one, and the icons shuffle sideways
        // while the answer comes back.
        std::set<std::string> noThumbnail;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int keyFocus = 0;
            int pad = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I thumbnail;
            int imageColumn = 0;
            Size2I sizeHint;

            // The tiles: the room each asks for, how many fit across the
            // width the view was given, and how tall that makes the grid.
            // The height is only known once the width is, so it is found
            // when the geometry is set and the size hint follows it.
            Size2I tileImage;
            int tileText = 0;
            Size2I tile;
            int columns = 1;
            int layoutWidth = -1;
            int layoutHeight = 0;
        };
        SizeData size;

        struct MouseData
        {
            int hover = -1;
            int pressed = -1;
            int click = -1;
            std::chrono::steady_clock::time_point clickTime;

            // The selection before the press, which selects: a press that
            // is cancelled puts it back.
            int currentPrev = -1;
            int anchorPrev = -1;
            std::set<int> selectionPrev;
        };
        MouseData mouse;
    };

    void FileBrowserView::_init(
        const std::shared_ptr<Context>& context,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::FileBrowserView", parent);
        FTK_P();

        setAcceptsKeyFocus(true);
        setBackgroundRole(ColorRole::Base);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.mode = mode;
        p.model = model;
        p.current = Observable<int>::create(-1);
        p.itemCount = Observable<size_t>::create(0);

        p.pathObserver = Observer<std::filesystem::path>::create(
            model->observePath(),
            [this](const std::filesystem::path&)
            {
                FTK_P();
                _clearCurrent();
                // The files without images belonged to the old directory;
                // clearing here also keeps the set from growing without
                // bound as directories are visited.
                p.noThumbnail.clear();
                _directoryUpdate();
            });

        p.optionsObserver = Observer<FileBrowserOptions>::create(
            model->observeOptions(),
            [this](const FileBrowserOptions& value)
            {
                FTK_P();
                if (value.dirList != p.options.dirList)
                {
                    _clearCurrent();
                }
                if (value.thumbnails != p.options.thumbnails ||
                    value.layout != p.options.layout)
                {
                    // The thumbnails were made for the old size, so they
                    // do not carry over to the new one.
                    for (auto& item : p.items)
                    {
                        item.thumbnailImage.reset();
                    }
                }
                p.options = value;
                _directoryUpdate();
            });

        p.extObserver = Observer<std::string>::create(
            model->observeExt(),
            [this](const std::string&)
            {
                _clearCurrent();
                _directoryUpdate();
            });
        p.extGroupObserver = Observer<std::string>::create(
            model->observeExtGroup(),
            [this](const std::string&)
            {
                _clearCurrent();
                _directoryUpdate();
            });
    }

    FileBrowserView::FileBrowserView() :
        _p(new Private)
    {}

    FileBrowserView::~FileBrowserView()
    {
        _cancelThumbnails();
    }

    std::shared_ptr<FileBrowserView> FileBrowserView::create(
        const std::shared_ptr<Context>& context,
        FileBrowserMode mode,
        const std::shared_ptr<FileBrowserModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FileBrowserView>(new FileBrowserView);
        out->_init(context, mode, model, parent);
        return out;
    }

    void FileBrowserView::reload()
    {
        FTK_P();
        // Reload is the refresh gesture, so the thumbnails are re-read
        // along with the directory.
        for (auto& item : p.items)
        {
            item.thumbnailImage.reset();
        }
        p.noThumbnail.clear();
        _directoryUpdate();
    }

    bool FileBrowserView::hasPaths() const
    {
        return _p->hasPaths;
    }

    const std::vector<Path>& FileBrowserView::getPaths() const
    {
        return _p->paths;
    }

    void FileBrowserView::setPaths(const std::vector<Path>& value)
    {
        FTK_P();
        if (p.hasPaths && value == p.paths)
            return;
        p.hasPaths = true;
        p.paths = value;
        p.pathEntries.clear();
        for (const auto& path : p.paths)
        {
            DirEntry entry;
            entry.path = path;
            // A sequence is measured by its first frame, which is the
            // file that is there.
            const std::filesystem::path file = toFileSystem(
                path.isSeq() && path.getFrames().has_value() ?
                    path.getFrame(path.getFrames()->min(), true) :
                    path.get());
            std::error_code ec;
            entry.isDir = std::filesystem::is_directory(file, ec);
            entry.size = std::filesystem::file_size(file, ec);
            if (ec)
            {
                entry.size = 0;
            }
            entry.time = std::filesystem::last_write_time(file, ec);
            p.pathEntries.push_back(entry);
        }
        _clearCurrent();
        p.noThumbnail.clear();
        _directoryUpdate();
    }

    void FileBrowserView::clearPaths()
    {
        FTK_P();
        if (!p.hasPaths)
            return;
        p.hasPaths = false;
        p.paths.clear();
        p.pathEntries.clear();
        _clearCurrent();
        p.noThumbnail.clear();
        _directoryUpdate();
    }

    void FileBrowserView::setCallback(
        const std::function<void(const std::vector<Path>&)>& value)
    {
        _p->callback = value;
    }

    void FileBrowserView::setSelectCallback(
        const std::function<void(const std::vector<Path>&)>& value)
    {
        _p->selectCallback = value;
    }

    bool FileBrowserView::isMultiple() const
    {
        return _p->multiple;
    }

    void FileBrowserView::setMultiple(bool value)
    {
        FTK_P();
        if (value == p.multiple)
            return;
        p.multiple = value;

        // Back to the one the keyboard is on: a selection made while several
        // were allowed cannot be handed to something that takes one.
        if (!p.multiple && p.selection.size() > 1)
        {
            _setCurrent(p.current->get());
        }
    }

    std::vector<Path> FileBrowserView::getSelection() const
    {
        FTK_P();
        std::vector<Path> out;
        for (int i : p.selection)
        {
            if (i >= 0 && i < static_cast<int>(p.dirEntries.size()))
            {
                out.push_back(p.dirEntries[i].path);
            }
        }
        return out;
    }

    const std::string& FileBrowserView::getSearch() const
    {
        return _p->search;
    }

    void FileBrowserView::setSearch(const std::string& value)
    {
        FTK_P();
        if (value == p.search)
            return;
        p.search = value;
        _clearCurrent();
        _directoryUpdate();
    }

    std::shared_ptr<IObservable<int> > FileBrowserView::observeCurrent() const
    {
        return _p->current;
    }

    void FileBrowserView::setCurrent(int value)
    {
        _setCurrent(value);
    }

    std::shared_ptr<IObservable<size_t> > FileBrowserView::observeItemCount() const
    {
        return _p->itemCount;
    }

    Box2I FileBrowserView::getRect(int index) const
    {
        FTK_P();
        return index >= 0 && index < static_cast<int>(p.items.size()) ?
            p.items[index].rect :
            Box2I(0, 0, getGeometry().w(), 0);
    }

    Size2I FileBrowserView::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void FileBrowserView::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (value.w() != p.size.layoutWidth)
        {
            _layoutUpdate();
            if (_isTiles() && p.size.layoutHeight != p.size.sizeHint.h)
            {
                // The height of the grid only becomes knowable here, where
                // the width is. Ask to be measured again with it; the
                // second pass agrees, because the width does not depend on
                // the height.
                p.size.sizeHint.h = p.size.layoutHeight;
                setSizeUpdate();
            }
            setDrawUpdate();
        }
    }

    void FileBrowserView::styleEvent(const StyleEvent& event)
    {
        IMouseWidget::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void FileBrowserView::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IMouseWidget::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();

        // Collect the thumbnails that have arrived. Only the rows in view
        // have a request outstanding, so this does not walk a directory of
        // thousands on every tick.
        bool sizeUpdate = false;
        bool drawUpdate = false;
        auto i = p.thumbnailRequests.begin();
        while (i != p.thumbnailRequests.end())
        {
            if (*i < 0 || *i >= static_cast<int>(p.items.size()))
            {
                i = p.thumbnailRequests.erase(i);
                continue;
            }
            auto& item = p.items[*i];
            if (item.thumbnailRequest.future.valid() &&
                item.thumbnailRequest.future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                item.thumbnailImage = item.thumbnailRequest.future.get();
                item.thumbnailRequest = FileBrowserThumbnailRequest();
                if (!item.thumbnailImage)
                {
                    // The file claimed a format but had no image in it. Give
                    // the row back to its icon rather than leaving the room
                    // for a thumbnail that is not coming, and stop asking.
                    item.thumbnail = false;
                    if (*i < static_cast<int>(p.dirEntries.size()))
                    {
                        p.noThumbnail.insert(p.dirEntries[*i].path.get());
                    }
                }
                item.imageSize = itemImageSize(item, p.size.thumbnail);
                i = p.thumbnailRequests.erase(i);
                sizeUpdate = true;
                drawUpdate = true;
            }
            else
            {
                ++i;
            }
        }
        if (sizeUpdate)
        {
            setSizeUpdate();
        }
        if (drawUpdate)
        {
            setDrawUpdate();
        }
    }

    void FileBrowserView::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();

        if (event.displayScale != p.iconScale)
        {
            p.iconScale = event.displayScale;
            p.directoryImage.reset();
            p.fileImage.reset();

            // The thumbnails were made for the old scale; the next draw asks
            // for them again at the new one.
            _cancelThumbnails();
            for (auto& item : p.items)
            {
                item.thumbnailImage.reset();
            }
            p.size.init = true;
        }
        if (!p.directoryImage)
        {
            p.directoryImage = event.iconSystem->get("Directory", event.displayScale);
        }
        if (!p.fileImage)
        {
            p.fileImage = event.iconSystem->get("File", event.displayScale);
        }

        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.thumbnail = Size2I();
            if (p.thumbnails)
            {
                // The largest a thumbnail is allowed to be. A row is only as
                // wide as its own image, but a panorama is held to this
                // rather than given the width its aspect asks for.
                // Twice the size as tiles, where the picture is what the
                // file is found by.
                const int h = getThumbnailHeight(
                    p.options.thumbnails,
                    event.style->getSizeRole(SizeRole::Thumbnail, event.displayScale)) *
                    (_isTiles() ? 2 : 1);
                p.size.thumbnail = Size2I(h * 16 / 9, h);
            }

            // A tile has room for a thumbnail, or for an icon when there
            // are none, and for a line of the name under it.
            p.size.tileImage = p.size.thumbnail;
            if (p.fileImage &&
                (!p.thumbnails || FileBrowserThumbnails::Off == p.options.thumbnails))
            {
                p.size.tileImage = p.fileImage->getSize();
            }
            p.size.tileText = std::max(
                p.size.tileImage.w,
                p.size.fontMetrics.lineHeight * 8);
            p.size.tile = Size2I(
                p.size.tileText + (p.size.pad + p.size.margin + p.size.keyFocus) * 2,
                p.size.tileImage.h + p.size.fontMetrics.lineHeight +
                    p.size.margin * 3 + p.size.keyFocus * 2);
            for (size_t i = 0; i < p.dirEntries.size() && i < p.items.size(); ++i)
            {
                auto& item = p.items[i];
                item.icon = p.dirEntries[i].isDir ? p.directoryImage : p.fileImage;
                item.imageSize = itemImageSize(item, p.size.thumbnail);
                item.textSizes.clear();
                for (const auto& text : item.text)
                {
                    item.textSizes.push_back(
                        event.fontSystem->getSize(text, p.size.fontInfo));
                }
                item.detailSize = !item.detail.empty() ?
                    event.fontSystem->getSize(item.detail, p.size.fontInfo) :
                    Size2I();

                // The name of a tile is cut from the middle to the width
                // of the tile, keeping the start and the frame number or
                // extension at the end.
                item.tileText.clear();
                item.tileTextSize = Size2I();
                if (_isTiles() && !item.text.empty())
                {
                    item.tileText = item.text.front();
                    item.tileTextSize = !item.textSizes.empty() ?
                        item.textSizes.front() :
                        Size2I();
                    size_t low = 1;
                    size_t high = item.tileText.size();
                    if (item.tileTextSize.w > p.size.tileText)
                    {
                        while (low < high)
                        {
                            const size_t mid = low + (high - low + 1) / 2;
                            if (event.fontSystem->getSize(
                                    elide(item.text.front(), mid, ElideMode::Middle),
                                    p.size.fontInfo).w <= p.size.tileText)
                            {
                                low = mid;
                            }
                            else
                            {
                                high = mid - 1;
                            }
                        }
                        item.tileText = elide(item.text.front(), low, ElideMode::Middle);
                        item.tileTextSize = event.fontSystem->getSize(
                            item.tileText,
                            p.size.fontInfo);
                    }
                }
            }
        }

        // The images share a column as wide as the widest of them, so that
        // the names start in the same place down the directory. The rows are
        // still only as tall as what is in them; it is ragged text that is
        // hard to read, not a little space before it.
        p.size.imageColumn = 0;
        for (const auto& item : p.items)
        {
            p.size.imageColumn = std::max(p.size.imageColumn, item.imageSize.w);
        }

        // The rows are measured apart from the text, which does not change
        // when a thumbnail arrives: a directory of thousands is not worth
        // measuring again for every image that lands.
        p.size.sizeHint = Size2I();
        for (auto& item : p.items)
        {
            item.size = Size2I(p.size.imageColumn, item.imageSize.h);
            for (const auto& textSize : item.textSizes)
            {
                item.size.w += textSize.w + p.size.pad * 2 + p.size.margin * 2 + p.size.keyFocus * 2;
                item.size.h = std::max(
                    item.size.h,
                    std::max(
                        textSize.h + item.detailSize.h + p.size.margin * 2,
                        item.imageSize.h) + p.size.keyFocus * 2);
            }
            p.size.sizeHint.w = std::max(p.size.sizeHint.w, item.size.w);
            p.size.sizeHint.h += item.size.h;
        }

        _layoutUpdate();
        if (_isTiles())
        {
            // As wide as one tile and no wider, so that the scroll area
            // gives the view its own width and the tiles wrap to it.
            p.size.sizeHint = Size2I(p.size.tile.w, p.size.layoutHeight);
        }
    }

    void FileBrowserView::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();
        const Box2I& g = getGeometry();

        // Draw the selection. Tinted rather than outlined, so that several
        // selected items read as a block.
        const Color4F selectionColor = checkedTint(
            event.style->getColorRole(ColorRole::Checked));
        for (int i : p.selection)
        {
            event.render->drawRect(
                move(getRect(i), g.min),
                selectionColor);
        }

        // Draw the current state. Over the selection: with several selected
        // this is the one the keyboard is on, which the fill cannot say --
        // and only while the keyboard is in use, like the other lists. A
        // clicked item is the selection, which the fill already says; a
        // dimmed ring beside it read as a stale outline.
        if (p.current->get() != -1 && showKeyFocus())
        {
            const Box2I g2 = move(getRect(p.current->get()), g.min);
            event.render->drawMesh(
                border(g2, p.size.keyFocus),
                event.style->getColorRole(ColorRole::KeyFocus));
        }

        // Draw the mouse hover.
        if (p.mouse.hover != -1)
        {
            const Box2I g2 = move(getRect(p.mouse.hover), g.min);
            event.render->drawRect(
                g2,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the items, asking for the thumbnails along the way. Rows half
        // a screen beyond the view are asked for so that scrolling has them
        // ready rather than blank, and rows several screens away are let go
        // of: a directory of thousands would otherwise finish with a
        // thumbnail held for every one of them.
        const Box2I requestRect = margin(drawRect, 0, drawRect.h() / 2);
        const Box2I retainRect = margin(drawRect, 0, drawRect.h() * 4);
        std::set<int> thumbnailRequests;
        const bool tiles = _isTiles();
        for (size_t i = 0; i < p.items.size(); ++i)
        {
            auto& item = p.items[i];
            const Box2I rowRect = move(item.rect, g.min);
            const int y = rowRect.min.y;
            int x = g.min.x + p.size.pad;
            const Box2I g2 = tiles ? rowRect : Box2I(x, y, item.size.w, item.size.h);

            if (item.thumbnail && intersects(rowRect, requestRect))
            {
                if (!item.thumbnailImage && !item.thumbnailRequest.future.valid())
                {
                    item.thumbnailRequest = p.thumbnails->request(
                        p.dirEntries[i].path,
                        p.size.thumbnail.h);
                }
                if (item.thumbnailRequest.future.valid())
                {
                    thumbnailRequests.insert(static_cast<int>(i));
                }
            }
            else if (!intersects(rowRect, retainRect))
            {
                item.thumbnailImage.reset();
            }

            if (intersects(g2, drawRect))
            {
                // A tile has its image centered over its name; a row has
                // it at the left with the text beside it.
                const int tileInset = p.size.margin + p.size.keyFocus;
                const Box2I imageRect = tiles ?
                    Box2I(
                        rowRect.min.x + rowRect.w() / 2 - item.imageSize.w / 2,
                        rowRect.min.y + tileInset +
                            p.size.tileImage.h / 2 - item.imageSize.h / 2,
                        item.imageSize.w,
                        item.imageSize.h) :
                    Box2I(
                        x,
                        y + item.size.h / 2 - item.imageSize.h / 2,
                        item.imageSize.w,
                        item.imageSize.h);
                if (item.thumbnailImage)
                {
                    // Not cached: a directory holds more thumbnails than the
                    // render's texture cache is meant to carry.
                    ImageOptions imageOptions;
                    imageOptions.cache = false;
                    event.render->drawImage(
                        item.thumbnailImage,
                        imageRect,
                        Color4F(1.F, 1.F, 1.F),
                        imageOptions);
                }
                else if (item.icon)
                {
                    // Centered in what the row gave the image, which is the
                    // icon's own size unless a thumbnail is on its way.
                    const Size2I& iconSize = item.icon->getSize();
                    event.render->drawImage(
                        item.icon,
                        Box2I(
                            imageRect.min.x + imageRect.w() / 2 - iconSize.w / 2,
                            imageRect.min.y + imageRect.h() / 2 - iconSize.h / 2,
                            iconSize.w,
                            iconSize.h),
                        event.style->getColorRole(ColorRole::Text));
                }
                if (tiles)
                {
                    if (!item.tileText.empty())
                    {
                        event.render->drawText(
                            event.fontSystem->getGlyphs(item.tileText, p.size.fontInfo),
                            p.size.fontMetrics,
                            V2I(rowRect.min.x + rowRect.w() / 2 - item.tileTextSize.w / 2,
                                rowRect.min.y + tileInset + p.size.tileImage.h + p.size.margin),
                            event.style->getColorRole(ColorRole::Text, isEnabled()));
                    }
                    continue;
                }
                x += p.size.imageColumn;
                int rightColumnsSize = 0;
                for (int j = 1; j < static_cast<int>(item.text.size()) && j < static_cast<int>(item.textSizes.size()); ++j)
                {
                    rightColumnsSize += item.textSizes[j].w + p.size.pad * 2 + p.size.margin * 2;
                }
                for (int j = 0; j < static_cast<int>(item.text.size()) && j < static_cast<int>(item.textSizes.size()); ++j)
                {
                    const auto glyphs = event.fontSystem->getGlyphs(item.text[j], p.size.fontInfo);
                    // The name moves up to make room for the line under it.
                    const int textY =
                        y + item.size.h / 2 - item.textSizes[j].h / 2 -
                        (0 == j ? item.detailSize.h / 2 : 0);
                    event.render->drawText(
                        glyphs,
                        p.size.fontMetrics,
                        V2I(x + p.size.pad + p.size.margin, textY),
                        event.style->getColorRole(ColorRole::Text, isEnabled()));
                    if (0 == j && !item.detail.empty())
                    {
                        // Dimmed, and cut off before the columns on the
                        // right.
                        const ClipRectEnabledState clipRectEnabledState(event.render);
                        const ClipRectState clipRectState(event.render);
                        event.render->setClipRectEnabled(true);
                        event.render->setClipRect(intersect(
                            Box2I(x, y, g.max.x - rightColumnsSize - x, item.size.h),
                            drawRect));
                        event.render->drawText(
                            event.fontSystem->getGlyphs(item.detail, p.size.fontInfo),
                            p.size.fontMetrics,
                            V2I(x + p.size.pad + p.size.margin, textY + item.textSizes[j].h),
                            event.style->getColorRole(ColorRole::Text, false));
                    }
                    if (0 == j)
                    {
                        x = g.max.x - rightColumnsSize;
                    }
                    else
                    {
                        x += item.textSizes[j].w + +p.size.pad * 2 + p.size.margin * 2;
                    }
                }
            }
        }

        // Cancel the rows that left the band while their request was still
        // in the queue, so that the thumbnail thread is not left reading
        // files nobody is looking at any more.
        if (p.thumbnails)
        {
            std::vector<uint64_t> cancel;
            for (int i : p.thumbnailRequests)
            {
                if (thumbnailRequests.find(i) == thumbnailRequests.end() &&
                    i >= 0 && i < static_cast<int>(p.items.size()))
                {
                    cancel.push_back(p.items[i].thumbnailRequest.id);
                    p.items[i].thumbnailRequest = FileBrowserThumbnailRequest();
                }
            }
            p.thumbnailRequests = thumbnailRequests;
            if (!cancel.empty())
            {
                p.thumbnails->cancelRequests(cancel);
            }
        }
    }

    void FileBrowserView::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        FTK_P();
        const int hover = _getItem(event.pos);
        if (hover != p.mouse.hover)
        {
            p.mouse.hover = hover;
            setDrawUpdate();
        }
    }

    void FileBrowserView::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        FTK_P();
        int hover = -1;
        if (hover != p.mouse.hover)
        {
            p.mouse.hover = hover;
            setDrawUpdate();
        }
    }

    void FileBrowserView::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        const int hover = _getItem(event.pos);
        if (hover != p.mouse.hover)
        {
            p.mouse.hover = hover;
            setDrawUpdate();
        }
    }

    void FileBrowserView::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        const int hover = _getItem(event.pos);
        if (hover != p.mouse.hover)
        {
            p.mouse.hover = hover;
            setDrawUpdate();
        }
        if (p.mouse.hover != -1)
        {
            p.mouse.currentPrev = p.current->get();
            p.mouse.anchorPrev = p.anchor;
            p.mouse.selectionPrev = p.selection;
            if (p.multiple &&
                (static_cast<int>(KeyModifier::Shift) & event.modifiers))
            {
                _selectRange(p.mouse.hover);
            }
            else if (p.multiple &&
                (static_cast<int>(commandKeyModifier) & event.modifiers))
            {
                _toggleCurrent(p.mouse.hover);
            }
            else
            {
                _setCurrent(p.mouse.hover);
            }
            p.mouse.pressed = p.mouse.hover;
            setDrawUpdate();
        }
    }

    void FileBrowserView::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        const Box2I& g = getGeometry();
        if (p.mouse.pressed != -1)
        {
            if (event.cancel)
            {
                // A touch gesture took the press: the first finger was not
                // choosing anything.
                p.anchor = p.mouse.anchorPrev;
                _selectionUpdate(
                    p.mouse.selectionPrev,
                    p.current->setIfChanged(p.mouse.currentPrev));
                p.mouse.hover = -1;
            }
            else if (contains(getRect(p.mouse.pressed), event.pos - g.min))
            {
                const auto now = std::chrono::steady_clock::now();
                const std::chrono::duration<float> diff = now - p.mouse.clickTime;
                if (p.mouse.click == p.mouse.pressed && diff.count() < doubleClickTime)
                {
                    _doubleClick(p.mouse.pressed);
                    p.mouse.click = -1;
                }
                else
                {
                    p.mouse.click = p.mouse.pressed;
                    p.mouse.clickTime = now;
                }
            }
            p.mouse.pressed = -1;
            setDrawUpdate();
        }
    }

    void FileBrowserView::keyFocusEvent(bool value)
    {
        FTK_P();
        IMouseWidget::keyFocusEvent(value);
        setDrawUpdate();
        if (p.keyFocusCallback)
        {
            p.keyFocusCallback(value);
        }
    }

    void FileBrowserView::setKeyFocusCallback(
        const std::function<void(bool)>& value)
    {
        _p->keyFocusCallback = value;
    }

    void FileBrowserView::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        // Up and down move by a row, which is one item in the list and as
        // many as there are across in the tiles; left and right move
        // along a row of tiles.
        const bool tiles = _isTiles();
        const int rowStep = tiles ? p.size.columns : 1;
        if (p.multiple &&
            static_cast<int>(KeyModifier::Shift) == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Up:
                event.accept = true;
                takeKeyFocus();
                _selectRange(p.current->get() - rowStep);
                break;
            case Key::Down:
                event.accept = true;
                takeKeyFocus();
                _selectRange(p.current->get() + rowStep);
                break;
            case Key::Left:
                if (tiles)
                {
                    event.accept = true;
                    takeKeyFocus();
                    _selectRange(p.current->get() - 1);
                }
                break;
            case Key::Right:
                if (tiles)
                {
                    event.accept = true;
                    takeKeyFocus();
                    _selectRange(p.current->get() + 1);
                }
                break;
            case Key::Home:
                event.accept = true;
                takeKeyFocus();
                _selectRange(0);
                break;
            case Key::End:
                event.accept = true;
                takeKeyFocus();
                _selectRange(static_cast<int>(p.dirEntries.size()) - 1);
                break;
            default: break;
            }
        }
        if (!event.accept && 0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Up:
                event.accept = true;
                if (!hasKeyFocus())
                {
                    takeKeyFocus();
                    _setCurrent(p.mouse.hover);
                }
                else
                {
                    // Not past the first row: stepping back a whole row
                    // from it would jump to the first tile.
                    _setCurrent(p.current->get() >= rowStep ?
                        p.current->get() - rowStep :
                        p.current->get());
                }
                break;
            case Key::Left:
                if (tiles)
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(p.current->get() - 1);
                }
                break;
            case Key::Right:
                if (tiles)
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(p.current->get() + 1);
                }
                break;
            case Key::Down:
                event.accept = true;
                if (!hasKeyFocus())
                {
                    takeKeyFocus();
                    _setCurrent(p.mouse.hover);
                }
                else
                {
                    // Not past the last row, for the same reason.
                    _setCurrent(
                        p.current->get() + rowStep < static_cast<int>(p.dirEntries.size()) ?
                        p.current->get() + rowStep :
                        (tiles ? p.current->get() : p.current->get() + 1));
                }
                break;
            case Key::Home:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(0);
                break;
            case Key::End:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(static_cast<int>(p.dirEntries.size()) - 1);
                break;
            case Key::Return:
                event.accept = true;
                takeKeyFocus();
                _doubleClick(p.current->get());
                break;
            case Key::Escape:
                // Escape is overloaded: it lets go of the list first, and
                // only closes the browser once the list is no longer holding
                // it. Closing on the first press would leave no way to put
                // the focus down.
                if (hasKeyFocus() && showKeyFocus())
                {
                    event.accept = true;
                    releaseKeyFocus();
                }
                break;
            default: break;
            }
        }
        if (!event.accept)
        {
            IMouseWidget::keyPressEvent(event);
        }
    }

    void FileBrowserView::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    int FileBrowserView::_getItem(const V2I& value) const
    {
        FTK_P();
        int out = -1;
        const V2I pos = value - getGeometry().min;
        for (size_t i = 0; i < p.items.size(); ++i)
        {
            if (contains(p.items[i].rect, pos))
            {
                out = static_cast<int>(i);
                break;
            }
        }
        return out;
    }

    bool FileBrowserView::_isTiles() const
    {
        return FileBrowserLayout::Tiles == _p->options.layout;
    }

    void FileBrowserView::_layoutUpdate()
    {
        FTK_P();
        const int w = getGeometry().w();
        p.size.layoutWidth = w;
        if (_isTiles())
        {
            // As many tiles across as fit, sharing the width between them
            // so that the grid reaches both sides.
            p.size.columns = p.size.tile.w > 0 ? std::max(1, w / p.size.tile.w) : 1;
            const int cellW = std::max(p.size.tile.w, w / p.size.columns);
            for (size_t i = 0; i < p.items.size(); ++i)
            {
                const int column = static_cast<int>(i) % p.size.columns;
                const int row = static_cast<int>(i) / p.size.columns;
                p.items[i].rect = Box2I(
                    column * cellW,
                    row * p.size.tile.h,
                    cellW,
                    p.size.tile.h);
            }
            const int rows =
                (static_cast<int>(p.items.size()) + p.size.columns - 1) /
                p.size.columns;
            p.size.layoutHeight = rows * p.size.tile.h;
        }
        else
        {
            p.size.columns = 1;
            int y = 0;
            for (auto& item : p.items)
            {
                item.rect = Box2I(0, y, w, item.size.h);
                y += item.size.h;
            }
            p.size.layoutHeight = y;
        }
    }

    void FileBrowserView::_directoryUpdate()
    {
        FTK_P();

        // The thumbnails that have already arrived move to the new rows,
        // matched by path -- typing in the search box rebuilds the list on
        // every key, and the rows that survive the filter should not flash
        // empty and load again.
        std::map<std::string, std::shared_ptr<Image> > thumbnailCarry;
        for (size_t i = 0; i < p.items.size() && i < p.dirEntries.size(); ++i)
        {
            if (p.items[i].thumbnailImage)
            {
                thumbnailCarry[p.dirEntries[i].path.get()] =
                    p.items[i].thumbnailImage;
            }
        }

        _cancelThumbnails();
        p.dirEntries.clear();
        p.items.clear();

        // Looked up here rather than held from the start: the browser can be
        // built before the application has registered its thumbnails.
        if (auto context = getContext())
        {
            if (auto system = context->getSystem<FileBrowserSystem>())
            {
                p.thumbnails = system->getThumbnails();
            }
        }

        if (p.hasPaths)
        {
            for (const auto& entry : p.pathEntries)
            {
                if (p.search.empty() ||
                    contains(entry.path.get(), p.search, CaseCompare::Insensitive))
                {
                    p.dirEntries.push_back(entry);
                }
            }
        }
        const auto& options = p.model->getOptions();
        auto dirListOptions = options.dirList;
        dirListOptions.filter = p.search;
        dirListOptions.filterFiles = FileBrowserMode::Dir == p.mode;
        const std::string& ext = p.model->getExt();
        const std::string& extGroup = p.model->getExtGroup();
        if (!ext.empty())
        {
            dirListOptions.filterExt.push_back(ext);
        }
        else if (!extGroup.empty())
        {
            for (const auto& group : p.model->getExtGroups())
            {
                if (group.label == extGroup)
                {
                    dirListOptions.filterExt = group.exts;
                    break;
                }
            }
        }
        else
        {
            // A filter set by whoever opened the browser still applies when
            // no single extension is picked; it is empty otherwise, which
            // lists everything.
            dirListOptions.filterExt = p.model->getExtsFilter();
        }
        if (!p.hasPaths)
        {
            p.dirEntries = dirList(p.model->getPath(), dirListOptions);
        }
        p.itemCount->setIfChanged(p.dirEntries.size());

        // Columns are aligned by padding them to a common width, so the frame
        // range column is only widened when a directory needs the room.
        int frameRangeWidth = 8 + 1 + 8;
        for (const auto& dirEntry : p.dirEntries)
        {
            if (dirEntry.path.isPartialSeq())
            {
                frameRangeWidth = std::max(
                    frameRangeWidth,
                    static_cast<int>(dirEntry.path.getFrameRange(true).size()));
            }
        }

        if (auto context = getContext())
        {
            for (size_t i = 0; i < p.dirEntries.size(); ++i)
            {
                const DirEntry& dirEntry = p.dirEntries[i];
                FileBrowserItem item;

                item.thumbnail =
                    p.thumbnails &&
                    FileBrowserThumbnails::Off != p.options.thumbnails &&
                    !dirEntry.isDir &&
                    p.thumbnails->isSupported(dirEntry.path) &&
                    p.noThumbnail.find(dirEntry.path.get()) ==
                        p.noThumbnail.end();
                if (item.thumbnail)
                {
                    const auto j = thumbnailCarry.find(dirEntry.path.get());
                    if (j != thumbnailCarry.end())
                    {
                        item.thumbnailImage = j->second;
                    }
                }

                // File name, and the directory of a file listed apart
                // from it.
                item.text.push_back(dirEntry.path.getFileName());
                if (p.hasPaths)
                {
                    item.detail = dirEntry.path.getDir();
                    if (item.text.back().empty())
                    {
                        // A directory given with its separator on the end
                        // has no file name: it is named by its last part,
                        // and is in what comes before that.
                        std::filesystem::path tmp = toFileSystem(dirEntry.path.get());
                        if (!tmp.has_filename())
                        {
                            tmp = tmp.parent_path();
                        }
                        item.text.back() = fromFileSystem(tmp.filename());
                        item.detail = fromFileSystem(tmp.parent_path());
                    }
                }

                // Frame range.
                if (dirEntry.path.isSeq())
                {
                    item.text.push_back(Format("{0}").
                        arg(dirEntry.path.getFrameRange(true), frameRangeWidth));
                }

                // File extension.
                item.text.push_back(!dirEntry.isDir ?
                    Format("{0}").arg(dirEntry.path.getExt(), 6).str() :
                    std::string());

                // File size.
                if (!dirEntry.isDir)
                {
                    std::string text;
                    if (dirEntry.size < megabyte)
                    {
                        text = Format("{0}KB").
                            arg(dirEntry.size / static_cast<float>(kilobyte), 2, 6);
                    }
                    else if (dirEntry.size < gigabyte)
                    {
                        text = Format("{0}MB").
                            arg(dirEntry.size / static_cast<float>(megabyte), 2, 6);
                    }
                    else
                    {
                        text = Format("{0}GB").
                            arg(dirEntry.size / static_cast<float>(gigabyte), 2, 6);
                    }
                    item.text.push_back(text);
                }

                // File last modification time.
                // \todo std::format is available in C++20.
                //text = std::format("{}", dirEntry.time);

                p.items.push_back(std::move(item));
            }
        }

        setSizeUpdate();
        setDrawUpdate();
        p.size.init = true;
    }

    void FileBrowserView::_cancelThumbnails()
    {
        FTK_P();
        if (p.thumbnails)
        {
            std::vector<uint64_t> cancel;
            for (int i : p.thumbnailRequests)
            {
                if (i >= 0 && i < static_cast<int>(p.items.size()))
                {
                    cancel.push_back(p.items[i].thumbnailRequest.id);
                    p.items[i].thumbnailRequest = FileBrowserThumbnailRequest();
                }
            }
            if (!cancel.empty())
            {
                p.thumbnails->cancelRequests(cancel);
            }
        }
        p.thumbnailRequests.clear();
    }

    void FileBrowserView::_setCurrent(int index)
    {
        FTK_P();
        const int tmp = !p.dirEntries.empty() ?
            clamp(index, 0, static_cast<int>(p.dirEntries.size()) - 1) :
            -1;
        std::set<int> selection;
        if (tmp != -1)
        {
            selection.insert(tmp);
        }
        p.anchor = tmp;
        _selectionUpdate(selection, p.current->setIfChanged(tmp));
    }

    void FileBrowserView::_toggleCurrent(int index)
    {
        FTK_P();
        if (index < 0 || index >= static_cast<int>(p.dirEntries.size()))
            return;
        std::set<int> selection = p.selection;
        const auto i = selection.find(index);
        if (i != selection.end())
        {
            selection.erase(i);
        }
        else
        {
            selection.insert(index);
        }
        p.anchor = index;
        _selectionUpdate(selection, p.current->setIfChanged(index));
    }

    void FileBrowserView::_selectRange(int index)
    {
        FTK_P();
        if (p.dirEntries.empty())
            return;
        const int tmp = clamp(index, 0, static_cast<int>(p.dirEntries.size()) - 1);

        // Measured from wherever the selection was last started: with nothing
        // to extend from, this is that first click instead.
        if (p.anchor < 0 || p.anchor >= static_cast<int>(p.dirEntries.size()))
        {
            _setCurrent(tmp);
            return;
        }
        std::set<int> selection;
        for (int i = std::min(p.anchor, tmp); i <= std::max(p.anchor, tmp); ++i)
        {
            selection.insert(i);
        }
        _selectionUpdate(selection, p.current->setIfChanged(tmp));
    }

    void FileBrowserView::_selectionUpdate(
        const std::set<int>& selection,
        bool currentChanged)
    {
        FTK_P();
        const bool selectionChanged = selection != p.selection;
        if (!selectionChanged && !currentChanged)
            return;
        p.selection = selection;
        if (selectionChanged && p.selectCallback)
        {
            p.selectCallback(getSelection());
        }
        setDrawUpdate();
    }

    void FileBrowserView::_clearCurrent()
    {
        FTK_P();
        p.anchor = -1;
        _selectionUpdate(std::set<int>(), p.current->setIfChanged(-1));
    }

    void FileBrowserView::_doubleClick(int index)
    {
        FTK_P();
        takeKeyFocus();
        if (index >= 0 && index < static_cast<int>(p.dirEntries.size()))
        {
            const DirEntry& dirEntry = p.dirEntries[index];
            switch (p.mode)
            {
            case FileBrowserMode::Open:
            case FileBrowserMode::Save:
                // A directory in a list of files is one of the things
                // listed, to be handed back like a file; in a directory it
                // is somewhere to go.
                if ((!dirEntry.isDir || p.hasPaths) && p.callback)
                {
                    // Everything selected when this is one of them, which is
                    // what opening from a selection means; on its own it is
                    // the one opened. Return comes through here as well, and
                    // that is the way several get opened at once -- the Ok
                    // button is not the only way out of the browser.
                    std::vector<Path> paths;
                    if (p.selection.size() > 1 &&
                        p.selection.find(index) != p.selection.end())
                    {
                        paths = getSelection();
                    }
                    else
                    {
                        paths.push_back(dirEntry.path);
                    }
                    p.callback(paths);
                }
                else if (dirEntry.isDir)
                {
                    p.model->setPath(toFileSystem(dirEntry.path.get()));
                }
                break;
            case FileBrowserMode::Dir:
                if (dirEntry.isDir)
                {
                    p.model->setPath(toFileSystem(dirEntry.path.get()));
                }
                break;
            default: break;
            }
        }
    }
}
