// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IconSystem.h>

#include <ftk/Core/ImageIO.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LRUCache.h>

#include <ftk_resource/IconResources.h>

#include <lunasvg/lunasvg.h>

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>

namespace std
{
    template<>
    struct hash<std::pair<std::string, int> >
    {
        size_t operator()(const std::pair<std::string, int>& v) const noexcept
        {
            const size_t seed = std::hash<std::string>{}(v.first);
            return ftk::hashCombine(seed, std::hash<int>{}(v.second));
        }
    };
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

        // The display scale is converted to an int for caching.
        const int displayScaleConvert = 100;
        typedef std::pair<std::string, int> CacheKey;

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

    namespace
    {
        std::shared_ptr<Image> renderIcon(
            const std::map<std::string, std::vector<uint8_t> >& iconData,
            const std::string& name,
            float displayScale)
        {
            std::shared_ptr<Image> image;
            const auto i = iconData.find(name);
            if (i != iconData.end() && !i->second.empty())
            {
                const std::string s(i->second.begin(), i->second.end());
                if (auto doc = lunasvg::Document::loadFromData(s))
                {
                    const int w = doc->width() * displayScale;
                    const int h = doc->height() * displayScale;
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
            return image;
        }
    }

    void IconSystem::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();
        p.context = context;

        // Every icon compiled into the library; the map is generated
        // from the files in etc/Icons.
        for (const auto& i : ftk_resource::getIconResources())
        {
            p.iconData[i.first] = *i.second;
        }

        p.mutex.cache.setMax(1000);
#if defined(__EMSCRIPTEN__)
        // The browser build has no threads: the icons are rendered as they
        // are asked for, in request(). An icon is a small SVG, so at
        // interface scale that costs nothing worth a thread.
        return;
#endif // __EMSCRIPTEN__
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
                                std::make_pair(request->name, static_cast<int>(request->displayScale * p.displayScaleConvert)),
                                image);
                        }
                        if (!cached)
                        {
                            if (auto context = p.context.lock())
                            {
                                image = renderIcon(
                                    p.iconData,
                                    request->name,
                                    request->displayScale);
                            }
                        }
                        request->promise.set_value(image);
                        {
                            std::unique_lock<std::mutex> lock(p.mutex.mutex);
                            p.mutex.cache.add(
                                std::make_pair(
                                    request->name,
                                    static_cast<int>(request->displayScale * p.displayScaleConvert)),
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
                std::make_pair(
                    name,
                    static_cast<int>(displayScale * p.displayScaleConvert)),
                image);
        }
        if (cached)
        {
            request->promise.set_value(image);
        }
#if defined(__EMSCRIPTEN__)
        else
        {
            image = renderIcon(p.iconData, name, displayScale);
            request->promise.set_value(image);
            std::unique_lock<std::mutex> lock(p.mutex.mutex);
            p.mutex.cache.add(
                std::make_pair(
                    name,
                    static_cast<int>(displayScale * p.displayScaleConvert)),
                image);
        }
#else // __EMSCRIPTEN__
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
#endif // __EMSCRIPTEN__
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
