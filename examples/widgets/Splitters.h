// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Splitter.h>

using namespace ftk;

namespace widgets
{
    class Splitters : public ftk::IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        Splitters() = default;

    public:
        virtual ~Splitters();

        static std::shared_ptr<Splitters> create(const std::shared_ptr<Context>&);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<ftk::Splitter> _splitter;
    };
}
