// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/RenderPrivate.h>

#include <ftk/Core/Format.h>

namespace ftk
{
    namespace gl
    {
        std::string vertexSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "attribute vec3 vPos;\n"
                "attribute vec2 vTexture;\n"
                "varying vec2 fTexture;\n"
                "\n"
                "struct Transform\n"
                "{\n"
                "    mat4 mvp;\n"
                "};\n"
                "\n"
                "uniform Transform transform;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = transform.mvp * vec4(vPos, 1.0);\n"
                "    fTexture = vTexture;\n"
                "}\n";
        }

        std::string meshFragmentSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "uniform vec4 color;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    gl_FragColor = color;\n"
                "}\n";
        }

        std::string colorMeshVertexSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "attribute vec3 vPos;\n"
                "attribute vec4 vColor;\n"
                "varying vec4 fColor;\n"
                "\n"
                "struct Transform\n"
                "{\n"
                "    mat4 mvp;\n"
                "};\n"
                "\n"
                "uniform Transform transform;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = transform.mvp * vec4(vPos, 1.0);\n"
                "    fColor = vColor;\n"
                "}\n";
        }

        std::string colorMeshFragmentSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "varying vec4 fColor;\n"
                "\n"
                "uniform vec4 color;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    gl_FragColor = fColor * color;\n"
                "}\n";
        }

        std::string textureFragmentSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "varying vec2 fTexture;\n"
                "\n"
                "uniform vec4 color;\n"
                "uniform sampler2D textureSampler;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    gl_FragColor = texture2D(textureSampler, fTexture) * color;\n"
                "}\n";
        }

        std::string textFragmentSource()
        {
            return
                "precision mediump float;\n"
                "\n"
                "varying vec2 fTexture;\n"
                "\n"
                "uniform vec4 color;\n"
                "uniform sampler2D textureSampler;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    gl_FragColor.r = color.r;\n"
                "    gl_FragColor.g = color.g;\n"
                "    gl_FragColor.b = color.b;\n"
                "    gl_FragColor.a = color.a * texture2D(textureSampler, fTexture).r;\n"
                "}\n";
        }

        namespace
        {
            const std::string imageType =
                "// enum ftk::ImageType\n"
                "const int ImageType_None              = 0;\n"
                "const int ImageType_L_U8              = 1;\n"
                "const int ImageType_L_U16             = 2;\n"
                "const int ImageType_L_U32             = 3;\n"
                "const int ImageType_L_F16             = 4;\n"
                "const int ImageType_L_F32             = 5;\n"
                "const int ImageType_LA_U8             = 6;\n"
                "const int ImageType_LA_U32            = 7;\n"
                "const int ImageType_LA_U16            = 8;\n"
                "const int ImageType_LA_F16            = 9;\n"
                "const int ImageType_LA_F32            = 10;\n"
                "const int ImageType_RGB_U8            = 11;\n"
                "const int ImageType_RGB_U10           = 12;\n"
                "const int ImageType_RGB_U16           = 13;\n"
                "const int ImageType_RGB_U32           = 14;\n"
                "const int ImageType_RGB_F16           = 15;\n"
                "const int ImageType_RGB_F32           = 16;\n"
                "const int ImageType_RGBA_U8           = 17;\n"
                "const int ImageType_RGBA_U16          = 18;\n"
                "const int ImageType_RGBA_U32          = 19;\n"
                "const int ImageType_RGBA_F16          = 20;\n"
                "const int ImageType_RGBA_F32          = 21;\n"
                "const int ImageType_YUV_420P_U8       = 22;\n"
                "const int ImageType_YUV_422P_U8       = 23;\n"
                "const int ImageType_YUV_444P_U8       = 24;\n"
                "const int ImageType_YUV_420P_U16      = 25;\n"
                "const int ImageType_YUV_422P_U16      = 26;\n"
                "const int ImageType_YUV_444P_U16      = 27;\n"
                "const int ImageType_ARGB_4444_Premult = 28;\n";

            const std::string videoLevels =
                "// enum ftk::VideoLevels\n"
                "const int VideoLevels_FullRange  = 0;\n"
                "const int VideoLevels_LegalRange = 1;\n";

            const std::string sampleTexture =
                "vec4 sampleTexture("
                "    vec2 textureCoord,\n"
                "    int imageType,\n"
                "    int videoLevels,\n"
                "    vec4 yuvCoefficients,\n"
                "    int imageChannels,\n"
                "    sampler2D s0,\n"
                "    sampler2D s1,\n"
                "    sampler2D s2)\n"
                "{\n"
                "    vec4 c;\n"
                "    if (ImageType_YUV_420P_U8 == imageType ||\n"
                "        ImageType_YUV_422P_U8 == imageType ||\n"
                "        ImageType_YUV_444P_U8 == imageType ||\n"
                "        ImageType_YUV_420P_U16 == imageType ||\n"
                "        ImageType_YUV_422P_U16 == imageType ||\n"
                "        ImageType_YUV_444P_U16 == imageType)\n"
                "    {\n"
                "        if (VideoLevels_FullRange == videoLevels)\n"
                "        {\n"
                "            float y  = texture2D(s0, textureCoord).r;\n"
                "            float cb = texture2D(s1, textureCoord).r - 0.5;\n"
                "            float cr = texture2D(s2, textureCoord).r - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        else if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            float y  = (texture2D(s0, textureCoord).r - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            float cb = (texture2D(s1, textureCoord).r - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            float cr = (texture2D(s2, textureCoord).r - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        c.a = 1.0;\n"
                "    }\n"
                "    else\n"
                "    {\n"
                "        c = texture2D(s0, textureCoord);\n"
                "\n"
                "        // Video levels.\n"
                "        if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            c.r = (c.r - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            c.g = (c.g - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0));\n"
                "            c.b = (c.b - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0));\n"
                "        }\n"
                "\n"
                "        // Swizzle for the image channels.\n"
                "        if (1 == imageChannels)\n"
                "        {\n"
                "            c.g = c.b = c.r;\n"
                "            c.a = 1.0;\n"
                "        }\n"
                "        else if (2 == imageChannels)\n"
                "        {\n"
                "            c.a = c.g;\n"
                "            c.g = c.b = c.r;\n"
                "        }\n"
                "        else if (3 == imageChannels)\n"
                "        {\n"
                "            c.a = 1.0;\n"
                "        }\n"
                "    }\n"
                "    return c;\n"
                "}\n";
        }

        std::string imageFragmentSource()
        {
            return Format(
                "precision mediump float;\n"
                "\n"
                "varying vec2 fTexture;\n"
                "\n"
                "{0}\n"
                "\n"
                "{1}\n"
                "\n"
                "{2}\n"
                "\n"
                "uniform vec4      color;\n"
                "uniform int       imageType;\n"
                "uniform int       videoLevels;\n"
                "uniform vec4      yuvCoefficients;\n"
                "uniform int       imageChannels;\n"
                "uniform int       mirrorX;\n"
                "uniform int       mirrorY;\n"
                "uniform sampler2D textureSampler0;\n"
                "uniform sampler2D textureSampler1;\n"
                "uniform sampler2D textureSampler2;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec2 t = fTexture;\n"
                "    if (1 == mirrorX)\n"
                "    {\n"
                "        t.x = 1.0 - t.x;\n"
                "    }\n"
                "    if (0 == mirrorY)\n"
                "    {\n"
                "        t.y = 1.0 - t.y;\n"
                "    }\n"
                "    gl_FragColor = sampleTexture("
                "        t,\n"
                "        imageType,\n"
                "        videoLevels,\n"
                "        yuvCoefficients,\n"
                "        imageChannels,\n"
                "        textureSampler0,\n"
                "        textureSampler1,\n"
                "        textureSampler2) *\n"
                "        color;\n"
                "}\n").
                arg(imageType).
                arg(videoLevels).
                arg(sampleTexture);
        }
    }
}
