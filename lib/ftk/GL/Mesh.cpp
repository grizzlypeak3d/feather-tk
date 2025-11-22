// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Mesh.h>

#include <ftk/GL/GL.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/Math.h>
#include <ftk/Core/String.h>

#include <array>
#include <sstream>

namespace ftk
{
    namespace gl
    {
        FTK_ENUM_IMPL(
            VBOType,
            "Pos2 F32",
            "Pos2 F32 UV U16",
            "Pos2 F32 Color F32",
            "Pos3 F32",
            "Pos3 F32 UV U16",
            "Pos3 F32 UV U16 Normal U10",
            "Pos3 F32 UV U16 Normal U10 Color U8",
            "Pos3 F32 UV F32 Normal F32",
            "Pos3 F32 UV F32 Normal F32 Color F32",
            "Pos3 F32 Color U8");

        namespace
        {
            struct PackedNormal
            {
                unsigned int x : 10;
                unsigned int y : 10;
                unsigned int z : 10;
                unsigned int unused : 2;
            };

            struct PackedColor
            {
                unsigned int r : 8;
                unsigned int g : 8;
                unsigned int b : 8;
                unsigned int a : 8;
            };
        }

        std::size_t getByteCount(VBOType value)
        {
            static const std::array<size_t, static_cast<size_t>(VBOType::Count)> data =
            {
                2 * sizeof(float),
                2 * sizeof(float) + 2 * sizeof(uint16_t),
                2 * sizeof(float) + 4 * sizeof(float),
                3 * sizeof(float),
                3 * sizeof(float) + 2 * sizeof(uint16_t),
                3 * sizeof(float) + 2 * sizeof(uint16_t) + sizeof(PackedNormal),
                3 * sizeof(float) + 2 * sizeof(uint16_t) + sizeof(PackedNormal) + sizeof(PackedColor),
                3 * sizeof(float) + 2 * sizeof(float) + 3 * sizeof(float),
                3 * sizeof(float) + 2 * sizeof(float) + 3 * sizeof(float) + 4 * sizeof(float),
                3 * sizeof(float) + sizeof(PackedColor)
            };
            return data[static_cast<size_t>(value)];
        }

        std::vector<uint8_t> convert(const TriMesh2F& mesh, VBOType type)
        {
            return convert(
                mesh,
                type,
                RangeSizeT(0, mesh.triangles.size() > 0 ? (mesh.triangles.size() - 1) : 0));
        }

