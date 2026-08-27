// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Util.h>

#include <ftk/GL/GL.h>

#include <ftk/Core/String.h>

#include <cctype>
#include <sstream>

namespace ftk
{
    namespace gl
    {
        int getMajorVersion(const std::string& version)
        {
            int out = 0;
            auto tmp = split(version, ' ');
            if (!tmp.empty())
            {
                for (size_t i = 0; i < tmp.size(); ++i)
                {
                    if (!tmp[i].empty() && std::isdigit(tmp[i][0]))
                    {
                        tmp = split(tmp[i], '.');
                        if (!tmp.empty())
                        {
                            std::stringstream ss(tmp[0]);
                            ss >> out;
                        }
                        break;
                    }
                }
            }
            return out;
        }

        unsigned int getReadPixelsFormat(ImageType value)
        {
            const std::array<GLenum, static_cast<std::size_t>(ImageType::Count)> data =
            {
                GL_NONE,

#if defined(FTK_API_GL_4_1)
                GL_RED,
                GL_RED,
                GL_RED,
                GL_RED,
                GL_RED,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGB,
                GL_RGBA,
                GL_RGB,
                GL_RGB,
                GL_RGB,
                GL_RGB,

                GL_RGBA,
                GL_RGBA,
                GL_RGBA,
                GL_RGBA,
                GL_RGBA,
#elif defined(FTK_API_GLES_3)
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGB,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_RGBA,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
#endif // FTK_API_GL_4_1

                GL_NONE
            };
            return data[static_cast<std::size_t>(value)];
        }

        unsigned int getReadPixelsType(ImageType value)
        {
            const std::array<GLenum, static_cast<std::size_t>(ImageType::Count)> data =
            {
                GL_NONE,

                GL_UNSIGNED_BYTE,
#if defined(FTK_API_GL_4_1)
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,
#elif defined(FTK_API_GLES_3)
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
#endif // FTK_API_GL_4_1

                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,

                GL_UNSIGNED_BYTE,
#if defined(FTK_API_GL_4_1)
                GL_UNSIGNED_INT_10_10_10_2,
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,
#elif defined(FTK_API_GLES_3)
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
#endif // FTK_API_GL_4_1

                GL_UNSIGNED_BYTE,
#if defined(FTK_API_GL_4_1)
                GL_UNSIGNED_SHORT,
                GL_UNSIGNED_INT,
                GL_HALF_FLOAT,
                GL_FLOAT,
#elif defined(FTK_API_GLES_3)
                GL_NONE,
                GL_NONE,
                GL_NONE,
                GL_NONE,
#endif // FTK_API_GL_4_1

                GL_NONE
            };
            return data[static_cast<std::size_t>(value)];
        }

        void setAlphaBlend(AlphaBlend alphaBlend)
        {
            switch (alphaBlend)
            {
            case AlphaBlend::None:
#if defined(FTK_API_GL_4_1)
                glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
#elif defined(FTK_API_GLES_3)
                glBlendFunc(GL_ONE, GL_ZERO);
#endif // FTK_API_GL_4_1
                break;
            case AlphaBlend::Straight:
#if defined(FTK_API_GL_4_1)
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#elif defined(FTK_API_GLES_3)
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // FTK_API_GL_4_1
                break;
            case AlphaBlend::Premultiplied:
#if defined(FTK_API_GL_4_1)
                glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#elif defined(FTK_API_GLES_3)
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif // FTK_API_GL_4_1
                break;
            default: break;
            }
        }

        struct SetAndRestore::Private
        {
            unsigned int id = 0;
            GLboolean previous = GL_FALSE;
        };

        SetAndRestore::SetAndRestore(unsigned int id, bool value) :
            _p(new Private)
        {
            _p->id = id;

            glGetBooleanv(id, &_p->previous);

            if (value)
            {
                glEnable(id);
            }
            else
            {
                glDisable(id);
            }
        }

        SetAndRestore::~SetAndRestore()
        {
            if (_p->previous)
            {
                glEnable(_p->id);
            }
            else
            {
                glDisable(_p->id);
            }
        }

        namespace
        {
            void setEnabled(GLenum id, GLboolean value)
            {
                if (value)
                {
                    glEnable(id);
                }
                else
                {
                    glDisable(id);
                }
            }
        }

        struct StateSave::Private
        {
            GLboolean blend = GL_FALSE;
            GLint blendSrcRGB = GL_ONE;
            GLint blendDstRGB = GL_ZERO;
            GLint blendSrcAlpha = GL_ONE;
            GLint blendDstAlpha = GL_ZERO;
            GLboolean depthTest = GL_FALSE;
            GLboolean depthMask = GL_TRUE;
            GLboolean cullFace = GL_FALSE;
            GLboolean scissorTest = GL_FALSE;
#if defined(FTK_API_GL_4_1)
            GLboolean programPointSize = GL_FALSE;
#endif // FTK_API_GL_4_1
        };

        StateSave::StateSave() :
            _p(new Private)
        {
            FTK_P();
            glGetBooleanv(GL_BLEND, &p.blend);
            glGetIntegerv(GL_BLEND_SRC_RGB, &p.blendSrcRGB);
            glGetIntegerv(GL_BLEND_DST_RGB, &p.blendDstRGB);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &p.blendSrcAlpha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, &p.blendDstAlpha);
            glGetBooleanv(GL_DEPTH_TEST, &p.depthTest);
            glGetBooleanv(GL_DEPTH_WRITEMASK, &p.depthMask);
            glGetBooleanv(GL_CULL_FACE, &p.cullFace);
            glGetBooleanv(GL_SCISSOR_TEST, &p.scissorTest);
#if defined(FTK_API_GL_4_1)
            glGetBooleanv(GL_PROGRAM_POINT_SIZE, &p.programPointSize);
#endif // FTK_API_GL_4_1
        }

        StateSave::~StateSave()
        {
            FTK_P();
            setEnabled(GL_BLEND, p.blend);
            glBlendFuncSeparate(
                p.blendSrcRGB,
                p.blendDstRGB,
                p.blendSrcAlpha,
                p.blendDstAlpha);
            setEnabled(GL_DEPTH_TEST, p.depthTest);
            glDepthMask(p.depthMask);
            setEnabled(GL_CULL_FACE, p.cullFace);
            setEnabled(GL_SCISSOR_TEST, p.scissorTest);
#if defined(FTK_API_GL_4_1)
            setEnabled(GL_PROGRAM_POINT_SIZE, p.programPointSize);
#endif // FTK_API_GL_4_1
        }

        std::string getErrorLabel(unsigned int value)
        {
            std::string out;
            switch(value)
            {
            case GL_NO_ERROR: out = "GL_NO_ERROR"; break;
            case GL_INVALID_ENUM: out = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: out = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: out = "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: out = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: out = "GL_OUT_OF_MEMORY"; break;
            default: break;
            }
            return out;
        }
    }
}

