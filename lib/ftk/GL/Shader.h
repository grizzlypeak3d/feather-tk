// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/Color.h>
#include <ftk/Core/Matrix.h>
#include <ftk/Core/Util.h>
#include <ftk/Core/Vector.h>

#include <memory>
#include <string>
#include <vector>

namespace ftk
{
    namespace gl
    {
        //! \name Shaders
        ///@{
        
        //! Shader.
        class FTK_GL_API_TYPE Shader : public std::enable_shared_from_this<Shader>
        {
            FTK_NON_COPYABLE(Shader);

        protected:
            void _init();

            Shader();

        public:
            FTK_GL_API ~Shader();

            //! Create a new shader.
            FTK_GL_API static std::shared_ptr<Shader> create(
                const std::string& vertexSource,
                const std::string& fragmentSource);

            //! Get the vertex shader source.
            FTK_GL_API const std::string& getVertexSource() const;

            //! Get the fragment shader source.
            FTK_GL_API const std::string& getFragmentSource() const;

            //! Get the OpenGL shader program.
            FTK_GL_API unsigned int getProgram() const;

            //! Bind the shader.
            FTK_GL_API void bind();

            //! \name Uniforms
            //! Set uniform values.
            ///@{

            FTK_GL_API void setUniform(int, int);
            FTK_GL_API void setUniform(int, float);
            FTK_GL_API void setUniform(int, const V2F&);
            FTK_GL_API void setUniform(int, const V3F&);
            FTK_GL_API void setUniform(int, const V4F&);
            FTK_GL_API void setUniform(int, const M33F&);
            FTK_GL_API void setUniform(int, const M44F&);
            FTK_GL_API void setUniform(int, const Color4F&);
            FTK_GL_API void setUniform(int, const float[4]);

            FTK_GL_API void setUniform(int, const std::vector<int>&);
            FTK_GL_API void setUniform(int, const std::vector<float>&);
            FTK_GL_API void setUniform(int, const std::vector<V3F>&);
            FTK_GL_API void setUniform(int, const std::vector<V4F>&);

            FTK_GL_API void setUniform(const std::string&, int);
            FTK_GL_API void setUniform(const std::string&, float);
            FTK_GL_API void setUniform(const std::string&, const V2F&);
            FTK_GL_API void setUniform(const std::string&, const V3F&);
            FTK_GL_API void setUniform(const std::string&, const V4F&);
            FTK_GL_API void setUniform(const std::string&, const M33F&);
            FTK_GL_API void setUniform(const std::string&, const M44F&);
            FTK_GL_API void setUniform(const std::string&, const Color4F&);
            FTK_GL_API void setUniform(const std::string&, const float[4]);

            FTK_GL_API void setUniform(const std::string&, const std::vector<int>&);
            FTK_GL_API void setUniform(const std::string&, const std::vector<float>&);
            FTK_GL_API void setUniform(const std::string&, const std::vector<V3F>&);
            FTK_GL_API void setUniform(const std::string&, const std::vector<V4F>&);

            ///@}

            //! Get the number of objects currenty instantiated.
            FTK_GL_API static size_t getObjectCount();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}
