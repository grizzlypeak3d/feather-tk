// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the tlRender project.

#include <ftk/Core/FileLogSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FileIO.h>
#include <ftk/Core/Time.h>

#include <atomic>
#include <condition_variable>
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
            std::condition_variable stop;
            std::mutex stopMutex;
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

                    // Wait out the rest of the tick, but wake as soon as the
                    // destructor clears the flag. Sleeping instead made
                    // shutdown wait for the current tick to expire.
                    const auto t1 = std::chrono::steady_clock::now();
                    const auto period =
                        std::chrono::duration_cast<std::chrono::microseconds>(timeout);
                    const auto elapsed =
                        std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
                    std::unique_lock<std::mutex> lock(p.thread.stopMutex);
                    p.thread.stop.wait_for(
                        lock,
                        period > elapsed ? period - elapsed : std::chrono::microseconds(0),
                        [&p] { return !p.thread.running; });
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
        {
            // Under the mutex so the thread cannot test the flag and begin
            // waiting between the store and the notify.
            std::unique_lock<std::mutex> lock(p.thread.stopMutex);
            p.thread.running = false;
        }
        p.thread.stop.notify_one();
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
