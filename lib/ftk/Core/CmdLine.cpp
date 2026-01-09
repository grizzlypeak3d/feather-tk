// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/CmdLine.h>

#include <cstdlib>

namespace ftk
{
    ICmdLineOption::ICmdLineOption(
        const std::vector<std::string>& names,
        const std::string& help,
        const std::string& group) :
        _names(names),
        _help(help),
        _group(group)
    {}

    ICmdLineOption::~ICmdLineOption()
    {}

    CmdLineFlagOption::CmdLineFlagOption(
        const std::vector<std::string>& names,
        const std::string& help,
        const std::string& group) :
        ICmdLineOption(names, help, group)
    {
        _help = join(_names, ", ") + " - " + help;
    }

    std::shared_ptr<CmdLineFlagOption> CmdLineFlagOption::create(
        const std::vector<std::string>& names,
        const std::string& help,
        const std::string& group)
    {
        return std::shared_ptr<CmdLineFlagOption>(new CmdLineFlagOption(names, help, group));
    }

    void CmdLineFlagOption::parse(std::vector<std::string>& args)
    {
        for (const auto& name : _names)
        {
            auto i = std::find(args.begin(), args.end(), name);
            if (i != args.end())
            {
                _matchedName = name;
                i = args.erase(i);
            }
        }
    }

    bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, std::string& value)
    {
        bool out = false;
        if (it != args.end())
        {
            value = *it;
            it = args.erase(it);
            out = true;
        }
        return out;
    }

    bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, bool& value)
    {
        bool out = false;
        if (it != args.end())
        {
            value = std::atoi(it->c_str());
            it = args.erase(it);
            out = true;
        }
        return out;
    }

    bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, int& value)
    {
        bool out = false;
        if (it != args.end())
        {
            value = std::atoi(it->c_str());
            it = args.erase(it);
            out = true;
        }
        return out;
    }

    bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, float& value)
    {
        bool out = false;
        if (it != args.end())
        {
            value = std::atof(it->c_str());
            it = args.erase(it);
            out = true;
        }
        return out;
    }

    bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, double& value)
    {
        bool out = false;
        if (it != args.end())
        {
            value = std::atof(it->c_str());
            it = args.erase(it);
            out = true;
        }
        return out;
    }
}