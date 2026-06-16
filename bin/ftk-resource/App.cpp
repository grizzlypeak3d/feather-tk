// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <vector>

#include <zlib.h>

void App::_init(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: ftk-resource (input file name) (output base name) (namespace)" << std::endl;
        _exit = 1;
    }
    else
    {
        _input = argv[1];
        _output = argv[2];
        _namespace = argv[3];
    }
}

App::~App()
{}
        
std::shared_ptr<App> App::create(int argc, char** argv)
{
    auto out = std::shared_ptr<App>(new App);
    out->_init(argc, argv);
    return out;
}

class File
{
public:
    File(const std::string& fileName, const std::string& mode) :
        _fileName(fileName)
    {
        _f = fopen(fileName.c_str(), mode.c_str());
        if (!_f)
        {
            throw std::runtime_error("Cannot open: " + _fileName);
        }
    }

    ~File()
    {
        if (_f)
        {
            fclose(_f);
            _f = nullptr;
        }
    }

    void read(uint8_t* data, size_t size)
    {
        size_t r = fread(data, size, 1, _f);
        if (r != 1)
        {
            throw std::runtime_error("Cannot read: " + _fileName);
        }
    }

    void write(const uint8_t* data, size_t size)
    {
        size_t r = fwrite(data, size, 1, _f);
        if (r != 1)
        {
            throw std::runtime_error("Cannot write: " + _fileName);
        }
    }

private:
    std::string _fileName;
    FILE* _f = nullptr;
};

void App::run()
{
    _startTime = std::chrono::steady_clock::now();

    size_t size = 0;
    std::vector<uint8_t> data;
    {
        std::cout << "Input: " << _input << std::endl;
        File file(_input, "rb");
        size = std::filesystem::file_size(_input);
        data.resize(size);
        if (size > 0)
        {
            file.read(data.data(), size);
        }
    }

    std::string var = std::filesystem::path(_input).stem().string();
    var = std::regex_replace(var, std::regex("[^A-Za-z0-9_]"), "_");

    // Compress the data. The bytes have to live in the binary either way, so
    // storing them deflated shrinks the executable (fonts roughly halve, SVG
    // icons more); each resource is inflated once when first used. If
    // compression does not help -- tiny or already-compressed data -- the raw
    // bytes are stored instead so nothing is pessimized.
    std::vector<uint8_t> compressed(compressBound(static_cast<uLong>(size)));
    uLongf compressedSize = static_cast<uLongf>(compressed.size());
    bool useCompression = false;
    if (size > 0)
    {
        const int r = compress2(
            compressed.data(),
            &compressedSize,
            data.data(),
            static_cast<uLong>(size),
            Z_BEST_COMPRESSION);
        useCompression = (Z_OK == r) && (compressedSize < size);
    }
    const uint8_t* payload = useCompression ? compressed.data() : data.data();
    const size_t payloadSize = useCompression ? compressedSize : size;
    {
        std::filesystem::path sourceOutput = _output;
        sourceOutput.replace_extension(".cpp");
        std::cout << "Source output: " << sourceOutput.string()
            << (useCompression ? " (compressed " : " (raw ")
            << payloadSize << "/" << size << " bytes)" << std::endl;

        // Emit the payload as a series of small string-literal arrays that are
        // stitched together at load time. String literals are parsed far more
        // cheaply than brace-enclosed initializer lists (which build one AST
        // node per byte), so even multi-megabyte resources -- e.g. CJK fonts --
        // stay fast and light to compile. Each chunk is a single literal under
        // MSVC's per-literal limit (16380 bytes), and bytes are written as
        // 3-digit octal escapes so no escape can run into the following byte.
        const size_t chunkSize = 16000;
        const size_t chunkCount = (payloadSize + chunkSize - 1) / chunkSize;

        std::string tmp;
        tmp.reserve(payloadSize * 4 + chunkCount * 96 + 256);
        tmp.append("#include <cstdint>\n");
        tmp.append("#include <vector>\n");
        if (useCompression)
        {
            tmp.append("#include <zlib.h>\n");
        }
        tmp.append("\n");
        tmp.append("namespace " + _namespace + "\n");
        tmp.append("{\n");
        for (size_t c = 0; c < chunkCount; ++c)
        {
            const size_t begin = c * chunkSize;
            size_t end = begin + chunkSize;
            if (end > payloadSize)
            {
                end = payloadSize;
            }
            tmp.append("    static const unsigned char " + var + "_" +
                std::to_string(c) + "[] =\n        \"");
            for (size_t j = begin; j < end; ++j)
            {
                const unsigned char b = payload[j];
                const char esc[4] =
                {
                    '\\',
                    static_cast<char>('0' + ((b >> 6) & 7)),
                    static_cast<char>('0' + ((b >> 3) & 7)),
                    static_cast<char>('0' + (b & 7))
                };
                tmp.append(esc, 4);
            }
            tmp.append("\";\n");
        }
        tmp.append("    std::vector<uint8_t> " + var + " = []\n");
        tmp.append("    {\n");
        if (useCompression)
        {
            // Gather the deflated bytes, then inflate into the result.
            tmp.append("        std::vector<uint8_t> z;\n");
            tmp.append("        z.reserve(" + std::to_string(payloadSize) + ");\n");
            for (size_t c = 0; c < chunkCount; ++c)
            {
                const std::string n = var + "_" + std::to_string(c);
                tmp.append("        z.insert(z.end(), " + n + ", " + n +
                    " + sizeof(" + n + ") - 1);\n");
            }
            tmp.append("        std::vector<uint8_t> v(" + std::to_string(size) + ");\n");
            tmp.append("        uLongf n = " + std::to_string(size) + ";\n");
            tmp.append("        uncompress(v.data(), &n, z.data(), static_cast<uLong>(z.size()));\n");
            tmp.append("        v.resize(n);\n");
            tmp.append("        return v;\n");
        }
        else
        {
            tmp.append("        std::vector<uint8_t> v;\n");
            tmp.append("        v.reserve(" + std::to_string(size) + ");\n");
            for (size_t c = 0; c < chunkCount; ++c)
            {
                const std::string n = var + "_" + std::to_string(c);
                tmp.append("        v.insert(v.end(), " + n + ", " + n +
                    " + sizeof(" + n + ") - 1);\n");
            }
            tmp.append("        return v;\n");
        }
        tmp.append("    }();\n");
        tmp.append("}\n");

        File file(sourceOutput.string(), "wb");
        file.write((const uint8_t*)tmp.c_str(), tmp.size());
    }

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<float> diff = now - _startTime;
    std::cout << "Seconds elapsed: " << std::setprecision(2) << diff.count() << std::endl;
}
