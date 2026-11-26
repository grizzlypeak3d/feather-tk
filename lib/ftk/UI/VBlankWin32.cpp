// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/VBlankPrivate.h>

#include <SDL3/SDL.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>
#include <dxgi.h>

#include <atomic>
#include <mutex>
#include <thread>

namespace ftk
{
    namespace
    {
        //! The DXGI output showing a monitor, or nothing.
        IDXGIOutput* findOutput(IDXGIFactory1* factory, HMONITOR monitor)
        {
            IDXGIOutput* out = nullptr;
            IDXGIAdapter1* adapter = nullptr;
            for (UINT a = 0; !out && factory->EnumAdapters1(a, &adapter) != DXGI_ERROR_NOT_FOUND; ++a)
            {
                IDXGIOutput* output = nullptr;
                for (UINT o = 0; !out && adapter->EnumOutputs(o, &output) != DXGI_ERROR_NOT_FOUND; ++o)
                {
                    DXGI_OUTPUT_DESC desc;
                    if (SUCCEEDED(output->GetDesc(&desc)) && desc.Monitor == monitor)
                    {
                        out = output;
                    }
                    else
                    {
                        output->Release();
                    }
                }
                adapter->Release();
            }
            return out;
        }
    }

    struct VBlankWatcher::Private
    {
        HWND hwnd = nullptr;
        std::atomic<bool> running{ true };
        std::thread thread;

        mutable std::mutex mutex;
        std::vector<std::chrono::steady_clock::time_point> vblanks;
        std::chrono::steady_clock::time_point latest;
    };

    VBlankWatcher::VBlankWatcher(SDL_Window* window) :
        _p(new Private)
    {
        Private* p = _p.get();
        p->hwnd = static_cast<HWND>(SDL_GetPointerProperty(
            SDL_GetWindowProperties(window),
            SDL_PROP_WINDOW_WIN32_HWND_POINTER,
            nullptr));
        if (!p->hwnd)
            return;
        p->thread = std::thread(
            [p]
            {
                IDXGIFactory1* factory = nullptr;
                if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory))))
                    return;
                HMONITOR monitor = nullptr;
                IDXGIOutput* output = nullptr;
                while (p->running)
                {
                    // The window can be moved to another monitor.
                    const HMONITOR current = MonitorFromWindow(p->hwnd, MONITOR_DEFAULTTONEAREST);
                    if (current != monitor || !output)
                    {
                        if (output)
                        {
                            output->Release();
                        }
                        monitor = current;
                        output = findOutput(factory, monitor);
                    }
                    if (!output || FAILED(output->WaitForVBlank()))
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(16));
                        continue;
                    }
                    const auto now = std::chrono::steady_clock::now();
                    std::lock_guard<std::mutex> lock(p->mutex);
                    p->vblanks.push_back(now);
                    // Kept short: taken once a refresh while playback is
                    // paced, and not otherwise.
                    if (p->vblanks.size() > 8)
                    {
                        p->vblanks.erase(p->vblanks.begin());
                    }
                    p->latest = now;
                }
                if (output)
                {
                    output->Release();
                }
                factory->Release();
            });
    }

    VBlankWatcher::~VBlankWatcher()
    {
        _p->running = false;
        if (_p->thread.joinable())
        {
            _p->thread.join();
        }
    }

    bool VBlankWatcher::isActive() const
    {
        std::lock_guard<std::mutex> lock(_p->mutex);
        return std::chrono::steady_clock::now() - _p->latest < std::chrono::milliseconds(100);
    }

    std::vector<std::chrono::steady_clock::time_point> VBlankWatcher::take()
    {
        std::lock_guard<std::mutex> lock(_p->mutex);
        std::vector<std::chrono::steady_clock::time_point> out;
        out.swap(_p->vblanks);
        return out;
    }
}
