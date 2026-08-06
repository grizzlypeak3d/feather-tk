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
                "#version 410\n"
                "\n"
                "in vec3 vPos;\n"
                "in vec2 vTexture;\n"
                "out vec2 fTexture;\n"
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
                "#version 410\n"
                "\n"
                "out vec4 outColor;\n"
                "\n"
                "uniform vec4 color;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    outColor = color;\n"
                "}\n";
        }

        std::string colorMeshVertexSource()
        {
            return
                "#version 410\n"
                "\n"
                "layout(location = 0) in vec3 vPos;\n"
                "layout(location = 1) in vec4 vColor;\n"
                "out vec4 fColor;\n"
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
                "#version 410\n"
                "\n"
                "in vec4 fColor;\n"
                "out vec4 outColor;\n"
                "\n"
                "uniform vec4 color;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    outColor = fColor * color;\n"
                "}\n";
        }

        std::string textureFragmentSource()
        {
            return
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "uniform vec4 color;\n"
                "uniform bool opaque;\n"
                "uniform sampler2D textureSampler;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    outColor = texture(textureSampler, fTexture) * color;\n"
                "    if (opaque)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "}\n";
        }

        std::string textFragmentSource()
        {
            return
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "uniform vec4 color;\n"
                "uniform sampler2D textureSampler;\n"
                "\n"
                "void main()\n"
                "{\n"
                "\n"
                "    outColor.r = color.r;\n"
                "    outColor.g = color.g;\n"
                "    outColor.b = color.b;\n"
                "    float coverage = texture(textureSampler, fTexture).r * color.a;\n"
                "    //! \todo Should this be configurable?\n"
                "    float gamma = 1.3;\n"
                "    outColor.a = pow(coverage, 1.0 / gamma);\n"
                "}\n";
        }

        namespace
        {
            const std::string imageType =
                "// enum ftk::ImageType\n"
                "const uint ImageType_None              = 0;\n"
                "const uint ImageType_L_U8              = 1;\n"
                "const uint ImageType_L_U16             = 2;\n"
                "const uint ImageType_L_U32             = 3;\n"
                "const uint ImageType_L_F16             = 4;\n"
                "const uint ImageType_L_F32             = 5;\n"
                "const uint ImageType_LA_U8             = 6;\n"
                "const uint ImageType_LA_U32            = 7;\n"
                "const uint ImageType_LA_U16            = 8;\n"
                "const uint ImageType_LA_F16            = 9;\n"
                "const uint ImageType_LA_F32            = 10;\n"
                "const uint ImageType_RGB_U8            = 11;\n"
                "const uint ImageType_RGB_U10           = 12;\n"
                "const uint ImageType_RGB_U16           = 13;\n"
                "const uint ImageType_RGB_U32           = 14;\n"
                "const uint ImageType_RGB_F16           = 15;\n"
                "const uint ImageType_RGB_F32           = 16;\n"
                "const uint ImageType_RGBA_U8           = 17;\n"
                "const uint ImageType_RGBA_U16          = 18;\n"
                "const uint ImageType_RGBA_U32          = 19;\n"
                "const uint ImageType_RGBA_F16          = 20;\n"
                "const uint ImageType_RGBA_F32          = 21;\n"
                "const uint ImageType_YUV_420P_U8       = 22;\n"
                "const uint ImageType_YUV_422P_U8       = 23;\n"
                "const uint ImageType_YUV_444P_U8       = 24;\n"
                "const uint ImageType_YUV_420P_U16      = 25;\n"
                "const uint ImageType_YUV_422P_U16      = 26;\n"
                "const uint ImageType_YUV_444P_U16      = 27;\n"
                "const uint ImageType_YUV_420SP_U8      = 28;\n"
                "const uint ImageType_YUV_420SP_U16     = 29;\n";

            const std::string channelDisplay =
                "// enum ftk::ChannelDisplay\n"
                "const uint ChannelDisplay_Color = 0;\n"
                "const uint ChannelDisplay_Red   = 1;\n"
                "const uint ChannelDisplay_Green = 2;\n"
                "const uint ChannelDisplay_Blue  = 3;\n"
                "const uint ChannelDisplay_Alpha = 4;\n";

            const std::string videoLevels =
                "// enum ftk::VideoLevels\n"
                "const uint VideoLevels_FullRange  = 0;\n"
                "const uint VideoLevels_LegalRange = 1;\n";

            const std::string sampleTexture =
                "vec4 sampleTexture("
                "    vec2 textureCoord,\n"
                "    int imageType,\n"
                "    int channelCount,\n"
                "    int videoLevels,\n"
                "    vec4 yuvCoefficients,\n"
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
                "            float y  = texture(s0, textureCoord).r;\n"
                "            float cb = texture(s1, textureCoord).r - 0.5;\n"
                "            float cr = texture(s2, textureCoord).r - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        else if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            float y  = (texture(s0, textureCoord).r - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            float cb = (texture(s1, textureCoord).r - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            float cr = (texture(s2, textureCoord).r - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        c.r = clamp(c.r, 0.0, 1.0);\n"
                "        c.g = clamp(c.g, 0.0, 1.0);\n"
                "        c.b = clamp(c.b, 0.0, 1.0);\n"
                "        c.a = 1.0;\n"
                "    }\n"
                "    else if (ImageType_YUV_420SP_U8 == imageType ||\n"
                "        ImageType_YUV_420SP_U16 == imageType)\n"
                "    {\n"
                "        if (VideoLevels_FullRange == videoLevels)\n"
                "        {\n"
                "            float y  = texture(s0, textureCoord).r;\n"
                "            float cb = texture(s1, textureCoord).r - 0.5;\n"
                "            float cr = texture(s1, textureCoord).g - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        else if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            float y  = (texture(s0, textureCoord).r - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            float cb = (texture(s1, textureCoord).r - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            float cr = (texture(s1, textureCoord).g - (16.0 / 255.0)) * (255.0 / (240.0 - 16.0)) - 0.5;\n"
                "            c.r = y + (yuvCoefficients.x * cr);\n"
                "            c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "            c.b = y + (yuvCoefficients.w * cb);\n"
                "        }\n"
                "        c.r = clamp(c.r, 0.0, 1.0);\n"
                "        c.g = clamp(c.g, 0.0, 1.0);\n"
                "        c.b = clamp(c.b, 0.0, 1.0);\n"
                "        c.a = 1.0;\n"
                "    }\n"
                "    else\n"
                "    {\n"
                "        c = texture(s0, textureCoord);\n"
                "\n"
                "        // Video levels.\n"
                "        if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            c.r = (c.r - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            c.g = (c.g - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            c.b = (c.b - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "            if (ImageType_L_U8 == imageType ||\n"
                "                ImageType_L_U16 == imageType ||\n"
                "                ImageType_L_U32 == imageType ||\n"
                "                ImageType_LA_U8 == imageType ||\n"
                "                ImageType_LA_U16 == imageType ||\n"
                "                ImageType_LA_U32 == imageType ||\n"
                "                ImageType_RGB_U8 == imageType ||\n"
                "                ImageType_RGB_U10 == imageType ||\n"
                "                ImageType_RGB_U16 == imageType ||\n"
                "                ImageType_RGB_U32 == imageType ||\n"
                "                ImageType_RGBA_U8 == imageType ||\n"
                "                ImageType_RGBA_U16 == imageType ||\n"
                "                ImageType_RGBA_U32 == imageType)\n"
                "            {\n"
                "                c.r = clamp(c.r, 0.0, 1.0);\n"
                "                c.g = clamp(c.g, 0.0, 1.0);\n"
                "                c.b = clamp(c.b, 0.0, 1.0);\n"
                "            }\n"
                "        }\n"
                "\n"
                "        // Swizzle for the channel count.\n"
                "        if (1 == channelCount)\n"
                "        {\n"
                "            c.g = c.b = c.r;\n"
                "            c.a = 1.0;\n"
                "        }\n"
                "        else if (2 == channelCount)\n"
                "        {\n"
                "            c.a = c.g;\n"
                "            c.g = c.b = c.r;\n"
                "        }\n"
                "        else if (3 == channelCount)\n"
                "        {\n"
                "            c.a = 1.0;\n"
                "        }\n"
                "    }\n"
                "    return c;\n"
                "}\n";
        }

        namespace
        {
            // One tap of a separable resample. The contribution texture holds,
            // for each output pixel, the source coordinate and weight of every
            // tap: column = output pixel, row = tap. Built on the CPU because
            // it depends only on the two sizes and the kernel, not on the
            // picture, so a whole pass shares one table.
            const std::string scaleTap =
                "vec2 scaleTap(sampler2D contrib, float outCoord, int tap, int taps)\n"
                "{\n"
                "    float t = taps > 1 ?\n"
                "        (float(tap) / float(taps - 1)) :\n"
                "        0.0;\n"
                "    return texture(contrib, vec2(outCoord, t)).rg;\n"
                "}\n";
        }

        std::string textureScaleFragmentSource()
        {
            // One axis of a separable resample over an ordinary texture. Used
            // for both passes; which axis is a uniform, so this compiles once.
            return Format(
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "{0}\n"
                "\n"
                "uniform sampler2D textureSampler;\n"
                "uniform sampler2D scaleContrib;\n"
                "uniform int       scaleTaps;\n"
                "uniform bool      scaleVertical;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec4 c = vec4(0.0);\n"
                "    float outCoord = scaleVertical ? fTexture.y : fTexture.x;\n"
                "    for (int i = 0; i < scaleTaps; ++i)\n"
                "    {\n"
                "        vec2 tap = scaleTap(scaleContrib, outCoord, i, scaleTaps);\n"
                "        vec2 t = scaleVertical ?\n"
                "            vec2(fTexture.x, tap.x) :\n"
                "            vec2(tap.x, fTexture.y);\n"
                "        c += tap.y * texture(textureSampler, t);\n"
                "    }\n"
                "    outColor = c;\n"
                "}\n").
                arg(scaleTap);
        }

        std::string imageScaleXFragmentSource()
        {
            // Pass one: resample across, and convert the picture to RGBA on
            // the way so that the second pass has one plane to weigh rather
            // than three.
            return Format(
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "{0}\n"
                "\n"
                "{1}\n"
                "\n"
                "{2}\n"
                "\n"
                "{3}\n"
                "\n"
                "uniform int       imageType;\n"
                "uniform int       channelCount;\n"
                "uniform int       videoLevels;\n"
                "uniform vec4      yuvCoefficients;\n"
                "uniform int       mirrorX;\n"
                "uniform sampler2D textureSampler0;\n"
                "uniform sampler2D textureSampler1;\n"
                "uniform sampler2D textureSampler2;\n"
                "uniform sampler2D scaleContrib;\n"
                "uniform int       scaleTaps;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec4 c = vec4(0.0);\n"
                "    for (int i = 0; i < scaleTaps; ++i)\n"
                "    {\n"
                "        vec2 tap = scaleTap(scaleContrib, fTexture.x, i, scaleTaps);\n"
                "        vec2 t = vec2(tap.x, fTexture.y);\n"
                "        if (1 == mirrorX)\n"
                "        {\n"
                "            t.x = 1.0 - t.x;\n"
                "        }\n"
                "        c += tap.y * sampleTexture("
                "            t,\n"
                "            imageType,\n"
                "            channelCount,\n"
                "            videoLevels,\n"
                "            yuvCoefficients,\n"
                "            textureSampler0,\n"
                "            textureSampler1,\n"
                "            textureSampler2);\n"
                "    }\n"
                "    outColor = c;\n"
                "}\n").
                arg(imageType).
                arg(videoLevels).
                arg(sampleTexture).
                arg(scaleTap);
        }

        std::string imageScaleYFragmentSource()
        {
            // Pass two: resample down the other axis, and apply what the image
            // options say about the result -- which is done here rather than in
            // the first pass because a channel shown on its own, or an alpha
            // forced opaque, is a statement about the finished pixel.
            return Format(
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "{0}\n"
                "\n"
                "{1}\n"
                "\n"
                "uniform vec4      color;\n"
                "uniform bool      opaque;\n"
                "uniform int       channelDisplay;\n"
                "uniform int       mirrorY;\n"
                "uniform sampler2D textureSampler0;\n"
                "uniform sampler2D scaleContrib;\n"
                "uniform int       scaleTaps;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec4 c = vec4(0.0);\n"
                "    for (int i = 0; i < scaleTaps; ++i)\n"
                "    {\n"
                "        vec2 tap = scaleTap(scaleContrib, fTexture.y, i, scaleTaps);\n"
                "        float y = tap.x;\n"
                // Drawing the first pass into a texture turns it over once
                // more than the single pass path, so this reads the opposite
                // way round to the image shader.
                "        if (1 == mirrorY)\n"
                "        {\n"
                "            y = 1.0 - y;\n"
                "        }\n"
                "        c += tap.y * texture(textureSampler0, vec2(fTexture.x, y));\n"
                "    }\n"
                "    outColor = c * color;\n"
                "    if (opaque)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "    if (ChannelDisplay_Red == channelDisplay)\n"
                "    {\n"
                "        outColor.g = outColor.b = outColor.r;\n"
                "    }\n"
                "    else if (ChannelDisplay_Green == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.b = outColor.g;\n"
                "    }\n"
                "    else if (ChannelDisplay_Blue == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b;\n"
                "    }\n"
                "    else if (ChannelDisplay_Alpha == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b = outColor.a;\n"
                "    }\n"
                "}\n").
                arg(channelDisplay).
                arg(scaleTap);
        }

        std::string imageFragmentSource()
        {
            return Format(
                "#version 410\n"
                "\n"
                "in vec2 fTexture;\n"
                "out vec4 outColor;\n"
                "\n"
                "{0}\n"
                "\n"
                "{1}\n"
                "\n"
                "{2}\n"
                "\n"
                "{3}\n"
                "\n"
                "uniform vec4      color;\n"
                "uniform bool      opaque;\n"
                "uniform int       imageType;\n"
                "uniform int       channelCount;\n"
                "uniform int       channelDisplay;\n"
                "uniform int       videoLevels;\n"
                "uniform vec4      yuvCoefficients;\n"
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
                "    outColor = sampleTexture("
                "        t,\n"
                "        imageType,\n"
                "        channelCount,\n"
                "        videoLevels,\n"
                "        yuvCoefficients,\n"
                "        textureSampler0,\n"
                "        textureSampler1,\n"
                "        textureSampler2) *\n"
                "        color;\n"
                "    if (opaque)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "    if (ChannelDisplay_Red == channelDisplay)\n"
                "    {\n"
                "        outColor.g = outColor.b = outColor.r;\n"
                "    }\n"
                "    else if (ChannelDisplay_Green == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.b = outColor.g;\n"
                "    }\n"
                "    else if (ChannelDisplay_Blue == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b;\n"
                "    }\n"
                "    else if (ChannelDisplay_Alpha == channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b = outColor.a;\n"
                "    }\n"
                "}\n").
                arg(imageType).
                arg(channelDisplay).
                arg(videoLevels).
                arg(sampleTexture);
        }
    }
}

