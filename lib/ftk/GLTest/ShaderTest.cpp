// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/ShaderTest.h>

#include <ftk/GL/Shader.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        ShaderTest::ShaderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::ShaderTest")
        {}

        ShaderTest::~ShaderTest()
        {}

        std::shared_ptr<ShaderTest> ShaderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ShaderTest>(new ShaderTest(context));
        }
        
        namespace
        {
            std::shared_ptr<Window> createWindow(
                const std::shared_ptr<Context>& context)
            {
                return Window::create(
                    context,
                    "ShaderTest",
                    Size2I(100, 100),
                    static_cast<int>(WindowOptions::MakeCurrent));
            }
        }

        namespace
        {
#if defined(FTK_API_GL_4_1)
            std::string vertexSource = 
                "#version 410\n"
                "\n"
                "in vec3 vPos;\n"
                "\n"
                "uniform int i;\n"
                "uniform float f;\n"
                "uniform vec2 v2;\n"
                "uniform vec3 v3;\n"
                "uniform vec4 v4;\n"
                "uniform mat3 m3;\n"
                "uniform mat4 m4;\n"
                "uniform vec4 c;\n"
                "uniform int ai[4];\n"
                "uniform float af[4];\n"
                "uniform vec3 av3[4];\n"
                "uniform vec4 av4[4];\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4(vPos, 1.0);\n"
                "}\n";
            std::string fragmentSource =
                "#version 410\n"
                "\n"
                "out vec4 outColor;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    outColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n";
#elif defined(FTK_API_GLES_2)
            std::string vertexSource =
                "precision mediump float;\n"
                "\n"
                "attribute vec3 vPos;\n"
                "\n"
                "uniform int i;\n"
                "uniform float f;\n"
                "uniform vec2 v2;\n"
                "uniform vec3 v3;\n"
                "uniform vec4 v4;\n"
                "uniform mat3 m3;\n"
                "uniform mat4 m4;\n"
                "uniform vec4 c;\n"
                "uniform int ai[4];\n"
                "uniform float af[4];\n"
                "uniform vec3 av3[4];\n"
                "uniform vec4 av4[4];\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4(vPos, 1.0);\n"
                "}\n";
            std::string fragmentSource =
                "precision mediump float;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n";
#endif // FTK_API_GL_4_1
        }
               
        void ShaderTest::run()
        {
            {
                auto window = createWindow(_context);

                auto shader = Shader::create(vertexSource, fragmentSource);
                FTK_ASSERT(vertexSource == shader->getVertexSource());
                FTK_ASSERT(fragmentSource == shader->getFragmentSource());
                FTK_ASSERT(shader->getProgram());
                shader->bind();
                shader->setUniform("i", 1);
                shader->setUniform("f", 1.F);
                shader->setUniform("v2", V2F(1.F, 1.F));
                shader->setUniform("v3", V3F(1.F, 1.F, 1.F));
                shader->setUniform("v4", V4F(1.F, 1.F, 1.F, 1.F));
                shader->setUniform("m3", M33F());
                shader->setUniform("m4", M44F());
                shader->setUniform("c", Color4F(1.F, 1.F, 1.F, 1.F));
                const float af[] = { 1.F, 1.F, 1.F, 1.F };
                shader->setUniform("af", af);
                shader->setUniform("ai", std::vector<int>({ 1, 1, 1, 1 }));
                shader->setUniform("af", std::vector<float>({ 1.F, 1.F, 1.F, 1.F }));
                shader->setUniform("av3", std::vector<V3F>(4, V3F(1.F, 1.F, 1.F)));
                shader->setUniform("av4", std::vector<V4F>(4, V4F(1.F, 1.F, 1.F, 1.F)));
            }
            {
                auto window = createWindow(_context);
                try
                {
                    auto shader = Shader::create("!", "!");
                    FTK_ASSERT(false);
                }
                catch (const std::exception&)
                {}
                try
                {
                    auto shader = Shader::create(vertexSource, "!");
                    FTK_ASSERT(false);
                }
                catch (const std::exception&)
                {}
            }
        }
    }
}

