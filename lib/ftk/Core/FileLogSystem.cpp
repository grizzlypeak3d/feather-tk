// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the tlRender project.

#include <ftk/Core/FileLogSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FileIO.h>
#include <ftk/Core/Time.h>

#include <atomic>
#include <mutex>
#include <thread>

namespace ftk
{
    namespace
    {
        const std::chrono::milliseconds timeout(1000);
    }

    struct FileLogSystem::Private
    {
        std::filesystem::path path;

        std::shared_ptr<ListObserver<LogItem> > logObserver;

        struct Mutex
        {
            std::vector<LogItem> items;
            std::mutex mutex;
        };
        Mutex mutex;

        struct Thread
        {
            std::thread thread;
            std::atomic<bool> running;
        };
        Thread thread;
    };

    FileLogSystem::FileLogSystem(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path) :
        ISystem(context, "ftk:::FileLogSystem"),
        _p(new Private)
    {
        FTK_P();

        p.path = path;

        p.logObserver = ListObserver<LogItem>::create(
            context->getLogSystem()->observeLogItems(),
            [this](const std::vector<LogItem>& value)
            {
                std::unique_lock<std::mutex> lock(_p->mutex.mutex);
                _p->mutex.items.insert(
                    _p->mutex.items.end(),
                    value.begin(),
                    value.end());
            });
            
        p.thread.running = true;
        p.thread.thread = std::thread(
            [this]
            {
                FTK_P();
                {
                    auto io = FileIO::create(p.path, FileMode::Write);
                }
                while (p.thread.running)
                {
                    const auto t0 = std::chrono::steady_clock::now();

                    std::vector<LogItem> items;
                    {
                        std::unique_lock<std::mutex> lock(p.mutex.mutex);
                        std::swap(p.mutex.items, items);
                    }
                    {
                        auto io = FileIO::create(p.path, FileMode::Append);
                        for (const auto& item : items)
                        {
                            io->write(getLabel(item) + "\n");
                        }
                    }

                    const auto t1 = std::chrono::steady_clock::now();
                    sleep(timeout, t0, t1);
                }
                std::vector<LogItem> items;
                {
                    std::unique_lock<std::mutex> lock(p.mutex.mutex);
                    std::swap(p.mutex.items, items);
                }
                {
                    auto io = FileIO::create(p.path, FileMode::Append);
                    for (const auto& item : items)
                    {
                        io->write(getLabel(item) + "\n");
                    }
                }
            });
    }

    FileLogSystem::~FileLogSystem()
    {
        FTK_P();
        p.thread.running = false;
        if (p.thread.thread.joinable())
        {
            p.thread.thread.join();
        }
    }

    std::shared_ptr<FileLogSystem> FileLogSystem::create(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path)
    {
        auto out = context->getSystem<FileLogSystem>();
        if (!out)
        {
            out = std::shared_ptr<FileLogSystem>(new FileLogSystem(context, path));
            context->addSystem(out);
        }
        return out;
    }
}
