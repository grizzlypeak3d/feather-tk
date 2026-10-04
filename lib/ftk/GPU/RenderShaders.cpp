// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/RenderPrivate.h>

namespace ftk
{
    namespace gpu
    {
        // These say what the OpenGL renderer's shaders say; see
        // ftk/GL/RenderShaders.cpp, which is where the reasons are.
        //
        // A stage is compiled on its own, so what passes from one to the
        // next is matched by its user() attribute. Uniforms are a block in
        // buffer zero of the stage, and textures and samplers go by index.

        namespace
        {
            const std::string header =
                "#include <metal_stdlib>\n"
                "using namespace metal;\n"
                "\n";
        }

        std::string vertexSourceMSL()
        {
            return header +
                "struct VertexIn\n"
                "{\n"
                "    float2 pos [[attribute(0)]];\n"
                "    float2 uv [[attribute(1)]];\n"
                "};\n"
                "\n"
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float2 uv [[user(locn0)]];\n"
                "};\n"
                "\n"
                "struct Uniforms\n"
                "{\n"
                "    float4x4 mvp;\n"
                "};\n"
                "\n"
                "vertex VertexOut vertexMain(\n"
                "    VertexIn in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]])\n"
                "{\n"
                "    VertexOut out;\n"
                "    out.position = u.mvp * float4(in.pos, 0.0, 1.0);\n"
                "    out.uv = in.uv;\n"
                "    return out;\n"
                "}\n";
        }

        std::string colorMeshVertexSourceMSL()
        {
            return header +
                "struct VertexIn\n"
                "{\n"
                "    float2 pos [[attribute(0)]];\n"
                "    float4 color [[attribute(1)]];\n"
                "};\n"
                "\n"
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float4 color [[user(locn0)]];\n"
                "};\n"
                "\n"
                "struct Uniforms\n"
                "{\n"
                "    float4x4 mvp;\n"
                "};\n"
                "\n"
                "vertex VertexOut vertexMain(\n"
                "    VertexIn in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]])\n"
                "{\n"
                "    VertexOut out;\n"
                "    out.position = u.mvp * float4(in.pos, 0.0, 1.0);\n"
                "    out.color = in.color;\n"
                "    return out;\n"
                "}\n";
        }

        namespace
        {
            const std::string fragmentIn =
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float2 uv [[user(locn0)]];\n"
                "};\n"
                "\n";
        }

