// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IconSystem.h>

#include <ftk/Core/ImageIO.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LRUCache.h>

#include <lunasvg/lunasvg.h>

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>

namespace ftk_resource
{
    extern std::vector<uint8_t> ArrowDown;
    extern std::vector<uint8_t> ArrowLeft;
    extern std::vector<uint8_t> ArrowRight;
    extern std::vector<uint8_t> ArrowUp;
    extern std::vector<uint8_t> Audio;
    extern std::vector<uint8_t> BellowsClosed;
    extern std::vector<uint8_t> BellowsOpen;
    extern std::vector<uint8_t> Clear;
    extern std::vector<uint8_t> ClearSmall;
    extern std::vector<uint8_t> Close;
    extern std::vector<uint8_t> CloseSmall;
    extern std::vector<uint8_t> Copy;
    extern std::vector<uint8_t> Cut;
    extern std::vector<uint8_t> Decrement;
    extern std::vector<uint8_t> Directory;
    extern std::vector<uint8_t> DirectoryBack;
    extern std::vector<uint8_t> DirectoryForward;
    extern std::vector<uint8_t> DirectoryUp;
    extern std::vector<uint8_t> Edit;
    extern std::vector<uint8_t> Empty;
    extern std::vector<uint8_t> File;
    extern std::vector<uint8_t> FileBrowser;
    extern std::vector<uint8_t> FileClose;
    extern std::vector<uint8_t> FileCloseAll;
    extern std::vector<uint8_t> FileNew;
    extern std::vector<uint8_t> FileOpen;
    extern std::vector<uint8_t> FileReload;
    extern std::vector<uint8_t> FileSave;
    extern std::vector<uint8_t> FrameEnd;
    extern std::vector<uint8_t> FrameInOut;
    extern std::vector<uint8_t> FrameNext;
    extern std::vector<uint8_t> FramePrev;
    extern std::vector<uint8_t> FrameStart;
    extern std::vector<uint8_t> Increment;
    extern std::vector<uint8_t> Info;
    extern std::vector<uint8_t> MenuArrow;
    extern std::vector<uint8_t> MenuChecked;
    extern std::vector<uint8_t> Mute;
    extern std::vector<uint8_t> Next;
    extern std::vector<uint8_t> PanelBottom;
    extern std::vector<uint8_t> PanelLeft;
    extern std::vector<uint8_t> PanelRight;
    extern std::vector<uint8_t> PanelTop;
    extern std::vector<uint8_t> Paste;
    extern std::vector<uint8_t> PlaybackForward;
    extern std::vector<uint8_t> PlaybackReverse;
    extern std::vector<uint8_t> PlaybackStop;
    extern std::vector<uint8_t> Prev;
    extern std::vector<uint8_t> Redo;
    extern std::vector<uint8_t> Reload;
    extern std::vector<uint8_t> Reset;
    extern std::vector<uint8_t> ReverseSort;
    extern std::vector<uint8_t> Search;
    extern std::vector<uint8_t> Settings;
    extern std::vector<uint8_t> SubMenuArrow;
    extern std::vector<uint8_t> Time;
    extern std::vector<uint8_t> Undo;
    extern std::vector<uint8_t> ViewFrame;
    extern std::vector<uint8_t> ViewZoomIn;
    extern std::vector<uint8_t> ViewZoomOut;
    extern std::vector<uint8_t> ViewZoomReset;
    extern std::vector<uint8_t> Volume;
    extern std::vector<uint8_t> WindowFullScreen;
    extern std::vector<uint8_t> feather_tk_512;
}

namespace ftk
{
    namespace
    {
        const size_t requestCount = 1;
    }

    struct IconSystem::Private
    {
        std::weak_ptr<Context> context;

        std::map<std::string, std::vector<uint8_t> > iconData;

        uint64_t id = 0;

        struct Request
        {
            uint64_t id = 0;
            std::string name;
            float displayScale = 1.F;
            std::promise<std::shared_ptr<Image> > promise;
        };
        const size_t requestTimeout = 5;

        typedef std::pair<std::string, float> CacheKey;

        struct Mutex
        {
            std::list<std::shared_ptr<Request> > requests;
            LRUCache<CacheKey, std::shared_ptr<Image> > cache;
            bool stopped = false;
            std::mutex mutex;
        };
        Mutex mutex;

        struct Thread
        {
            std::condition_variable cv;
            std::thread thread;
            std::atomic<bool> running;
        };
        Thread thread;

        void _cancelRequests();
    };

    void IconSystem::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();
        p.context = context;

