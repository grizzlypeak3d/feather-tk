// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/ISystem.h>
#include <ftk/Core/Image.h>

#include <future>

namespace ftk
{
    //! \name Icons
    ///@{

    struct FTK_UI_API_TYPE IconRequest
    {
        uint64_t id = 0;
        std::future<std::shared_ptr<Image> > future;
    };
        
    //! Icon system.
    class FTK_UI_API_TYPE IconSystem : public ISystem
    {
        FTK_NON_COPYABLE(IconSystem);

    protected:
        void _init(const std::shared_ptr<Context>&);

        IconSystem(const std::shared_ptr<Context>&);

    public:
        FTK_UI_API ~IconSystem();

        //! Create a new system.
        FTK_UI_API static std::shared_ptr<IconSystem> create(
            const std::shared_ptr<Context>&);

        //! Get the icon names.
        FTK_UI_API std::vector<std::string> getNames() const;
        
        //! Add an icon. The icon is stored as an SVG file.
        FTK_UI_API void add(const std::string& name, const std::vector<uint8_t>& svg);

        //! Get an icon.
        FTK_UI_API std::shared_ptr<Image> get(
            const std::string& name,
            float displayScale);

        //! Request an async icon.
        FTK_UI_API IconRequest request(
            const std::string& name,
            float displayScale);

        //! Cancel async requests.
        FTK_UI_API void cancelRequests(const std::vector<uint64_t>&);

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
