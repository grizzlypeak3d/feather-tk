// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/FileIO.h>
#include <ftk/Core/ISystem.h>
#include <ftk/Core/Image.h>

#include <list>

namespace ftk
{
        //! \name Image I/O
        ///@{

        //! Image I/O options.
        typedef std::map<std::string, std::string> ImageIOOptions;

        //! Merge image I/O options.
        FTK_API ImageIOOptions merge(const ImageIOOptions&, const ImageIOOptions&);
        
        //! Base class for image readers.
        class FTK_API_TYPE IImageReader
        {
        public:
            FTK_API IImageReader(
                const std::filesystem::path&,
                const MemFile*,
                const ImageIOOptions&);
            
            FTK_API virtual ~IImageReader() = 0;

            //! Get information about the image.
            FTK_API virtual const ImageInfo& getInfo() const = 0;

            //! Read the image.
            FTK_API virtual std::shared_ptr<Image> read() = 0;

        protected:
            std::filesystem::path _path;
        };

        //! Base class for image writers.
        class FTK_API_TYPE IImageWriter
        {
        public:
            FTK_API IImageWriter(
                const std::filesystem::path&,
                const ImageIOOptions&);

            FTK_API virtual ~IImageWriter() = 0;
            
            //! Write the image.
            FTK_API virtual void write(const std::shared_ptr<Image>&) = 0;

        protected:
            std::filesystem::path _path;
        };
        
        //! Base class for image I/O plugins.
        class FTK_API_TYPE IImagePlugin : std::enable_shared_from_this<IImagePlugin>
        {
            FTK_NON_COPYABLE(IImagePlugin);

        protected:
            IImagePlugin(
                const std::string& name,
                const std::vector<std::string>& exts);

        public:
            FTK_API virtual ~IImagePlugin() = 0;

            FTK_API const std::string& getName() const;

            FTK_API const std::vector<std::string>& getExts() const;
            
            FTK_API virtual bool canRead(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions());

            FTK_API virtual std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions());
            
            FTK_API virtual std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const MemFile&,
                const ImageIOOptions& = ImageIOOptions());

            FTK_API virtual bool canWrite(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions());

            FTK_API virtual std::shared_ptr<IImageWriter> write(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions());
                
        private:
            std::string _name;
            std::vector<std::string> _exts;
        };
        
        //! Image I/O system.
        class FTK_API_TYPE ImageIO : public ISystem
        {
        protected:
            ImageIO(const std::shared_ptr<Context>&);

        public:
            FTK_API virtual ~ImageIO();

            //! Create a new system.
            FTK_API static std::shared_ptr<ImageIO> create(const std::shared_ptr<Context>&);

            //! Get the plugins.
            FTK_API const std::list<std::shared_ptr<IImagePlugin> >& getPlugins() const;
            
            //! Add a plugin.
            FTK_API void addPlugin(const std::shared_ptr<IImagePlugin>&);
            
            //! Get an image reader.
            FTK_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const ImageIOOptions& = ImageIOOptions());

            //! Get an image reader.
            FTK_API std::shared_ptr<IImageReader> read(
                const std::filesystem::path&,
                const MemFile&,
                const ImageIOOptions& = ImageIOOptions());
            
            //! Get an image writer.
            FTK_API std::shared_ptr<IImageWriter> write(
                const std::filesystem::path&,
                const ImageInfo&,
                const ImageIOOptions& = ImageIOOptions());

        private:
            std::list<std::shared_ptr<IImagePlugin> > _plugins;
        };
        
        ///@}
}

