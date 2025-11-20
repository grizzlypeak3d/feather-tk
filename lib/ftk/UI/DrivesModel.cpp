// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DrivesModel.h>

#include <ftk/Core/Path.h>
#include <ftk/Core/Time.h>
#include <ftk/Core/Timer.h>

#include <atomic>
#include <mutex>
#include <thread>

namespace ftk
{
    namespace
    {
        const std::chrono::milliseconds timeout(100);
    }

    struct DrivesModel::Private
    {
        std::shared_ptr<ObservableList<std::filesystem::path> > drives;

        struct Mutex
        {
            std::vector<std::filesystem::path> drives;
            std::mutex mutex;
        };
        Mutex mutex;
        std::thread thread;
        std::atomic<bool> running;

        std::shared_ptr<Timer> timer;
    };

    void DrivesModel::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.drives = ObservableList<std::filesystem::path>::create();

        p.running = true;
        p.thread = std::thread(
            [this]
            {
                FTK_P();
                while (p.running)
                {
                    const auto drives = getDrives();
                    {
                        std::lock_guard<std::mutex> lock(p.mutex.mutex);
                        p.mutex.drives = drives;
                    }
                    sleep(timeout);
                }
            });

        p.timer = Timer::create(context);
        p.timer->setRepeating(true);
        p.timer->start(
            timeout,
            [this]
            {
                FTK_P();
                std::vector<std::filesystem::path> drives;
                {
                    std::lock_guard<std::mutex> lock(p.mutex.mutex);
                    drives = p.mutex.drives;
                }
                p.drives->setIfChanged(drives);
            });
    }

    DrivesModel::DrivesModel() :
        _p(new Private)
    {}

    DrivesModel::~DrivesModel()
    {
        FTK_P();
        p.running = false;
        if (p.thread.joinable())
        {
            p.thread.join();
        }
    }

    std::shared_ptr<DrivesModel> DrivesModel::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<DrivesModel>(new DrivesModel);
        out->_init(context);
        return out;
    }

    std::shared_ptr<IObservableList<std::filesystem::path> > DrivesModel::observeDrives() const
    {
        return _p->drives;
    }
}