        std::string meshFragmentSourceMSL()
        {
            return header + fragmentIn +
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]])\n"
                "{\n"
                "    return u.color;\n"
                "}\n";
        }

        std::string colorMeshFragmentSourceMSL()
        {
            return header +
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float4 color [[user(locn0)]];\n"
                "};\n"
                "\n"
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]])\n"
                "{\n"
                "    return in.color * u.color;\n"
                "}\n";
        }

        std::string textureFragmentSourceMSL()
        {
            return header + fragmentIn +
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "    int opaque;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    sampler s0 [[sampler(0)]])\n"
                "{\n"
                "    float4 outColor = t0.sample(s0, in.uv) * u.color;\n"
                "    if (u.opaque != 0)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "    return outColor;\n"
                "}\n";
        }

        std::string textFragmentSourceMSL()
        {
            return header + fragmentIn +
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    sampler s0 [[sampler(0)]])\n"
                "{\n"
                "    float4 outColor;\n"
                "    outColor.rgb = u.color.rgb;\n"
                "    float coverage = t0.sample(s0, in.uv).r * u.color.a;\n"
                "    float gamma = 1.3;\n"
                "    outColor.a = pow(coverage, 1.0 / gamma);\n"
                "    return outColor;\n"
                "}\n";
        }

        namespace
        {
            const std::string imageType =
                "// enum ftk::ImageType\n"
                "constant int ImageType_None              = 0;\n"
                "constant int ImageType_L_U8              = 1;\n"
                "constant int ImageType_L_U16             = 2;\n"
                "constant int ImageType_L_U32             = 3;\n"
                "constant int ImageType_L_F16             = 4;\n"
                "constant int ImageType_L_F32             = 5;\n"
                "constant int ImageType_LA_U8             = 6;\n"
                "constant int ImageType_LA_U32            = 7;\n"
                "constant int ImageType_LA_U16            = 8;\n"
                "constant int ImageType_LA_F16            = 9;\n"
                "constant int ImageType_LA_F32            = 10;\n"
                "constant int ImageType_RGB_U8            = 11;\n"
                "constant int ImageType_RGB_U10           = 12;\n"
                "constant int ImageType_RGB_U16           = 13;\n"
                "constant int ImageType_RGB_U32           = 14;\n"
                "constant int ImageType_RGB_F16           = 15;\n"
                "constant int ImageType_RGB_F32           = 16;\n"
                "constant int ImageType_RGBA_U8           = 17;\n"
                "constant int ImageType_RGBA_U16          = 18;\n"
                "constant int ImageType_RGBA_U32          = 19;\n"
                "constant int ImageType_RGBA_F16          = 20;\n"
                "constant int ImageType_RGBA_F32          = 21;\n"
                "constant int ImageType_YUV_420P_U8       = 22;\n"
                "constant int ImageType_YUV_422P_U8       = 23;\n"
                "constant int ImageType_YUV_444P_U8       = 24;\n"
                "constant int ImageType_YUV_420P_U16      = 25;\n"
                "constant int ImageType_YUV_422P_U16      = 26;\n"
                "constant int ImageType_YUV_444P_U16      = 27;\n"
                "constant int ImageType_YUV_420SP_U8      = 28;\n"
                "constant int ImageType_YUV_420SP_U16     = 29;\n"
                "constant int ImageType_RGB_F16_P         = 30;\n"
                "\n"
                "// enum ftk::ChannelDisplay\n"
                "constant int ChannelDisplay_Color = 0;\n"
                "constant int ChannelDisplay_Red   = 1;\n"
                "constant int ChannelDisplay_Green = 2;\n"
                "constant int ChannelDisplay_Blue  = 3;\n"
                "constant int ChannelDisplay_Alpha = 4;\n"
                "\n"
                "// enum ftk::VideoLevels\n"
                "constant int VideoLevels_FullRange  = 0;\n"
                "constant int VideoLevels_LegalRange = 1;\n"
                "\n";

            const std::string sampleTexture =
                "float codeScale(int imageType)\n"
                "{\n"
                "    float scale = 255.0;\n"
                "    if (ImageType_YUV_420P_U16 == imageType ||\n"
                "        ImageType_YUV_422P_U16 == imageType ||\n"
                "        ImageType_YUV_444P_U16 == imageType ||\n"
                "        ImageType_YUV_420SP_U16 == imageType)\n"
                "    {\n"
                "        scale = 65535.0 / 256.0;\n"
                "    }\n"
                "    return scale;\n"
                "}\n"
                "\n"
                "float4 sampleTexture(\n"
                "    float2 uv,\n"
                "    int imageType,\n"
                "    int channelCount,\n"
                "    int videoLevels,\n"
                "    float4 yuvCoefficients,\n"
                "    texture2d<float> t0,\n"
                "    texture2d<float> t1,\n"
                "    texture2d<float> t2,\n"
                "    sampler s0,\n"
                "    sampler s1,\n"
                "    sampler s2)\n"
                "{\n"
                "    float4 c = float4(0.0);\n"
                "    if (ImageType_YUV_420P_U8 == imageType ||\n"
                "        ImageType_YUV_422P_U8 == imageType ||\n"
                "        ImageType_YUV_444P_U8 == imageType ||\n"
                "        ImageType_YUV_420P_U16 == imageType ||\n"
                "        ImageType_YUV_422P_U16 == imageType ||\n"
                "        ImageType_YUV_444P_U16 == imageType ||\n"
                "        ImageType_YUV_420SP_U8 == imageType ||\n"
                "        ImageType_YUV_420SP_U16 == imageType)\n"
                "    {\n"
                "        // The interleaved chroma of the NV12 types is one plane\n"
                "        // of two channels, and the others have a plane of each.\n"
                "        bool sp =\n"
                "            ImageType_YUV_420SP_U8 == imageType ||\n"
                "            ImageType_YUV_420SP_U16 == imageType;\n"
                "        float y = t0.sample(s0, uv).r;\n"
                "        float cb = t1.sample(s1, uv).r;\n"
                "        float cr = sp ? t1.sample(s1, uv).g : t2.sample(s2, uv).r;\n"
                "        float s = codeScale(imageType);\n"
                "        if (VideoLevels_FullRange == videoLevels)\n"
                "        {\n"
                "            cb -= 128.0 / s;\n"
                "            cr -= 128.0 / s;\n"
                "        }\n"
                "        else\n"
                "        {\n"
                "            y  = (y * s - 16.0) / (235.0 - 16.0);\n"
                "            cb = (cb * s - 16.0) / (240.0 - 16.0) - 0.5;\n"
                "            cr = (cr * s - 16.0) / (240.0 - 16.0) - 0.5;\n"
                "        }\n"
                "        c.r = y + (yuvCoefficients.x * cr);\n"
                "        c.g = y - (yuvCoefficients.y * cr) - (yuvCoefficients.z * cb);\n"
                "        c.b = y + (yuvCoefficients.w * cb);\n"
                "        c.rgb = clamp(c.rgb, 0.0, 1.0);\n"
                "        c.a = 1.0;\n"
                "    }\n"
                "    else if (ImageType_RGB_F16_P == imageType)\n"
                "    {\n"
                "        c.r = t0.sample(s0, uv).r;\n"
                "        c.g = t1.sample(s1, uv).r;\n"
                "        c.b = t2.sample(s2, uv).r;\n"
                "        c.a = 1.0;\n"
                "        if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            c.rgb = (c.rgb - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
                "        }\n"
                "    }\n"
                "    else\n"
                "    {\n"
                "        c = t0.sample(s0, uv);\n"
                "        if (VideoLevels_LegalRange == videoLevels)\n"
                "        {\n"
                "            c.rgb = (c.rgb - (16.0 / 255.0)) * (255.0 / (235.0 - 16.0));\n"
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
                "                c.rgb = clamp(c.rgb, 0.0, 1.0);\n"
                "            }\n"
                "        }\n"
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
                "}\n"
                "\n";
        }

        std::string imageFragmentSourceMSL()
        {
            return header + fragmentIn + imageType + sampleTexture +
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "    float4 yuvCoefficients;\n"
                "    int opaque;\n"
                "    int imageType;\n"
                "    int channelCount;\n"
                "    int channelDisplay;\n"
                "    int videoLevels;\n"
                "    int mirrorX;\n"
                "    int mirrorY;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    texture2d<float> t1 [[texture(1)]],\n"
                "    texture2d<float> t2 [[texture(2)]],\n"
                "    sampler s0 [[sampler(0)]],\n"
                "    sampler s1 [[sampler(1)]],\n"
                "    sampler s2 [[sampler(2)]])\n"
                "{\n"
                "    float2 t = in.uv;\n"
                "    if (1 == u.mirrorX)\n"
                "    {\n"
                "        t.x = 1.0 - t.x;\n"
                "    }\n"
                "    if (0 == u.mirrorY)\n"
                "    {\n"
                "        t.y = 1.0 - t.y;\n"
                "    }\n"
                "    float4 outColor = sampleTexture(\n"
                "        t,\n"
                "        u.imageType,\n"
                "        u.channelCount,\n"
                "        u.videoLevels,\n"
                "        u.yuvCoefficients,\n"
                "        t0, t1, t2, s0, s1, s2) *\n"
                "        u.color;\n"
                "    if (u.opaque != 0)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "    if (ChannelDisplay_Red == u.channelDisplay)\n"
                "    {\n"
                "        outColor.g = outColor.b = outColor.r;\n"
                "    }\n"
                "    else if (ChannelDisplay_Green == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.b = outColor.g;\n"
                "    }\n"
                "    else if (ChannelDisplay_Blue == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b;\n"
                "    }\n"
                "    else if (ChannelDisplay_Alpha == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b = outColor.a;\n"
                "    }\n"
                "    return outColor;\n"
                "}\n";
        }

        namespace
        {
            // One tap of a separable resample; see getScaleContrib().
            const std::string scaleTap =
                "float2 scaleTap(texture2d<float> contrib, sampler cs, float outCoord, int tap, int taps)\n"
                "{\n"
                "    float t = taps > 1 ?\n"
                "        (float(tap) / float(taps - 1)) :\n"
                "        0.0;\n"
                "    return contrib.sample(cs, float2(outCoord, t)).rg;\n"
                "}\n"
                "\n";

            const std::string channelDisplaySwizzle =
                "    if (u.opaque != 0)\n"
                "    {\n"
                "        outColor.a = 1.0;\n"
                "    }\n"
                "    if (ChannelDisplay_Red == u.channelDisplay)\n"
                "    {\n"
                "        outColor.g = outColor.b = outColor.r;\n"
                "    }\n"
                "    else if (ChannelDisplay_Green == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.b = outColor.g;\n"
                "    }\n"
                "    else if (ChannelDisplay_Blue == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b;\n"
                "    }\n"
                "    else if (ChannelDisplay_Alpha == u.channelDisplay)\n"
                "    {\n"
                "        outColor.r = outColor.g = outColor.b = outColor.a;\n"
                "    }\n";
        }

        std::string textureScaleFragmentSourceMSL()
        {
            // One axis of a separable resample over an ordinary texture.
            // Used for both passes; which axis is a uniform.
            return header + fragmentIn + scaleTap +
                "struct Uniforms\n"
                "{\n"
                "    int scaleTaps;\n"
                "    int scaleVertical;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    texture2d<float> t1 [[texture(1)]],\n"
                "    sampler s0 [[sampler(0)]],\n"
                "    sampler s1 [[sampler(1)]])\n"
                "{\n"
                "    float4 c = float4(0.0);\n"
                "    float4 lo = float4(1.0e38);\n"
                "    float4 hi = float4(-1.0e38);\n"
                "    bool range = false;\n"
                "    float outCoord = u.scaleVertical != 0 ? in.uv.y : in.uv.x;\n"
                "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                "    {\n"
                "        float2 tap = scaleTap(t1, s1, outCoord, i, u.scaleTaps);\n"
                "        float2 t = u.scaleVertical != 0 ?\n"
                "            float2(in.uv.x, tap.x) :\n"
                "            float2(tap.x, in.uv.y);\n"
                "        float4 texel = t0.sample(s0, t);\n"
                "        c += tap.y * texel;\n"
                "        if (tap.y > 0.0)\n"
                "        {\n"
                "            lo = min(lo, texel);\n"
                "            hi = max(hi, texel);\n"
                "            range = true;\n"
                "        }\n"
                "    }\n"
                "    // Held to the range of the texels the kernel weighs up; see\n"
                "    // the OpenGL renderer.\n"
                "    return range ? clamp(c, lo, hi) : c;\n"
                "}\n";
        }

        std::string imageScaleXFragmentSourceMSL()
        {
            // Pass one: resample across, and convert the picture to RGBA on
            // the way.
            return header + fragmentIn + imageType + sampleTexture + scaleTap +
                "struct Uniforms\n"
                "{\n"
                "    float4 yuvCoefficients;\n"
                "    int imageType;\n"
                "    int channelCount;\n"
                "    int videoLevels;\n"
                "    int mirrorX;\n"
                "    int scaleTaps;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    texture2d<float> t1 [[texture(1)]],\n"
                "    texture2d<float> t2 [[texture(2)]],\n"
                "    texture2d<float> t3 [[texture(3)]],\n"
                "    sampler s0 [[sampler(0)]],\n"
                "    sampler s1 [[sampler(1)]],\n"
                "    sampler s2 [[sampler(2)]],\n"
                "    sampler s3 [[sampler(3)]])\n"
                "{\n"
                "    float4 c = float4(0.0);\n"
                "    float4 lo = float4(1.0e38);\n"
                "    float4 hi = float4(-1.0e38);\n"
                "    bool range = false;\n"
                "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                "    {\n"
                "        float2 tap = scaleTap(t3, s3, in.uv.x, i, u.scaleTaps);\n"
                "        float2 t = float2(tap.x, in.uv.y);\n"
                "        if (1 == u.mirrorX)\n"
                "        {\n"
                "            t.x = 1.0 - t.x;\n"
                "        }\n"
                "        float4 texel = sampleTexture(\n"
                "            t,\n"
                "            u.imageType,\n"
                "            u.channelCount,\n"
                "            u.videoLevels,\n"
                "            u.yuvCoefficients,\n"
                "            t0, t1, t2, s0, s1, s2);\n"
                "        c += tap.y * texel;\n"
                "        if (tap.y > 0.0)\n"
                "        {\n"
                "            lo = min(lo, texel);\n"
                "            hi = max(hi, texel);\n"
                "            range = true;\n"
                "        }\n"
                "    }\n"
                "    return range ? clamp(c, lo, hi) : c;\n"
                "}\n";
        }

        std::string imageScaleYFragmentSourceMSL()
        {
            // Pass two: resample down, and apply what the image options say
            // about the finished pixel. The first pass keeps the image's
            // rows as they are in memory, so this turns them over as the
            // image shader does.
            return header + fragmentIn + imageType + scaleTap +
                "struct Uniforms\n"
                "{\n"
                "    float4 color;\n"
                "    int opaque;\n"
                "    int channelDisplay;\n"
                "    int mirrorY;\n"
                "    int scaleTaps;\n"
                "};\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    texture2d<float> t1 [[texture(1)]],\n"
                "    sampler s0 [[sampler(0)]],\n"
                "    sampler s1 [[sampler(1)]])\n"
                "{\n"
                "    float4 c = float4(0.0);\n"
                "    float4 lo = float4(1.0e38);\n"
                "    float4 hi = float4(-1.0e38);\n"
                "    bool range = false;\n"
                "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                "    {\n"
                "        float2 tap = scaleTap(t1, s1, in.uv.y, i, u.scaleTaps);\n"
                "        float y = tap.x;\n"
                "        if (0 == u.mirrorY)\n"
                "        {\n"
                "            y = 1.0 - y;\n"
                "        }\n"
                "        float4 texel = t0.sample(s0, float2(in.uv.x, y));\n"
                "        c += tap.y * texel;\n"
                "        if (tap.y > 0.0)\n"
                "        {\n"
                "            lo = min(lo, texel);\n"
                "            hi = max(hi, texel);\n"
                "            range = true;\n"
                "        }\n"
                "    }\n"
                "    float4 outColor = (range ? clamp(c, lo, hi) : c) * u.color;\n" +
                channelDisplaySwizzle +
                "    return outColor;\n"
                "}\n";
        }

        namespace
        {
            const std::string fragmentHeaderGLSL =
                "#version 450\n"
                "\n"
                "layout(location = 0) in vec2 fTexture;\n"
                "layout(location = 0) out vec4 outColor;\n"
                "\n";

            std::string vertexSourceGLSL()
            {
                return
                    "#version 450\n"
                    "\n"
                    "layout(location = 0) in vec2 vPos;\n"
                    "layout(location = 1) in vec2 vTexture;\n"
                    "layout(location = 0) out vec2 fTexture;\n"
                    "\n"
                    "layout(set = 1, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    mat4 mvp;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    gl_Position = u.mvp * vec4(vPos, 0.0, 1.0);\n"
                    "    fTexture = vTexture;\n"
                    "}\n";
            }

            std::string colorMeshVertexSourceGLSL()
            {
                return
                    "#version 450\n"
                    "\n"
                    "layout(location = 0) in vec2 vPos;\n"
                    "layout(location = 1) in vec4 vColor;\n"
                    "layout(location = 0) out vec4 fColor;\n"
                    "\n"
                    "layout(set = 1, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    mat4 mvp;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    gl_Position = u.mvp * vec4(vPos, 0.0, 1.0);\n"
                    "    fColor = vColor;\n"
                    "}\n";
            }

            std::string meshFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    outColor = u.color;\n"
                    "}\n";
            }

            std::string colorMeshFragmentSourceGLSL()
            {
                return
                    "#version 450\n"
                    "\n"
                    "layout(location = 0) in vec4 fColor;\n"
                    "layout(location = 0) out vec4 outColor;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    outColor = fColor * u.color;\n"
                    "}\n";
            }

            std::string textureFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "    int opaque;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    outColor = texture(s0, fTexture) * u.color;\n"
                    "    if (u.opaque != 0)\n"
                    "    {\n"
                    "        outColor.a = 1.0;\n"
                    "    }\n"
                    "}\n";
            }

            std::string textFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    outColor.rgb = u.color.rgb;\n"
                    "    float coverage = texture(s0, fTexture).r * u.color.a;\n"
                    "    float gamma = 1.3;\n"
                    "    outColor.a = pow(coverage, 1.0 / gamma);\n"
                    "}\n";
            }

            // What the Metal source declares "constant int" and takes
            // textures and samplers apart for, here: "const int", and a
            // sampler that is both.
            std::string toGLSL(std::string value)
            {
                const auto replace = [&value](const std::string& a, const std::string& b)
                {
                    size_t i = 0;
                    while ((i = value.find(a, i)) != std::string::npos)
                    {
                        value.replace(i, a.size(), b);
                        i += b.size();
                    }
                };
                replace("constant int", "const int");
                replace("float4", "vec4");
                replace("float3", "vec3");
                replace("float2", "vec2");
                replace("t0.sample(s0, uv)", "texture(s0, uv)");
                replace("t1.sample(s1, uv)", "texture(s1, uv)");
                replace("t2.sample(s2, uv)", "texture(s2, uv)");
                replace(
                    "    texture2d<float> t0,\n"
                    "    texture2d<float> t1,\n"
                    "    texture2d<float> t2,\n"
                    "    sampler s0,\n"
                    "    sampler s1,\n"
                    "    sampler s2)",
                    "    sampler2D s0,\n"
                    "    sampler2D s1,\n"
                    "    sampler2D s2)");
                return value;
            }

            std::string imageFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    toGLSL(imageType) +
                    toGLSL(sampleTexture) +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "layout(set = 2, binding = 1) uniform sampler2D s1;\n"
                    "layout(set = 2, binding = 2) uniform sampler2D s2;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "    vec4 yuvCoefficients;\n"
                    "    int opaque;\n"
                    "    int imageType;\n"
                    "    int channelCount;\n"
                    "    int channelDisplay;\n"
                    "    int videoLevels;\n"
                    "    int mirrorX;\n"
                    "    int mirrorY;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    vec2 t = fTexture;\n"
                    "    if (1 == u.mirrorX)\n"
                    "    {\n"
                    "        t.x = 1.0 - t.x;\n"
                    "    }\n"
                    "    if (0 == u.mirrorY)\n"
                    "    {\n"
                    "        t.y = 1.0 - t.y;\n"
                    "    }\n"
                    "    outColor = sampleTexture(\n"
                    "        t,\n"
                    "        u.imageType,\n"
                    "        u.channelCount,\n"
                    "        u.videoLevels,\n"
                    "        u.yuvCoefficients,\n"
                    "        s0, s1, s2) *\n"
                    "        u.color;\n"
                    "    if (u.opaque != 0)\n"
                    "    {\n"
                    "        outColor.a = 1.0;\n"
                    "    }\n"
                    "    if (ChannelDisplay_Red == u.channelDisplay)\n"
                    "    {\n"
                    "        outColor.g = outColor.b = outColor.r;\n"
                    "    }\n"
                    "    else if (ChannelDisplay_Green == u.channelDisplay)\n"
                    "    {\n"
                    "        outColor.r = outColor.b = outColor.g;\n"
                    "    }\n"
                    "    else if (ChannelDisplay_Blue == u.channelDisplay)\n"
                    "    {\n"
                    "        outColor.r = outColor.g = outColor.b;\n"
                    "    }\n"
                    "    else if (ChannelDisplay_Alpha == u.channelDisplay)\n"
                    "    {\n"
                    "        outColor.r = outColor.g = outColor.b = outColor.a;\n"
                    "    }\n"
                    "}\n";
            }
        }

        namespace
        {
            const std::string scaleTapGLSL =
                "vec2 scaleTap(sampler2D contrib, float outCoord, int tap, int taps)\n"
                "{\n"
                "    float t = taps > 1 ?\n"
                "        (float(tap) / float(taps - 1)) :\n"
                "        0.0;\n"
                "    return texture(contrib, vec2(outCoord, t)).rg;\n"
                "}\n"
                "\n";

            std::string textureScaleFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL + scaleTapGLSL +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "layout(set = 2, binding = 1) uniform sampler2D s1;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    int scaleTaps;\n"
                    "    int scaleVertical;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    vec4 c = vec4(0.0);\n"
                    "    vec4 lo = vec4(1.0e38);\n"
                    "    vec4 hi = vec4(-1.0e38);\n"
                    "    bool range = false;\n"
                    "    float outCoord = u.scaleVertical != 0 ? fTexture.y : fTexture.x;\n"
                    "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                    "    {\n"
                    "        vec2 tap = scaleTap(s1, outCoord, i, u.scaleTaps);\n"
                    "        vec2 t = u.scaleVertical != 0 ?\n"
                    "            vec2(fTexture.x, tap.x) :\n"
                    "            vec2(tap.x, fTexture.y);\n"
                    "        vec4 texel = texture(s0, t);\n"
                    "        c += tap.y * texel;\n"
                    "        if (tap.y > 0.0)\n"
                    "        {\n"
                    "            lo = min(lo, texel);\n"
                    "            hi = max(hi, texel);\n"
                    "            range = true;\n"
                    "        }\n"
                    "    }\n"
                    "    outColor = range ? clamp(c, lo, hi) : c;\n"
                    "}\n";
            }

            std::string imageScaleXFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    toGLSL(imageType) +
                    toGLSL(sampleTexture) +
                    scaleTapGLSL +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "layout(set = 2, binding = 1) uniform sampler2D s1;\n"
                    "layout(set = 2, binding = 2) uniform sampler2D s2;\n"
                    "layout(set = 2, binding = 3) uniform sampler2D s3;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 yuvCoefficients;\n"
                    "    int imageType;\n"
                    "    int channelCount;\n"
                    "    int videoLevels;\n"
                    "    int mirrorX;\n"
                    "    int scaleTaps;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    vec4 c = vec4(0.0);\n"
                    "    vec4 lo = vec4(1.0e38);\n"
                    "    vec4 hi = vec4(-1.0e38);\n"
                    "    bool range = false;\n"
                    "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                    "    {\n"
                    "        vec2 tap = scaleTap(s3, fTexture.x, i, u.scaleTaps);\n"
                    "        vec2 t = vec2(tap.x, fTexture.y);\n"
                    "        if (1 == u.mirrorX)\n"
                    "        {\n"
                    "            t.x = 1.0 - t.x;\n"
                    "        }\n"
                    "        vec4 texel = sampleTexture(\n"
                    "            t,\n"
                    "            u.imageType,\n"
                    "            u.channelCount,\n"
                    "            u.videoLevels,\n"
                    "            u.yuvCoefficients,\n"
                    "            s0, s1, s2);\n"
                    "        c += tap.y * texel;\n"
                    "        if (tap.y > 0.0)\n"
                    "        {\n"
                    "            lo = min(lo, texel);\n"
                    "            hi = max(hi, texel);\n"
                    "            range = true;\n"
                    "        }\n"
                    "    }\n"
                    "    outColor = range ? clamp(c, lo, hi) : c;\n"
                    "}\n";
            }

            std::string imageScaleYFragmentSourceGLSL()
            {
                return fragmentHeaderGLSL +
                    toGLSL(imageType) +
                    scaleTapGLSL +
                    "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                    "layout(set = 2, binding = 1) uniform sampler2D s1;\n"
                    "\n"
                    "layout(set = 3, binding = 0) uniform Uniforms\n"
                    "{\n"
                    "    vec4 color;\n"
                    "    int opaque;\n"
                    "    int channelDisplay;\n"
                    "    int mirrorY;\n"
                    "    int scaleTaps;\n"
                    "} u;\n"
                    "\n"
                    "void main()\n"
                    "{\n"
                    "    vec4 c = vec4(0.0);\n"
                    "    vec4 lo = vec4(1.0e38);\n"
                    "    vec4 hi = vec4(-1.0e38);\n"
                    "    bool range = false;\n"
                    "    for (int i = 0; i < u.scaleTaps; ++i)\n"
                    "    {\n"
                    "        vec2 tap = scaleTap(s1, fTexture.y, i, u.scaleTaps);\n"
                    "        float y = tap.x;\n"
                    "        if (0 == u.mirrorY)\n"
                    "        {\n"
                    "            y = 1.0 - y;\n"
                    "        }\n"
                    "        vec4 texel = texture(s0, vec2(fTexture.x, y));\n"
                    "        c += tap.y * texel;\n"
                    "        if (tap.y > 0.0)\n"
                    "        {\n"
                    "            lo = min(lo, texel);\n"
                    "            hi = max(hi, texel);\n"
                    "            range = true;\n"
                    "        }\n"
                    "    }\n"
                    "    outColor = (range ? clamp(c, lo, hi) : c) * u.color;\n" +
                    channelDisplaySwizzle +
                    "}\n";
            }
        }

        ShaderSource textureScaleFragmentSource()
        {
            return { textureScaleFragmentSourceMSL(), textureScaleFragmentSourceGLSL() };
        }

        ShaderSource imageScaleXFragmentSource()
        {
            return { imageScaleXFragmentSourceMSL(), imageScaleXFragmentSourceGLSL() };
        }

        ShaderSource imageScaleYFragmentSource()
        {
            return { imageScaleYFragmentSourceMSL(), imageScaleYFragmentSourceGLSL() };
        }

        ShaderSource vertexSource()
        {
            return { vertexSourceMSL(), vertexSourceGLSL() };
        }

        ShaderSource colorMeshVertexSource()
        {
            return { colorMeshVertexSourceMSL(), colorMeshVertexSourceGLSL() };
        }

        ShaderSource meshFragmentSource()
        {
            return { meshFragmentSourceMSL(), meshFragmentSourceGLSL() };
        }

        ShaderSource colorMeshFragmentSource()
        {
            return { colorMeshFragmentSourceMSL(), colorMeshFragmentSourceGLSL() };
        }

        ShaderSource textureFragmentSource()
        {
            return { textureFragmentSourceMSL(), textureFragmentSourceGLSL() };
        }

        ShaderSource textFragmentSource()
        {
            return { textFragmentSourceMSL(), textFragmentSourceGLSL() };
        }

        ShaderSource imageFragmentSource()
        {
            return { imageFragmentSourceMSL(), imageFragmentSourceGLSL() };
        }
    }
}
