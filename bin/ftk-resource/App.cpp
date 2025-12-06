// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>

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
        file.read(data.data(), size);
    }

    const std::string var = std::filesystem::path(_input).stem().string();
    {
        std::filesystem::path sourceOutput = _output;
        sourceOutput.replace_extension(".cpp");
        std::cout << "Source output: " << sourceOutput.string() << std::endl;
        std::string tmp;
        tmp.append("#include <cstdint>\n");
        tmp.append("#include <vector>\n");
        tmp.append("\n");
        tmp.append("namespace " + _namespace + "\n");
        tmp.append("{\n");
        tmp.append("    std::vector<uint8_t> " + var + " = \n");
        tmp.append("    {\n");
        const size_t columns = 30;
        for (size_t i = 0; i < size; i += columns)
        {
            tmp.append("        ");
            for (size_t j = i; j < i + columns && j < size; ++j)
            {
                std::stringstream ss;
                ss << static_cast<int>(data[j]) << ", ";
                tmp.append(ss.str());
            }
            tmp.append("\n");
        }
        tmp.append("    };\n");
        tmp.append("}\n");
        File file(sourceOutput.string(), "wb");
        file.write((const uint8_t*)tmp.c_str(), tmp.size());
    }

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<float> diff = now - _startTime;
    std::cout << "Seconds elapsed:" << std::setprecision(2) << diff.count() << std::endl;
}
