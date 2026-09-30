// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Image.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#include <atomic>
#include <array>
#include <cstring>
#include <map>
#include <mutex>
#include <sstream>
#include <vector>

namespace ftk
{
    FTK_ENUM_IMPL(
        ImageType,
        "None",

        "L U8",
        "L U16",
        "L U32",
        "L F16",
        "L F32",

        "LA U8",
        "LA U16",
        "LA U32",
        "LA F16",
        "LA F32",

        "RGB U8",
        "RGB U10",
        "RGB U16",
        "RGB U32",
        "RGB F16",
        "RGB F32",

        "RGBA U8",
        "RGBA U16",
        "RGBA U32",
        "RGBA F16",
        "RGBA F32",

        "YUV 420P U8",
        "YUV 422P U8",
        "YUV 444P U8",
        "YUV 420P U16",
        "YUV 422P U16",
        "YUV 444P U16",

        "YUV 420SP U8",
        "YUV 420SP U16",

        "RGB F16 P");

    int getChannelCount(ImageType value)
    {
        const std::array<int, static_cast<size_t>(ImageType::Count)> values =
        {
            0,
            1, 1, 1, 1, 1,
            2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3,
            4, 4, 4, 4, 4,
            3, 3, 3,
            3, 3, 3,
            3, 3,
            3
        };
        return values[static_cast<size_t>(value)];
    }

    int getBitDepth(ImageType value)
    {
        const std::array<int, static_cast<size_t>(ImageType::Count)> values =
        {
            0,
            8, 16, 32, 16, 32,
            8, 16, 32, 16, 32,
            8, 10, 16, 32, 16, 32,
            8, 16, 32, 16, 32,
            8, 8, 8,
            16, 16, 16,
            8, 16,
            16
        };
        return values[static_cast<size_t>(value)];
    }

    FTK_ENUM_IMPL(
        YUVCoefficients,
        "REC709",
        "BT2020",
        "BT601");

    V4F getYUVCoefficients(YUVCoefficients value)
    {
        //! References:
        //! * https://www.itu.int/rec/R-REC-BT.709
        //! * https://www.itu.int/rec/R-REC-BT.2020
        //! * https://www.itu.int/rec/R-REC-BT.601
        //! * https://gist.github.com/yohhoy/dafa5a47dade85d8b40625261af3776a
        //!
        //!     Y  = a * R + b * G + c * B
        //!     Cb = (B - Y) / d
        //!     Cr = (R - Y) / e
        //!
        //!     R = Y + e * Cr
        //!     G = Y - (a * e / b) * Cr - (c * d / b) * Cb
        //!     B = Y + d * Cb
        //!
        //!        BT.601   BT.709   BT.2020
        //!     ----------------------------
        //!     a  0.299    0.2126   0.2627
        //!     b  0.587    0.7152   0.6780
        //!     c  0.114    0.0722   0.0593
        //!     d  1.772    1.8556   1.8814
        //!     e  1.402    1.5748   1.4746
        //!
        const std::array<V4F, static_cast<size_t>(YUVCoefficients::Count)> data =
        {
            V4F(1.5748, 0.468124273, 0.187324273, 1.8556),
            V4F(1.4746, 0.571353127, 0.164553127, 1.8814),
            V4F(1.402, 0.714136286, 0.344136286, 1.772)
        };
        return data[static_cast<size_t>(value)];
    }

    FTK_ENUM_IMPL(
        VideoLevels,
        "Full Range",
        "Legal Range");