        std::vector<uint8_t> convert(
            const TriMesh2F& mesh,
            VBOType type,
            const RangeSizeT& range)
        {
            const size_t vertexByteCount = getByteCount(type);
            std::vector<uint8_t> out((range.max() - range.min() + 1) * 3 * vertexByteCount);
            const size_t vSize = mesh.v.size();
            const size_t tSize = mesh.t.size();
            const size_t cSize = mesh.c.size();
            uint8_t* p = out.data();
            switch (type)
            {
            case VBOType::Pos2_F32:
            {
                float* pf = reinterpret_cast<float*>(p);
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle2* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                        }
                        pf += 2;
                    }
                }
                break;
            }
            case VBOType::Pos2_F32_UV_U16:
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle2* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        float* pf = reinterpret_cast<float*>(p);
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                        }
                        p += 2 * sizeof(float);

                        const size_t t = tri->v[k].t;
                        uint16_t* pu16 = reinterpret_cast<uint16_t*>(p);
                        if (t && t <= tSize)
                        {
                            pu16[0] = clamp(static_cast<int>(mesh.t[t - 1].x * 65535.F), 0, 65535);
                            pu16[1] = clamp(static_cast<int>(mesh.t[t - 1].y * 65535.F), 0, 65535);
                        }
                        else
                        {
                            pu16[0] = 0;
                            pu16[1] = 0;
                        }
                        p += 2 * sizeof(uint16_t);
                    }
                }
                break;
            case VBOType::Pos2_F32_Color_F32:
            {
                float* pf = reinterpret_cast<float*>(p);
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle2* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                        }
                        pf += 2;

                        const size_t c = tri->v[k].c;
                        if (c && c <= cSize)
                        {
                            pf[0] = mesh.c[c - 1].x;
                            pf[1] = mesh.c[c - 1].y;
                            pf[2] = mesh.c[c - 1].z;
                            pf[3] = mesh.c[c - 1].w;
                        }
                        else
                        {
                            pf[0] = 1.F;
                            pf[1] = 1.F;
                            pf[2] = 1.F;
                            pf[3] = 1.F;
                        }
                        pf += 4;
                    }
                }
                break;
            }
            default: break;
            }
            return out;
        }

        std::vector<uint8_t> convert(const TriMesh3F& mesh, VBOType type)
        {
            return convert(
                mesh,
                type,
                RangeSizeT(0, mesh.triangles.size() > 0 ? (mesh.triangles.size() - 1) : 0));
        }

        std::vector<uint8_t> convert(
            const TriMesh3F& mesh,
            VBOType type,
            const RangeSizeT& range)
        {
            const size_t vertexByteCount = getByteCount(type);
            std::vector<uint8_t> out((range.max() - range.min() + 1) * 3 * vertexByteCount);
            const size_t vSize = mesh.v.size();
            const size_t tSize = mesh.t.size();
            const size_t nSize = mesh.n.size();
            const size_t cSize = mesh.c.size();
            uint8_t* p = out.data();
            switch (type)
            {
            case VBOType::Pos3_F32:
            {
                float* pf = reinterpret_cast<float*>(p);
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        pf += 3;
                    }
                }
                break;
            }
            case VBOType::Pos3_F32_UV_U16:
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        float* pf = reinterpret_cast<float*>(p);
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        p += 3 * sizeof(float);

                        const size_t t = tri->v[k].t;
                        uint16_t* pu16 = reinterpret_cast<uint16_t*>(p);
                        if (t && t <= tSize)
                        {
                            pu16[0] = clamp(static_cast<int>(mesh.t[t - 1].x * 65535.F), 0, 65535);
                            pu16[1] = clamp(static_cast<int>(mesh.t[t - 1].y * 65535.F), 0, 65535);
                        }
                        else
                        {
                            pu16[0] = 0;
                            pu16[0] = 0;
                        }
                        p += 2 * sizeof(uint16_t);
                    }
                }
                break;
            case VBOType::Pos3_F32_UV_U16_Normal_U10:
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        float* pf = reinterpret_cast<float*>(p);
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        p += 3 * sizeof(float);

                        const size_t t = tri->v[k].t;
                        uint16_t* pu16 = reinterpret_cast<uint16_t*>(p);
                        if (t && t <= tSize)
                        {
                            pu16[0] = clamp(static_cast<int>(mesh.t[t - 1].x * 65535.F), 0, 65535);
                            pu16[1] = clamp(static_cast<int>(mesh.t[t - 1].y * 65535.F), 0, 65535);
                        }
                        else
                        {
                            pu16[0] = 0;
                            pu16[1] = 0;
                        }
                        p += 2 * sizeof(uint16_t);

                        const size_t n = tri->v[k].n;
                        auto packedNormal = reinterpret_cast<PackedNormal*>(p);
                        if (n && n <= nSize)
                        {
                            packedNormal->x = clamp(static_cast<int>(mesh.n[n - 1].x * 511.F), -512, 511);
                            packedNormal->y = clamp(static_cast<int>(mesh.n[n - 1].y * 511.F), -512, 511);
                            packedNormal->z = clamp(static_cast<int>(mesh.n[n - 1].z * 511.F), -512, 511);
                        }
                        else
                        {
                            packedNormal->x = 0;
                            packedNormal->y = 0;
                            packedNormal->z = 0;
                        }
                        p += sizeof(PackedNormal);
                    }
                }
                break;
            case VBOType::Pos3_F32_UV_U16_Normal_U10_Color_U8:
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        float* pf = reinterpret_cast<float*>(p);
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        p += 3 * sizeof(float);

                        const size_t t = tri->v[k].t;
                        uint16_t* pu16 = reinterpret_cast<uint16_t*>(p);
                        if (t && t <= tSize)
                        {
                            pu16[0] = clamp(static_cast<int>(mesh.t[t - 1].x * 65535.F), 0, 65535);
                            pu16[1] = clamp(static_cast<int>(mesh.t[t - 1].y * 65535.F), 0, 65535);
                        }
                        else
                        {
                            pu16[0] = 0;
                            pu16[1] = 0;
                        }
                        p += 2 * sizeof(uint16_t);

                        const size_t n = tri->v[k].n;
                        auto packedNormal = reinterpret_cast<PackedNormal*>(p);
                        if (n && n <= nSize)
                        {
                            packedNormal->x = clamp(static_cast<int>(mesh.n[n - 1].x * 511.F), -512, 511);
                            packedNormal->y = clamp(static_cast<int>(mesh.n[n - 1].y * 511.F), -512, 511);
                            packedNormal->z = clamp(static_cast<int>(mesh.n[n - 1].z * 511.F), -512, 511);
                        }
                        else
                        {
                            packedNormal->x = 0;
                            packedNormal->y = 0;
                            packedNormal->z = 0;
                        }
                        p += sizeof(PackedNormal);

                        const size_t c = tri->v[k].c;
                        auto packedColor = reinterpret_cast<PackedColor*>(p);
                        if (c && c <= cSize)
                        {
                            packedColor->r = clamp(static_cast<int>(mesh.c[c - 1].x * 255.F), 0, 255);
                            packedColor->g = clamp(static_cast<int>(mesh.c[c - 1].y * 255.F), 0, 255);
                            packedColor->b = clamp(static_cast<int>(mesh.c[c - 1].z * 255.F), 0, 255);
                            packedColor->a = clamp(static_cast<int>(mesh.c[c - 1].w * 255.F), 0, 255);
                        }
                        else
                        {
                            packedColor->r = 255;
                            packedColor->g = 255;
                            packedColor->b = 255;
                            packedColor->a = 255;
                        }
                        p += sizeof(PackedColor);
                    }
                }
                break;
            case VBOType::Pos3_F32_UV_F32_Normal_F32:
            {
                float* pf = reinterpret_cast<float*>(p);
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        pf += 3;

                        const size_t t = tri->v[k].t;
                        if (t && t <= tSize)
                        {
                            pf[0] = mesh.t[t - 1].x;
                            pf[1] = mesh.t[t - 1].y;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                        }
                        pf += 2;

                        const size_t n = tri->v[k].n;
                        if (n && n <= nSize)
                        {
                            pf[0] = mesh.n[n - 1].x;
                            pf[1] = mesh.n[n - 1].y;
                            pf[2] = mesh.n[n - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        pf += 3;
                    }
                }
                break;
            }
            case VBOType::Pos3_F32_UV_F32_Normal_F32_Color_F32:
            {
                float* pf = reinterpret_cast<float*>(p);
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        if (v && v <= vSize)
                        {
                            pf[0] = mesh.v[v - 1].x;
                            pf[1] = mesh.v[v - 1].y;
                            pf[2] = mesh.v[v - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        pf += 3;

                        const size_t t = tri->v[k].t;
                        if (t && t <= tSize)
                        {
                            pf[0] = mesh.t[t - 1].x;
                            pf[1] = mesh.t[t - 1].y;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                        }
                        pf += 2;

                        const size_t n = tri->v[k].n;
                        if (n && n <= nSize)
                        {
                            pf[0] = mesh.n[n - 1].x;
                            pf[1] = mesh.n[n - 1].y;
                            pf[2] = mesh.n[n - 1].z;
                        }
                        else
                        {
                            pf[0] = 0.F;
                            pf[1] = 0.F;
                            pf[2] = 0.F;
                        }
                        pf += 3;

                        const size_t c = tri->v[k].c;
                        if (c && c <= cSize)
                        {
                            pf[0] = mesh.c[c - 1].x;
                            pf[1] = mesh.c[c - 1].y;
                            pf[2] = mesh.c[c - 1].z;
                            pf[3] = mesh.c[c - 1].w;
                        }
                        else
                        {
                            pf[0] = 1.F;
                            pf[1] = 1.F;
                            pf[2] = 1.F;
                            pf[3] = 1.F;
                        }
                        pf += 4;
                    }
                }
                break;
            }
            case VBOType::Pos3_F32_Color_U8:
                for (size_t i = range.min(); i <= range.max(); ++i)
                {
                    const Triangle3* tri = &mesh.triangles[i];
                    for (size_t k = 0; k < 3; ++k)
                    {
                        const size_t v = tri->v[k].v;
                        float* pf = reinterpret_cast<float*>(p);
                        pf[0] = (v && v <= vSize) ? mesh.v[v - 1].x : 0.F;
                        pf[1] = (v && v <= vSize) ? mesh.v[v - 1].y : 0.F;
                        pf[2] = (v && v <= vSize) ? mesh.v[v - 1].z : 0.F;
                        p += 3 * sizeof(float);

                        const size_t c = tri->v[k].c;
                        auto packedColor = reinterpret_cast<PackedColor*>(p);
                        packedColor->r = (c && c <= cSize) ? clamp(static_cast<int>(mesh.c[c - 1].x * 255.F), 0, 255) : 255;
                        packedColor->g = (c && c <= cSize) ? clamp(static_cast<int>(mesh.c[c - 1].y * 255.F), 0, 255) : 255;
                        packedColor->b = (c && c <= cSize) ? clamp(static_cast<int>(mesh.c[c - 1].z * 255.F), 0, 255) : 255;
                        packedColor->a = (c && c <= cSize) ? clamp(static_cast<int>(mesh.c[c - 1].w * 255.F), 0, 255) : 255;
                        p += sizeof(PackedColor);
                    }
                }
                break;
            default: break;
            }
            return out;
        }

        struct VBO::Private
        {
            std::size_t size = 0;
            VBOType type = VBOType::First;
            GLuint vbo = 0;
        };

        VBO::VBO(std::size_t size, VBOType type) :
            _p(new Private)
        {
            FTK_P();
            p.size = size;
            p.type = type;
            glGenBuffers(1, &p.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, p.vbo);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(p.size * getByteCount(type)), NULL, GL_DYNAMIC_DRAW);
        }

        VBO::~VBO()
        {
            FTK_P();
            if (p.vbo)
            {
                glDeleteBuffers(1, &p.vbo);
                p.vbo = 0;
            }
        }

        std::shared_ptr<VBO> VBO::create(std::size_t size, VBOType type)
        {
            return std::shared_ptr<VBO>(new VBO(size, type));
        }

        size_t VBO::getSize() const
        {
            return _p->size;
        }

        VBOType VBO::getType() const
        {
            return _p->type;
        }

        unsigned int VBO::getID() const
        {
            return _p->vbo;
        }

        void VBO::copy(const std::vector<uint8_t>& data)
        {
            glBindBuffer(GL_ARRAY_BUFFER, _p->vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(data.size()), (void*)data.data());
        }

        void VBO::copy(const std::vector<uint8_t>& data, std::size_t offset, std::size_t size)
        {
            glBindBuffer(GL_ARRAY_BUFFER, _p->vbo);
            glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizei>(size), (void*)data.data());
        }

        struct VAO::Private
        {
            GLuint vao = 0;
        };

        VAO::VAO(VBOType type, unsigned int vbo) :
            _p(new Private)
        {
            FTK_P();

#if defined(FTK_API_GL_4_1)
            glGenVertexArrays(1, &p.vao);
            glBindVertexArray(p.vao);
#elif defined(FTK_API_GLES_2)
            glGenVertexArraysOES(1, &p.vao);
            glBindVertexArrayOES(p.vao);
#endif // FTK_API_GL_4_1
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            const std::size_t byteCount = getByteCount(type);
            switch (type)
            {
            case VBOType::Pos2_F32:
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                break;
            case VBOType::Pos2_F32_UV_U16:
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, static_cast<GLsizei>(byteCount), (GLvoid*)8);
                glEnableVertexAttribArray(1);
                break;
            case VBOType::Pos2_F32_Color_F32:
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)8);
                glEnableVertexAttribArray(1);
                break;
            case VBOType::Pos3_F32:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                break;
            case VBOType::Pos3_F32_UV_U16:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, static_cast<GLsizei>(byteCount), (GLvoid*)12);
                glEnableVertexAttribArray(1);
                break;
            case VBOType::Pos3_F32_UV_F32_Normal_F32:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)12);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)20);
                glEnableVertexAttribArray(2);
                break;
            case VBOType::Pos3_F32_UV_F32_Normal_F32_Color_F32:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)12);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)20);
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)32);
                glEnableVertexAttribArray(3);
                break;
            case VBOType::Pos3_F32_Color_U8:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(byteCount), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, static_cast<GLsizei>(byteCount), (GLvoid*)12);
                glEnableVertexAttribArray(1);
                break;
            default: break;
            }
        }

        VAO::~VAO()
        {
            FTK_P();
            if (p.vao)
            {
#if defined(FTK_API_GL_4_1)
                glDeleteVertexArrays(1, &p.vao);
#elif defined(FTK_API_GLES_2)
                glDeleteVertexArraysOES(1, &p.vao);
#endif // FTK_API_GL_4_1
                p.vao = 0;
            }
        }

        std::shared_ptr<VAO> VAO::create(VBOType type, unsigned int vbo)
        {
            return std::shared_ptr<VAO>(new VAO(type, vbo));
        }

        unsigned int VAO::getID() const
        {
            return _p->vao;
        }

        void VAO::bind()
        {
#if defined(FTK_API_GL_4_1)
            glBindVertexArray(_p->vao);
#elif defined(FTK_API_GLES_2)
            glBindVertexArrayOES(_p->vao);
#endif // FTK_API_GL_4_1
        }

        void VAO::draw(unsigned int mode, std::size_t offset, std::size_t size)
        {
            glDrawArrays(mode, static_cast<GLsizei>(offset), static_cast<GLsizei>(size));
        }
    }
}
