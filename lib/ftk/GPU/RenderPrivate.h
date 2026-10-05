// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/Render.h>

#include <ftk/GPU/OffscreenBuffer.h>
#include <ftk/GPU/System.h>
#include <ftk/GPU/Texture.h>

#include <ftk/Core/BoxPack.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/LRUCache.h>

#include <SDL3/SDL.h>

#include <array>
#include <chrono>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>

namespace ftk
{
    namespace gpu
    {
        //! \name Shaders
        ///@{

        std::string vertexSourceMSL();
        std::string colorMeshVertexSourceMSL();
        std::string meshFragmentSourceMSL();
        std::string colorMeshFragmentSourceMSL();
        std::string textureFragmentSourceMSL();
        std::string textFragmentSourceMSL();
        std::string imageFragmentSourceMSL();
        std::string textureScaleFragmentSourceMSL();
        std::string imageScaleXFragmentSourceMSL();
        std::string imageScaleYFragmentSourceMSL();

        ShaderSource vertexSource();
        ShaderSource colorMeshVertexSource();
        ShaderSource meshFragmentSource();
        ShaderSource colorMeshFragmentSource();
        ShaderSource textureFragmentSource();
        ShaderSource textFragmentSource();
        ShaderSource imageFragmentSource();
        ShaderSource textureScaleFragmentSource();
        ShaderSource imageScaleXFragmentSource();
        ShaderSource imageScaleYFragmentSource();

        ///@}

        //! What a vertex holds besides its position.
        enum class VertexType
        {
            UV,
            Color
        };

        struct VertexUV
        {
            float x = 0.F;
            float y = 0.F;
            float u = 0.F;
            float v = 0.F;
        };

        struct VertexColor
        {
            float x = 0.F;
            float y = 0.F;
            float r = 1.F;
            float g = 1.F;
            float b = 1.F;
            float a = 1.F;
        };

        //! \name Uniforms
        //! Laid out as the shaders declare them.
        ///@{

        struct ColorUniforms
        {
            float color[4];
        };

        struct TextureUniforms
        {
            float color[4];
            int32_t opaque = 0;
            int32_t pad[3] = { 0, 0, 0 };
        };

        struct ImageUniforms
        {
            float color[4];
            float yuvCoefficients[4];
            int32_t opaque = 0;
            int32_t imageType = 0;
            int32_t channelCount = 0;
            int32_t channelDisplay = 0;
            int32_t videoLevels = 0;
            int32_t mirrorX = 0;
            int32_t mirrorY = 0;
            int32_t pad = 0;
        };

        struct TextureScaleUniforms
        {
            int32_t scaleTaps = 0;
            int32_t scaleVertical = 0;
            int32_t pad[2] = { 0, 0 };
        };

        struct ImageScaleXUniforms
        {
            float yuvCoefficients[4];
            int32_t imageType = 0;
            int32_t channelCount = 0;
            int32_t videoLevels = 0;
            int32_t mirrorX = 0;
            int32_t scaleTaps = 0;
            int32_t pad[3] = { 0, 0, 0 };
        };

        struct ImageScaleYUniforms
        {
            float color[4];
            int32_t opaque = 0;
            int32_t channelDisplay = 0;
            int32_t mirrorY = 0;
            int32_t scaleTaps = 0;
        };

        ///@}

        struct Render::Private
        {
            std::shared_ptr<System> system;
            SDL_GPUDevice* device = nullptr;

            Size2I size;
            RenderOptions options;
            Box2I viewport;
            bool clipRectEnabled = false;
            Box2I clipRect;
            M44F transform;

            std::shared_ptr<OffscreenBuffer> target;
            struct TargetState
            {
                std::shared_ptr<OffscreenBuffer> target;
                Box2I viewport;
                bool clipRectEnabled = false;
                bool clearPending = false;
                Color4F clearColor;
            };
            std::vector<TargetState> targets;
            SDL_GPUCommandBuffer* cmd = nullptr;
            bool clearPending = false;
            Color4F clearColor;
            bool blendEnabled = true;