    size_t ImageInfo::getByteCount() const
    {
        std::size_t out = 0;
        const size_t w = size.w;
        const size_t h = size.h;
        const size_t alignment = layout.alignment;
        switch (type)
        {
        case ImageType::L_U8:     out = getAlignedByteCount(w, alignment) * h; break;
        case ImageType::L_U16:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::L_U32:    out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::L_F16:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::L_F32:    out = getAlignedByteCount(w * 4, alignment) * h; break;

        case ImageType::LA_U8:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::LA_U16:   out = getAlignedByteCount(w * 2 * 2, alignment) * h; break;
        case ImageType::LA_U32:   out = getAlignedByteCount(w * 2 * 4, alignment) * h; break;
        case ImageType::LA_F16:   out = getAlignedByteCount(w * 2 * 2, alignment) * h; break;
        case ImageType::LA_F32:   out = getAlignedByteCount(w * 2 * 4, alignment) * h; break;

        case ImageType::RGB_U8:   out = getAlignedByteCount(w * 3, alignment) * h; break;
        case ImageType::RGB_U10:  out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::RGB_U16:  out = getAlignedByteCount(w * 3 * 2, alignment) * h; break;
        case ImageType::RGB_U32:  out = getAlignedByteCount(w * 3 * 4, alignment) * h; break;
        case ImageType::RGB_F16:  out = getAlignedByteCount(w * 3 * 2, alignment) * h; break;
        case ImageType::RGB_F32:  out = getAlignedByteCount(w * 3 * 4, alignment) * h; break;

        case ImageType::RGBA_U8:  out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::RGBA_U16: out = getAlignedByteCount(w * 4 * 2, alignment) * h; break;
        case ImageType::RGBA_U32: out = getAlignedByteCount(w * 4 * 4, alignment) * h; break;
        case ImageType::RGBA_F16: out = getAlignedByteCount(w * 4 * 2, alignment) * h; break;
        case ImageType::RGBA_F32: out = getAlignedByteCount(w * 4 * 4, alignment) * h; break;

            //! \todo Is YUV data aligned?
        case ImageType::YUV_420P_U8:  out = w * h + ((w + 1) / 2) * ((h + 1) / 2) * 2; break;
        case ImageType::YUV_422P_U8:  out = w * h + ((w + 1) / 2) * h * 2; break;
        case ImageType::YUV_444P_U8:  out = w * h * 3; break;
        case ImageType::YUV_420P_U16: out = (w * h + ((w + 1) / 2) * ((h + 1) / 2) * 2) * 2; break;
        case ImageType::YUV_422P_U16: out = (w * h + ((w + 1) / 2) * h * 2) * 2; break;
        case ImageType::YUV_444P_U16: out = (w * h * 3) * 2; break;

        case ImageType::YUV_420SP_U8:  out = w * h + ((w + 1) / 2) * ((h + 1) / 2) * 2; break;
        case ImageType::YUV_420SP_U16: out = (w * h + ((w + 1) / 2) * ((h + 1) / 2) * 2) * 2; break;

        // Three planes, each the size of a single channel half image.
        case ImageType::RGB_F16_P:
            out = getAlignedByteCount(w * 2, alignment) * h * 3;
            break;

        default: break;
        }
        return out;
    }

    ImageType getInterleavedType(ImageType value)
    {
        return ImageType::RGB_F16_P == value ? ImageType::RGB_F16 : value;
    }

    std::string getLabel(const ImageInfo& info)
    {
        return ftk::Format("{0}x{1}:{2} {3}").
            arg(info.size.w).
            arg(info.size.h).
            arg(info.getAspect(), 2).
            arg(getInterleavedType(info.type));
    }

    namespace
    {
        std::atomic<size_t> objectCount = 0;
        std::atomic<size_t> totalByteCount = 0;

        // Room past the end of the image data. FFmpeg's sws_scale() writes
        // its output a SIMD register at a time and runs off the end of the
        // last row by as much as a register; the image has to carry the
        // slack or the write lands outside the allocation.
        //
        // Sixty-four because that is what FFmpeg itself pads by --
        // AV_INPUT_BUFFER_PADDING_SIZE, raised from 8 to 16 to 32 to 64 as
        // the registers grew. Named here rather than taken from FFmpeg: this
        // library sits underneath it and does not otherwise know it exists.
        //
        // Sixteen was the value before, and a build with a checked heap
        // caught a write thirty-two bytes past the end of it.
        constexpr size_t dataPadding = 64;

        // Image data kept for reuse. Freeing a large buffer unmaps it, and
        // allocating one maps fresh pages that fault in on first write; both
        // wait on the process's memory map, which every other thread faulting
        // is waiting on too. A buffer kept and handed out again does neither.
        //
        // A player seeking through 4K EXRs freed its whole cache at once, 42 MB
        // at a time, and filled it again with new buffers: the cache thread
        // spent most of a second freeing, and faulting in the new frames was
        // most of the time the reader took for each.
        //
        // Only kept while the images that are alive and the buffers kept stay
        // within the maximum: the kept buffers are the room the live ones have
        // just given up, not memory on top of it. Buffers are only handed out
        // again for the same size, so when the sizes change -- another file,
        // another resolution -- the new images take the room and the old
        // buffers are no longer kept.
        struct BufferPool
        {
            std::mutex mutex;
            std::map<size_t, std::vector<uint8_t*> > buffers;
            size_t byteCount = 0;
            size_t maxByteCount = 0;
        };

        // Only buffers this big: small images are cheap to allocate.
        constexpr size_t bufferPoolMin = 1024 * 1024;

        BufferPool& getBufferPool()
        {
            // Never destroyed, so an image outliving the static objects still
            // has a pool to go back to.
            static BufferPool* pool = new BufferPool;
            return *pool;
        }

