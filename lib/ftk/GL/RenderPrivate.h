// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Render.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Mesh.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#include <ftk/GL/TextureAtlas.h>

#include <chrono>
#include <list>
#include <map>
#include <unordered_map>

#include <array>

namespace ftk
{
    namespace gl
    {
        std::string vertexSource();
        std::string meshFragmentSource();
        std::string colorMeshVertexSource();
        std::string colorMeshFragmentSource();
        std::string textureFragmentSource();
        std::string textFragmentSource();
        std::string imageFragmentSource();
        std::string textureScaleFragmentSource();
        std::string imageScaleXFragmentSource();
        std::string imageScaleYFragmentSource();

        struct Render::Private
        {
            Size2I size;
            RenderOptions options;
            Box2I viewport;
            bool clipRectEnabled = false;
            Box2I clipRect;
            M44F transform;
            
            std::map<std::string, std::shared_ptr<gl::Shader> > shaders;

            LRUCache<
                std::string,
                std::vector<std::shared_ptr<Texture> > > texturePool;
            LRUCache<
                std::shared_ptr<Image>,
                std::vector<std::shared_ptr<Texture> > > textureCache;
            std::shared_ptr<gl::TextureAtlas> glyphAtlas;
            std::unordered_map<GlyphInfo, BoxPackID> glyphIDs;
            TriMesh2F textMesh;
            // High quality scaling: the intermediate the first pass writes,
            // and the contribution tables, which depend only on the two sizes
            // so they are rebuilt only when those change.
            struct ScaleData
            {
                // Kept by size rather than one of each: a comparison draws
                // two pictures of different sizes in the same frame, and a
                // single slot means each evicts the other -- an offscreen
                // buffer and a pair of tables built and thrown away several
                // times a frame, for as long as the playback runs.
                struct Table
                {
                    std::shared_ptr<Texture> texture;
                    int in = 0;
                    int out = 0;
                    int taps = 0;
                };
                std::list<Table> tables;
                std::list<std::shared_ptr<OffscreenBuffer> > buffers;

                // What the passes below are using now.
                std::shared_ptr<OffscreenBuffer> buffer;
                std::shared_ptr<Texture> xContrib;
                std::shared_ptr<Texture> yContrib;
                int xTaps = 0;
                int yTaps = 0;
            };
            ScaleData scale;

            std::map<std::string, std::shared_ptr<gl::VBO> > vbos;
            std::map<std::string, std::shared_ptr<gl::VAO> > vaos;

            std::chrono::time_point<std::chrono::steady_clock> startTime;
            RenderDiag diag;

            //! Frame times in microseconds, oldest overwritten first. About a
            //! second at sixty frames a second.
            std::array<int64_t, 60> frameTimes;
            size_t frameTimePos = 0;
            size_t frameTimeCount = 0;
        };
    }
}