            // A pass is kept, not written, until it ends: what it draws,
            // and the textures made ready for it. It is then written into
            // the frame's command buffer after what it draws with, the
            // vertices and those textures, so that the one command buffer
            // holds both in the order they are needed. Sent in command
            // buffers of their own ahead of the one that drew with them,
            // they depended on the driver finishing one before starting
            // the next, which the Raspberry Pi's does not.
            struct DrawCmd
            {
                SDL_GPUGraphicsPipeline* pipeline = nullptr;
                size_t chunk = 0;
                size_t offset = 0;
                size_t vertexCount = 0;
                SDL_GPUViewport viewport = {};
                SDL_Rect scissor = { 0, 0, 0, 0 };
                float mvp[16];
                size_t uniformsOffset = 0;
                size_t uniformsByteCount = 0;
                size_t texturesOffset = 0;
                size_t textureCount = 0;
            };
            struct PassState
            {
                bool open = false;
                std::shared_ptr<OffscreenBuffer> target;
                bool clear = false;
                Color4F clearColor;
                std::vector<DrawCmd> draws;
                std::vector<uint8_t> uniforms;
                std::vector<SDL_GPUTextureSamplerBinding> textures;
                //! The textures the draws read, to know when one of them is
                //! about to be given other contents.
                std::set<SDL_GPUTexture*> drawTextures;
            };
            PassState pass;
            std::vector<Texture::Upload> uploads;

            struct Shader
            {
                SDL_GPUShader* vertex = nullptr;
                SDL_GPUShader* fragment = nullptr;
                VertexType vertexType = VertexType::UV;
            };
            std::map<std::string, Shader> shaders;
            struct CustomShader
            {
                ShaderSource fragmentSource;
                size_t samplers = 0;
            };
            std::map<std::string, CustomShader> customShaders;
            std::map<std::tuple<std::string, Blend, BufferType>, SDL_GPUGraphicsPipeline*> pipelines;

            // The frame's vertices, in buffers of a fixed size so that one
            // can be bound before it is known how much the frame draws.
            struct Chunk
            {
                SDL_GPUBuffer* buffer = nullptr;
                SDL_GPUTransferBuffer* transfer = nullptr;
                std::vector<uint8_t> data;
                size_t used = 0;
                //! How much of what is used has been sent, this frame.
                size_t sent = 0;
            };
            std::vector<Chunk> chunks;
            size_t chunk = 0;

            SDL_GPUSampler* sampler = nullptr;
            SDL_GPUSampler* samplerNearest = nullptr;

            std::shared_ptr<Texture> glyphTexture;
            std::shared_ptr<BoxPack> glyphPack;
            int glyphAtlasSize = 0;
            std::unordered_map<GlyphInfo, BoxPackID> glyphIDs;
            TriMesh2F textMesh;

            LRUCache<
                std::string,
                std::vector<std::shared_ptr<Texture> > > texturePool;
            LRUCache<
                std::shared_ptr<Image>,
                std::vector<std::shared_ptr<Texture> > > textureCache;

            // The two pass resample: the tables, which depend only on the
            // two sizes, and the intermediates the first pass writes. Kept
            // by size, a few of each; see the OpenGL renderer.
            struct ScaleTable
            {
                std::shared_ptr<Texture> texture;
                int in = 0;
                int out = 0;
                int taps = 0;
            };
            std::list<ScaleTable> scaleTables;
            std::list<std::shared_ptr<OffscreenBuffer> > scaleBuffers;

            const ScaleTable& scaleTable(int in, int out);
            std::shared_ptr<OffscreenBuffer> scaleBuffer(const Size2I&);

            // What this renderer last added to the totals, to take back.
            struct CacheTotals
            {
                size_t cacheByteCount = 0;
                size_t cacheCount = 0;
                size_t poolByteCount = 0;
                size_t poolCount = 0;
            };
            CacheTotals cacheTotals;

            std::vector<VertexUV> verticesUV;
            std::vector<VertexColor> verticesColor;

            std::chrono::time_point<std::chrono::steady_clock> startTime;
            RenderDiag diag;
            std::array<int64_t, 60> frameTimes;
            size_t frameTimePos = 0;
            size_t frameTimeCount = 0;

            void beginPass();
            void endPass();
            void flush();
            void discard();
            void upload(const Texture::Upload&);
            const Shader& getShader(const std::string&);
            SDL_GPUGraphicsPipeline* getPipeline(const std::string&, Blend);
            void draw(
                const std::string& shader,
                Blend,
                const void* vertices,
                size_t vertexCount,
                size_t vertexSize,
                const M44F& transform,
                const void* uniforms,
                size_t uniformsByteCount,
                const SDL_GPUTextureSamplerBinding* textures = nullptr,
                size_t textureCount = 0);
            void drawUV(
                const std::string& shader,
                Blend,
                const TriMesh2F&,
                const M44F& transform,
                const void* uniforms,
                size_t uniformsByteCount,
                const SDL_GPUTextureSamplerBinding* textures = nullptr,
                size_t textureCount = 0);
        };
    }
}