        // Free kept buffers until the images alive and the buffers kept fit
        // within the maximum. Called with the pool locked.
        void trimBufferPool(BufferPool& pool)
        {
            auto i = pool.buffers.begin();
            while (i != pool.buffers.end() &&
                totalByteCount + pool.byteCount > pool.maxByteCount)
            {
                while (!i->second.empty() &&
                    totalByteCount + pool.byteCount > pool.maxByteCount)
                {
                    delete[] i->second.back();
                    i->second.pop_back();
                    pool.byteCount -= i->first;
                }
                i = i->second.empty() ? pool.buffers.erase(i) : std::next(i);
            }
        }

        uint8_t* acquireBuffer(size_t size)
        {
            if (size >= bufferPoolMin)
            {
                auto& pool = getBufferPool();
                std::unique_lock<std::mutex> lock(pool.mutex);
                const auto i = pool.buffers.find(size);
                if (i != pool.buffers.end() && !i->second.empty())
                {
                    uint8_t* out = i->second.back();
                    i->second.pop_back();
                    pool.byteCount -= size;
                    return out;
                }

                // None this size: the kept ones are another size, and make
                // room for this one, which counts as alive already.
                trimBufferPool(pool);
            }
            return new uint8_t[size];
        }

        // Called with the image no longer counted as alive.
        void releaseBuffer(uint8_t* data, size_t size)
        {
            if (size >= bufferPoolMin)
            {
                auto& pool = getBufferPool();
                std::unique_lock<std::mutex> lock(pool.mutex);
                if (totalByteCount + pool.byteCount + size <= pool.maxByteCount)
                {
                    pool.buffers[size].push_back(data);
                    pool.byteCount += size;
                    return;
                }
            }
            delete[] data;
        }
    }

    Image::Image(const ImageInfo& info, uint8_t* externalData) :
        _info(info),
        _byteCount(info.getByteCount()),
        _externalData(externalData)
    {
        ++objectCount;
        totalByteCount += _byteCount;

        if (externalData)
        {
            _data = externalData;
        }
        else
        {
            _data = acquireBuffer(_byteCount + dataPadding);
        }
    }

    Image::~Image()
    {
        --objectCount;
        totalByteCount -= _byteCount;

        if (!_externalData)
        {
            releaseBuffer(_data, _byteCount + dataPadding);
        }
    }

    std::shared_ptr<Image> Image::create(const ImageInfo& info)
    {
        return std::shared_ptr<Image>(new Image(info));
    }

    std::shared_ptr<Image> Image::create(const ImageInfo& info, uint8_t* externalData)
    {
        return std::shared_ptr<Image>(new Image(info, externalData));
    }

    std::shared_ptr<Image> Image::create(const Size2I& size, ImageType type)
    {
        return std::shared_ptr<Image>(new Image(ImageInfo(size, type)));
    }

    std::shared_ptr<Image> Image::create(int w, int h, ImageType type)
    {
        return std::shared_ptr<Image>(new Image(ImageInfo(w, h, type)));
    }

    void Image::setTags(const ImageTags& tags)
    {
        _tags = tags;
    }

    void Image::zero()
    {
        memset(_data, 0, _byteCount);
    }

    size_t Image::getObjectCount()
    {
        return objectCount;
    }

    size_t Image::getTotalByteCount()
    {
        return totalByteCount;
    }

    void Image::setBufferPoolMax(size_t value)
    {
        auto& pool = getBufferPool();
        std::unique_lock<std::mutex> lock(pool.mutex);
        pool.maxByteCount = value;
        trimBufferPool(pool);
    }

    size_t Image::getBufferPoolMax()
    {
        auto& pool = getBufferPool();
        std::unique_lock<std::mutex> lock(pool.mutex);
        return pool.maxByteCount;
    }

    size_t Image::getBufferPoolByteCount()
    {
        auto& pool = getBufferPool();
        std::unique_lock<std::mutex> lock(pool.mutex);
        return pool.byteCount;
    }

    void Image::clearBufferPool()
    {
        auto& pool = getBufferPool();
        std::unique_lock<std::mutex> lock(pool.mutex);
        for (auto& i : pool.buffers)
        {
            for (auto* data : i.second)
            {
                delete[] data;
            }
        }
        pool.buffers.clear();
        pool.byteCount = 0;
    }

    void to_json(nlohmann::json& json, const ImageMirror& in)
    {
        json["X"] = in.x;
        json["Y"] = in.y;
    }

    void from_json(const nlohmann::json& json, ImageMirror& out)
    {
        json.at("X").get_to(out.x);
        json.at("Y").get_to(out.y);
    }
}
