// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>

namespace ftk
{
    inline const std::string& ICmdLineOption::getHelp() const
    {
        return _help;
    }

    inline const std::string& ICmdLineOption::getGroup() const
    {
        return _group;
    }

    inline bool ICmdLineOption::found() const
    {
        return !_matchedName.empty();
    }

    inline const std::string& ICmdLineOption::getMatchedName() const
    {
        return _matchedName;
    }

    template<typename T>
    inline CmdLineValueOption<T>::CmdLineValueOption(
        const std::vector<std::string>& names,
        const std::string& help,
        const std::string& group,
        const std::optional<T>& defaultValue,
        const std::string& possibleValues) :
        ICmdLineOption(names, help, group),
        _value(defaultValue),
        _defaultValue(defaultValue),
        _possibleValues(possibleValues)
    {
        _help = join(_names, ", ") + " (value) - " + help;
        if (_defaultValue.has_value())
        {
            std::stringstream ss;
            ss << _defaultValue.value();
            _help += " Default: \"" + ss.str() + "\".";
        }
        if (!_possibleValues.empty())
        {
            _help += " Options: " + _possibleValues + ".";
        }
    }

    template<typename T>
    inline std::shared_ptr<CmdLineValueOption<T> > CmdLineValueOption<T>::create(
        const std::vector<std::string>& names,
        const std::string& help,
        const std::string& group,
        const std::optional<T>& defaultValue,
        const std::string& possibleValues)
    {
        return std::shared_ptr<CmdLineValueOption<T> >(new CmdLineValueOption<T>(
            names,
            help,
            group,
            defaultValue,
            possibleValues));
    }

    template<typename T>
    bool CmdLineValueOption<T>::hasValue() const
    {
        return _value.has_value();
    }

    template<typename T>
    const T& CmdLineValueOption<T>::getValue() const
    {
        return _value.value();
    }

    template<typename T>
    inline void CmdLineValueOption<T>::parse(std::vector<std::string>& args)
    {
        for (const auto& name : _names)
        {
            auto i = std::find(args.begin(), args.end(), name);
            if (i != args.end())
            {
                _matchedName = name;
                i = args.erase(i);
                _value = T();
                if (!cmdLineParse(args, i, *_value))
                {
                    throw ParseError();
                }
            }
        }
    }

    template<>
    inline void CmdLineValueOption<std::string>::parse(std::vector<std::string>& args)
    {
        for (const auto& name : _names)
        {
            auto i = std::find(args.begin(), args.end(), name);
            if (i != args.end())
            {
                _matchedName = name;
                i = args.erase(i);
                if (i != args.end())
                {
                    _value = *i;
                    i = args.erase(i);
                }
                else
                {
                    throw ParseError();
                }
            }
        }
    }

    inline ICmdLineArg::ICmdLineArg(
        const std::string& name,
        const std::string& help,
        bool optional) :
        _name(name),
        _help(help),
        _optional(optional)
    {}

    inline ICmdLineArg::~ICmdLineArg()
    {}

    inline const std::string& ICmdLineArg::getName() const
    {
        return _name;
    }

    inline const std::string& ICmdLineArg::getHelp() const
    {
        return _help;
    }

    inline bool ICmdLineArg::isOptional() const
    {
        return _optional;
    }

    template<typename T>
    inline CmdLineValueArg<T>::CmdLineValueArg(
        const std::string& name,
        const std::string& help,
        bool optional) :
        ICmdLineArg(name, help, optional)
    {}

    template<typename T>
    inline std::shared_ptr<CmdLineValueArg<T> > CmdLineValueArg<T>::create(
        const std::string& name,
        const std::string& help,
        bool optional)
    {
        return std::shared_ptr<CmdLineValueArg<T> >(new CmdLineValueArg<T>(name, help, optional));
    }

    template<typename T>
    bool CmdLineValueArg<T>::hasValue() const
    {
        return _value.has_value();
    }

    template<typename T>
    const T& CmdLineValueArg<T>::getValue() const
    {
        return _value.value();
    }

    template<typename T>
    inline void CmdLineValueArg<T>::parse(std::vector<std::string>& args)
    {
        auto i = args.begin();
        _value = T();
        if (!cmdLineParse(args, i, *_value))
        {
            throw ParseError();
        }
    }

    template<>
    inline void CmdLineValueArg<std::string>::parse(std::vector<std::string>& args)
    {
        auto i = args.begin();
        if (i != args.end())
        {
            _value = *i;
            i = args.erase(i);
        }
        else
        {
            throw ParseError();
        }
    }

    template<typename T>
    inline CmdLineListArg<T>::CmdLineListArg(
        const std::string& name,
        const std::string& help,
        bool optional) :
        ICmdLineArg(name, help, optional)
    { }

    template<typename T>
    inline std::shared_ptr<CmdLineListArg<T> > CmdLineListArg<T>::create(
        const std::string& name,
        const std::string& help,
        bool optional)
    {
        return std::shared_ptr<CmdLineListArg<T> >(new CmdLineListArg<T>(name, help, optional));
    }

    template<typename T>
    inline const std::vector<T>& CmdLineListArg<T>::getList() const
    {
        return _list;
    }

    template<typename T>
    inline void CmdLineListArg<T>::parse(std::vector<std::string>& args)
    {
        auto i = args.begin();
        while (i != args.end())
        {
            T value;
            if (!cmdLineParse(args, i, value))
            {
                throw ParseError();
            }
            _list.push_back(value);
        }
    }

    template<typename T>
    inline bool cmdLineParse(std::vector<std::string>& args, std::vector<std::string>::iterator& it, T& value)
    {
        bool out = false;
        if (it != args.end())
        {
            from_string(*it, value);
            it = args.erase(it);
            out = true;
        }
        return out;
    }
}