        p.iconData["ArrowDown"] = ftk_resource::ArrowDown;
        p.iconData["ArrowLeft"] = ftk_resource::ArrowLeft;
        p.iconData["ArrowRight"] = ftk_resource::ArrowRight;
        p.iconData["ArrowUp"] = ftk_resource::ArrowUp;
        p.iconData["Audio"] = ftk_resource::Audio;
        p.iconData["BellowsClosed"] = ftk_resource::BellowsClosed;
        p.iconData["BellowsOpen"] = ftk_resource::BellowsOpen;
        p.iconData["Clear"] = ftk_resource::Clear;
        p.iconData["ClearSmall"] = ftk_resource::ClearSmall;
        p.iconData["Close"] = ftk_resource::Close;
        p.iconData["CloseSmall"] = ftk_resource::CloseSmall;
        p.iconData["Copy"] = ftk_resource::Copy;
        p.iconData["Cut"] = ftk_resource::Cut;
        p.iconData["Decrement"] = ftk_resource::Decrement;
        p.iconData["Directory"] = ftk_resource::Directory;
        p.iconData["DirectoryBack"] = ftk_resource::DirectoryBack;
        p.iconData["DirectoryForward"] = ftk_resource::DirectoryForward;
        p.iconData["DirectoryUp"] = ftk_resource::DirectoryUp;
        p.iconData["Edit"] = ftk_resource::Edit;
        p.iconData["Empty"] = ftk_resource::Empty;
        p.iconData["File"] = ftk_resource::File;
        p.iconData["FileBrowser"] = ftk_resource::FileBrowser;
        p.iconData["FileClose"] = ftk_resource::FileClose;
        p.iconData["FileCloseAll"] = ftk_resource::FileCloseAll;
        p.iconData["FileNew"] = ftk_resource::FileNew;
        p.iconData["FileOpen"] = ftk_resource::FileOpen;
        p.iconData["FileReload"] = ftk_resource::FileReload;
        p.iconData["FileSave"] = ftk_resource::FileSave;
        p.iconData["FrameEnd"] = ftk_resource::FrameEnd;
        p.iconData["FrameInOut"] = ftk_resource::FrameInOut;
        p.iconData["FrameNext"] = ftk_resource::FrameNext;
        p.iconData["FramePrev"] = ftk_resource::FramePrev;
        p.iconData["FrameStart"] = ftk_resource::FrameStart;
        p.iconData["Increment"] = ftk_resource::Increment;
        p.iconData["Info"] = ftk_resource::Info;
        p.iconData["MenuArrow"] = ftk_resource::MenuArrow;
        p.iconData["MenuChecked"] = ftk_resource::MenuChecked;
        p.iconData["Mute"] = ftk_resource::Mute;
        p.iconData["Next"] = ftk_resource::Next;
        p.iconData["PanelBottom"] = ftk_resource::PanelBottom;
        p.iconData["PanelLeft"] = ftk_resource::PanelLeft;
        p.iconData["PanelRight"] = ftk_resource::PanelRight;
        p.iconData["PanelTop"] = ftk_resource::PanelTop;
        p.iconData["Paste"] = ftk_resource::Paste;
        p.iconData["PlaybackForward"] = ftk_resource::PlaybackForward;
        p.iconData["PlaybackReverse"] = ftk_resource::PlaybackReverse;
        p.iconData["PlaybackStop"] = ftk_resource::PlaybackStop;
        p.iconData["Prev"] = ftk_resource::Prev;
        p.iconData["Redo"] = ftk_resource::Redo;
        p.iconData["Reload"] = ftk_resource::Reload;
        p.iconData["Reset"] = ftk_resource::Reset;
        p.iconData["ReverseSort"] = ftk_resource::ReverseSort;
        p.iconData["Search"] = ftk_resource::Search;
        p.iconData["Settings"] = ftk_resource::Settings;
        p.iconData["SubMenuArrow"] = ftk_resource::SubMenuArrow;
        p.iconData["Time"] = ftk_resource::Time;
        p.iconData["Undo"] = ftk_resource::Undo;
        p.iconData["ViewFrame"] = ftk_resource::ViewFrame;
        p.iconData["ViewZoomIn"] = ftk_resource::ViewZoomIn;
        p.iconData["ViewZoomOut"] = ftk_resource::ViewZoomOut;
        p.iconData["ViewZoomReset"] = ftk_resource::ViewZoomReset;
        p.iconData["Volume"] = ftk_resource::Volume;
        p.iconData["WindowFullScreen"] = ftk_resource::WindowFullScreen;
        p.iconData["feather_tk_512"] = ftk_resource::feather_tk_512;

