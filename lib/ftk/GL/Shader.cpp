// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Shader.h>

#include <ftk/GL/GL.h>

#include <ftk/Core/Color.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#include <atomic>
#include <iostream>

namespace ftk
{
    namespace gl
    {
        struct Shader::Private
        {
            std::string vertexSource;
            std::string fragmentSource;
            GLuint vertex = 0;
            GLuint fragment = 0;
            GLuint program = 0;
        };

        namespace
        {
            std::atomic<size_t> objectCount = 0;
        }

        void Shader::_init()
        {
            FTK_P();

            ++objectCount;

            p.vertex = glCreateShader(GL_VERTEX_SHADER);
            if (!p.vertex)
            {
                throw std::runtime_error("Cannot create vertex shader");
            }
            const char* src = p.vertexSource.c_str();
            glShaderSource(p.vertex, 1, &src, NULL);
            glCompileShader(p.vertex);
            int success = 0;
            glGetShaderiv(p.vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[cStringSize];
                glGetShaderInfoLog(p.vertex, cStringSize, NULL, infoLog);
                auto lines = split(
                    p.vertexSource,
                    { '\n', '\r' },
                    SplitOptions::KeepEmpty);
                for (size_t i = 0; i < lines.size(); ++i)
                {
                    lines[i].insert(0, Format("{0}: ").arg(i));
                }
                lines.push_back(infoLog);
                //std::cout << join(lines, '\n') << std::endl;
                throw std::runtime_error(join(lines, '\n'));
            }

            p.fragment = glCreateShader(GL_FRAGMENT_SHADER);
            if (!p.fragment)
            {
                throw std::runtime_error("Cannot create fragment shader");
            }
            src = p.fragmentSource.c_str();
            glShaderSource(p.fragment, 1, &src, NULL);
            glCompileShader(p.fragment);
            glGetShaderiv(p.fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[cStringSize];
                glGetShaderInfoLog(p.fragment, cStringSize, NULL, infoLog);
                auto lines = split(
                    p.fragmentSource,
                    { '\n', '\r' },
                    SplitOptions::KeepEmpty);
                for (size_t i = 0; i < lines.size(); ++i)
                {
                    lines[i].insert(0, Format("{0}: ").arg(i));
                }
                lines.push_back(infoLog);
                //std::cout << join(lines, '\n') << std::endl;
                throw std::runtime_error(join(lines, '\n'));
            }

            p.program = glCreateProgram();
            glAttachShader(p.program, p.vertex);
            glAttachShader(p.program, p.fragment);
            glLinkProgram(p.program);
            glGetProgramiv(p.program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char infoLog[cStringSize];
                glGetProgramInfoLog(p.program, cStringSize, NULL, infoLog);
                throw std::runtime_error(infoLog);
            }
        }

        Shader::Shader() :
            _p(new Private)
        {}

        Shader::~Shader()
        {
            FTK_P();
            if (p.program)
            {
                glDeleteProgram(p.program);
                p.program = 0;
            }
            if (p.vertex)
            {
                glDeleteShader(p.vertex);
                p.vertex = 0;
            }
            if (p.fragment)
            {
                glDeleteShader(p.fragment);
                p.fragment = 0;
            }

            --objectCount;
        }

        std::shared_ptr<Shader> Shader::create(
            const std::string& vertexSource,
            const std::string& fragmentSource)
        {
            auto out = std::shared_ptr<Shader>(new Shader);
            out->_p->vertexSource = vertexSource;
            out->_p->fragmentSource = fragmentSource;
            out->_init();
            return out;
        }

        const std::string& Shader::getVertexSource() const
        {
            return _p->vertexSource;
        }

        const std::string& Shader::getFragmentSource() const
        {
            return _p->fragmentSource;
        }

        unsigned int Shader::getProgram() const
        {
            return _p->program;
        }

        void Shader::bind()
        {
            glUseProgram(_p->program);
        }

        void Shader::setUniform(int location, int value)
        {
            glUniform1i(location, value);
        }

        void Shader::setUniform(int location, float value)
        {
            glUniform1f(location, value);
        }

        void Shader::setUniform(int location, const V2F& value)
        {
            glUniform2fv(location, 1, value.data());
        }

        void Shader::setUniform(int location, const V3F& value)
        {
            glUniform3fv(location, 1, value.data());
        }

        void Shader::setUniform(int location, const V4F& value)
        {
            glUniform4fv(location, 1, value.data());
        }

        void Shader::setUniform(int location, const M33F& value)
        {
            // Transpose the matrix for OpenGL (column-major).
            glUniformMatrix3fv(location, 1, GL_TRUE, value.data());
        }

        void Shader::setUniform(int location, const M44F& value)
        {
            // Transpose the matrix for OpenGL (column-major).
            glUniformMatrix4fv(location, 1, GL_TRUE, value.data());
        }

        void Shader::setUniform(int location, const Color4F& value)
        {
            glUniform4fv(location, 1, value.data());
        }

        void Shader::setUniform(int location, const float value[4])
        {
            glUniform4fv(location, 1, value);
        }

        void Shader::setUniform(int location, const std::vector<int>& value)
        {
            glUniform1iv(location, value.size(), &value[0]);
        }

        void Shader::setUniform(int location, const std::vector<float>& value)
        {
            glUniform1fv(location, value.size(), &value[0]);
        }

        void Shader::setUniform(int location, const std::vector<V3F>& value)
        {
            glUniform3fv(location, value.size(), value[0].data());
        }

        void Shader::setUniform(int location, const std::vector<V4F>& value)
        {
            glUniform4fv(location, value.size(), value[0].data());
        }

        void Shader::setUniform(const std::string& name, int value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, float value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const V2F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const V3F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const V4F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const M33F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const M44F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }
        
        void Shader::setUniform(const std::string& name, const Color4F& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const float value[4])
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const std::vector<int>& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const std::vector<float>& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const std::vector<V3F>& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        void Shader::setUniform(const std::string& name, const std::vector<V4F>& value)
        {
            const GLint location = glGetUniformLocation(_p->program, name.c_str());
            setUniform(location, value);
        }

        size_t Shader::getObjectCount()
        {
            return objectCount;
        }
    }
}
