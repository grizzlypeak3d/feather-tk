// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/ImageIOTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/ImageIO.h>

namespace ftk
{
    namespace core_test
    {
        ImageIOTest::ImageIOTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::ImageIOTest")
        {}

        ImageIOTest::~ImageIOTest()
        {}

        std::shared_ptr<ImageIOTest> ImageIOTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ImageIOTest>(new ImageIOTest(context));
        }
        
        void ImageIOTest::run()
        {
            _members();
            _functions();
        }
        
        namespace
        {
            class DummyPlugin : public IImagePlugin
            {
            protected:
                DummyPlugin() :
                    IImagePlugin("Dummy", { ".dum" })
                {}

            public:
                virtual ~DummyPlugin()
                {}
                
                static std::shared_ptr<DummyPlugin> create()
                {
                    return std::shared_ptr<DummyPlugin>(new DummyPlugin);
                }
            };
        }
        
        void ImageIOTest::_members()
        {
            {
                auto io = _context->getSystem<ImageIO>();
                auto dummy = DummyPlugin::create();
                io->addPlugin(dummy);
                for (const auto& plugin : io->getPlugins())
                {
                    _print(Format("Plugin: {0}").arg(plugin->getName()));
                }
            }
        }
        
        void ImageIOTest::_functions()
        {
            ImageIOOptions options;
            options["Layer"] = "1";
            ImageIOOptions options2;
            options2["Compression"] = "RLE";
            ImageIOOptions options3 = merge(options, options2);
            FTK_ASSERT(2 == options3.size());
            FTK_ASSERT(options3["Layer"] == "1");
            FTK_ASSERT(options3["Compression"] == "RLE");
        }
    }
}