        p.mutex.cache.setMax(1000);
        p.thread.running = true;
        p.thread.thread = std::thread(
            [this]
            {
                FTK_P();
                while (p.thread.running)
                {
                    std::list<std::shared_ptr<Private::Request> > requests;
                    {
                        std::unique_lock<std::mutex> lock(p.mutex.mutex);
                        if (p.thread.cv.wait_for(
                            lock,
                            std::chrono::milliseconds(p.requestTimeout),
                            [this]
                            {
                                return !_p->mutex.requests.empty();
                            }))
                        {
                            for (
                                size_t i = 0;
                                i < requestCount && !p.mutex.requests.empty();
                                ++i)
                            {
                                requests.push_back(p.mutex.requests.front());
                                p.mutex.requests.pop_front();
                            }
                        }
                    }

                    for (const auto& request : requests)
                    {
                        //std::cout << "icon request: " << request->name << " " << request->displayScale << std::endl;
                        std::shared_ptr<Image> image;
                        bool cached = false;
                        {
                            std::unique_lock<std::mutex> lock(p.mutex.mutex);
                            cached = p.mutex.cache.get(
                                std::make_pair(request->name, request->displayScale),
                                image);
                        }
                        if (!cached)
                        {
                            std::vector<uint8_t> resource;
                            if (resource.empty())
                            {
                                const auto i = p.iconData.find(request->name);
                                if (i != p.iconData.end())
                                {
                                    resource = i->second;
                                }
                            }
                            if (!resource.empty())
                            {
                                if (auto context = p.context.lock())
                                {
                                    const std::string s(resource.begin(), resource.end());
                                    if (auto doc = lunasvg::Document::loadFromData(s))
                                    {
                                        const int w = doc->width() * request->displayScale;
                                        const int h = doc->height() * request->displayScale;
                                        auto bitmap = doc->renderToBitmap(w, h, 0x00000000);
                                        if (!bitmap.isNull())
                                        {
                                            image = Image::create(w, h, ImageType::RGBA_U8);
                                            for (int y = 0; y < h; ++y)
                                            {
                                                uint8_t* imageP = image->getData() + y * w * 4;
                                                const uint8_t* bitmapP = bitmap.data() + (h - 1 - y) * w * 4;
                                                for (int x = 0; x < w; ++x, imageP += 4, bitmapP += 4)
                                                {
                                                    imageP[0] = bitmapP[2];
                                                    imageP[1] = bitmapP[1];
                                                    imageP[2] = bitmapP[0];
                                                    imageP[3] = bitmapP[3];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        request->promise.set_value(image);
                        {
                            std::unique_lock<std::mutex> lock(p.mutex.mutex);
                            p.mutex.cache.add(
                                std::make_pair(request->name, request->displayScale),
                                image);
                        }
                    }
                }
                {
                    std::unique_lock<std::mutex> lock(p.mutex.mutex);
                    p.mutex.stopped = true;
                }
                p._cancelRequests();
            });
    }

    IconSystem::IconSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::IconSystem"),
        _p(new Private)
    {}

    IconSystem::~IconSystem()
    {
        FTK_P();
        p.thread.running = false;
        if (p.thread.thread.joinable())
        {
            p.thread.thread.join();
        }
    }

    std::shared_ptr<IconSystem> IconSystem::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<IconSystem>(new IconSystem(context));
        out->_init(context);
        return out;
    }

    std::vector<std::string> IconSystem::getNames() const
    {
        FTK_P();
        std::vector<std::string> out;
        for (const auto& i : p.iconData)
        {
            out.push_back(i.first);
        }
        return out;
    }
    
    void IconSystem::add(const std::string& name, const std::vector<uint8_t>& svg)
    {
        FTK_P();
        p.iconData[name] = svg;
    }

    std::shared_ptr<Image> IconSystem::get(
        const std::string& name,
        float displayScale)
    {
        return request(name, displayScale).future.get();
    }

    IconRequest IconSystem::request(
        const std::string& name,
        float displayScale)
    {
        FTK_P();
        IconRequest out;
        out.id = p.id++;
        auto request = std::make_shared<Private::Request>();
        request->id = out.id;
        request->name = name;
        request->displayScale = displayScale;
        out.future = request->promise.get_future();
        std::shared_ptr<Image> image;
        bool cached = false;
        {
            std::unique_lock<std::mutex> lock(p.mutex.mutex);
            cached = p.mutex.cache.get(
                std::make_pair(name, displayScale),
                image);
        }
        if (cached)
        {
            request->promise.set_value(image);
        }
        else
        {
            bool valid = false;
            {
                std::unique_lock<std::mutex> lock(p.mutex.mutex);
                if (!p.mutex.stopped)
                {
                    valid = true;
                    p.mutex.requests.push_back(request);
                }
            }
            if (valid)
            {
                p.thread.cv.notify_one();
            }
            else
            {
                request->promise.set_value(nullptr);
            }
        }
        return out;
    }

    void IconSystem::cancelRequests(const std::vector<uint64_t>& ids)
    {
        FTK_P();
        std::list<std::shared_ptr<Private::Request> > requests;
        {
            std::unique_lock<std::mutex> lock(p.mutex.mutex);
            auto i = p.mutex.requests.begin();
            while (i != p.mutex.requests.end())
            {
                const auto j = std::find(ids.begin(), ids.end(), (*i)->id);
                if (j != ids.end())
                {
                    i = p.mutex.requests.erase(i);
                }
                else
                {
                    ++i;
                }
            }
        }
    }

    void IconSystem::Private::_cancelRequests()
    {
        std::list<std::shared_ptr<Private::Request> > requests;
        {
            std::unique_lock<std::mutex> lock(mutex.mutex);
            requests = std::move(mutex.requests);
        }
        for (auto& request : requests)
        {
            request->promise.set_value(nullptr);
        }
    }
}
