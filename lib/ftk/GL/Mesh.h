// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Mesh.h>
#include <ftk/Core/Range.h>
#include <ftk/Core/Util.h>

#include <memory>
#include <vector>

namespace ftk
{
    namespace gl
    {        
        //! \name Meshes
        ///@{

        //! Vertex buffer object types.
        enum class FTK_API_TYPE VBOType
        {
            Pos2_F32,
            Pos2_F32_UV_U16,
            Pos2_F32_Color_F32,
            Pos3_F32,
            Pos3_F32_UV_U16,
            Pos3_F32_UV_U16_Normal_U10,
            Pos3_F32_UV_U16_Normal_U10_Color_U8,
            Pos3_F32_UV_F32_Normal_F32,
            Pos3_F32_UV_F32_Normal_F32_Color_F32,
            Pos3_F32_Color_U8,

            Count,
            First = Pos2_F32
        };
        FTK_ENUM(VBOType);

        //! Get the number of bytes used to store vertex buffer object types.
        FTK_API std::size_t getByteCount(VBOType);

        //! Convert a triangle mesh to vertex buffer data.
        FTK_API std::vector<uint8_t> convert(const TriMesh2F&, VBOType);

        //! Convert a triangle mesh to vertex buffer data.
        FTK_API std::vector<uint8_t> convert(const TriMesh2F&, VBOType, const RangeSizeT&);

        //! Convert a triangle mesh to vertex buffer data.
        FTK_API std::vector<uint8_t> convert(const TriMesh3F&, VBOType);

        //! Convert a triangle mesh to vertex buffer data.
        FTK_API std::vector<uint8_t> convert(const TriMesh3F&, VBOType, const RangeSizeT&);

        //! Vertex buffer object.
        class FTK_API_TYPE VBO : public std::enable_shared_from_this<VBO>
        {
            FTK_NON_COPYABLE(VBO);

        protected:
            VBO(std::size_t size, VBOType);

        public:
            FTK_API ~VBO();

            //! Create a new object.
            FTK_API static std::shared_ptr<VBO> create(std::size_t size, VBOType);

            //! Get the size.
            FTK_API std::size_t getSize() const;

            //! Get the type.
            FTK_API VBOType getType() const;

            //! Get the OpenGL ID.
            FTK_API unsigned int getID() const;

            //! \name Copy
            //! Copy data to the vertex buffer object.
            ///@{

            FTK_API void copy(const std::vector<uint8_t>&);
            FTK_API void copy(const std::vector<uint8_t>&, std::size_t offset, std::size_t size);

            ///@}

            //! Get the number of objects currenty instantiated.
            FTK_API static size_t getObjectCount();

            //! Get the total number of bytes currently used.
            FTK_API static size_t getTotalByteCount();

        private:
            FTK_PRIVATE();
        };

        //! Vertex array object.
        class FTK_API_TYPE VAO : public std::enable_shared_from_this<VAO>
        {
            FTK_NON_COPYABLE(VAO);

        protected:
            VAO(VBOType, unsigned int vbo);

        public:
            FTK_API ~VAO();

            //! Create a new object.
            FTK_API static std::shared_ptr<VAO> create(VBOType, unsigned int vbo);

            //! Get the OpenGL ID.
            FTK_API unsigned int getID() const;

            //! Bind the vertex array object.
            FTK_API void bind();

            //! Draw the vertex array object.
            FTK_API void draw(unsigned int mode, std::size_t offset, std::size_t size);

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
