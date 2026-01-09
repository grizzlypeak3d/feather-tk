// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace ftk
{
    //! Base class for command line options.
    class FTK_API_TYPE ICmdLineOption : public std::enable_shared_from_this<ICmdLineOption>
    {
        FTK_NON_COPYABLE(ICmdLineOption);

    protected:
        ICmdLineOption(
            const std::vector<std::string>& names,
            const std::string& help,
            const std::string& group = std::string());

    public:
        FTK_API virtual ~ICmdLineOption() = 0;
            
        //! Get the help.
        const std::string& getHelp() const;

        //! Get the group.
        const std::string& getGroup() const;

        //! Parse the option.
        FTK_API virtual void parse(std::vector<std::string>& args) = 0;

        //! Get whether the option was found.
        bool found() const;

        //! Get the option name that was matched.
        const std::string& getMatchedName() const;

    protected:
        std::vector<std::string> _names;
        std::string _help;
        std::string _group;
        std::string _matchedName;
    };

    //! Command line flag option.
    class FTK_API_TYPE CmdLineFlagOption : public ICmdLineOption
    {
    protected:
        CmdLineFlagOption(
            const std::vector<std::string>& names,
            const std::string& help,
            const std::string& group);

    public:
        //! Create a new command line flag option.
        FTK_API static std::shared_ptr<CmdLineFlagOption> create(
            const std::vector<std::string>& names,
            const std::string& help,
            const std::string& group = std::string());

        FTK_API void parse(std::vector<std::string>& args) override;
    };

    //! Command line value option.
    template<typename T>
    class CmdLineValueOption : public ICmdLineOption
    {
    protected:
        CmdLineValueOption(
            const std::vector<std::string>& names,
            const std::string& help,
            const std::string& group,
            const std::optional<T>& defaultValue,
            const std::string& possibleValues);

    public:
        //! Create a new command line value option.
        static std::shared_ptr<CmdLineValueOption<T> > create(
            const std::vector<std::string>& names,
            const std::string& help,
            const std::string& group = std::string(),
            const std::optional<T>& defaultValue = std::optional<T>(),
            const std::string& possibleValues = std::string());

        //! Get whether there is a value. The value is either from the command
        //! line or the default value.
        bool hasValue() const;

        //! Get the value.
        const T& getValue() const;

        void parse(std::vector<std::string>& args) override;

    private:
        std::optional<T> _value;
        std::optional<T> _defaultValue;
        std::string _possibleValues;
    };

    //! Base class for command line arguments.
    class FTK_API_TYPE ICmdLineArg : public std::enable_shared_from_this<ICmdLineArg>
    {
    protected:
        ICmdLineArg(
            const std::string& name,
            const std::string& help,
            bool optional);

    public:
        FTK_API virtual ~ICmdLineArg() = 0;

        //! Parse the argument.
        FTK_API virtual void parse(std::vector<std::string>& args) = 0;

        //! Get the argument name.
        const std::string& getName() const;

        //! Get the help.
        const std::string& getHelp() const;

        //! Get whether this argument is optional.
        bool isOptional() const;

    protected:
        std::string _name;
        std::string _help;
        bool _optional = false;
    };

    //! Command line value argument.
    template<typename T>
    class CmdLineValueArg : public ICmdLineArg
    {
    protected:
        CmdLineValueArg(
            const std::string& name,
            const std::string& help,
            bool optional);

    public:
        //! Create a new command line argument.
        static std::shared_ptr<CmdLineValueArg<T> > create(
            const std::string& name,
            const std::string& help,
            bool optional = false);

        //! Get whether there is a value.
        bool hasValue() const;

        //! Get the value.
        const T& getValue() const;

        void parse(std::vector<std::string>& args) override;

    private:
        std::optional<T> _value;
    };

    //! Command line list argument. This argument should come last since it
    //! will take all of the remaining arguments.
    template<typename T>
    class CmdLineListArg : public ICmdLineArg
    {
    protected:
        CmdLineListArg(
            const std::string& name,
            const std::string& help,
            bool optional);

    public:
        //! Create a new command line argument.
        static std::shared_ptr<CmdLineListArg<T> > create(
            const std::string& name,
            const std::string& help,
            bool optional = false);

        //! Get the list.
        const std::vector<T>& getList() const;

        void parse(std::vector<std::string>& args) override;

    private:
        std::vector<T> _list;
    };

    FTK_API bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, std::string&);
    FTK_API bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, bool&);
    FTK_API bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, int&);
    FTK_API bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, float&);
    FTK_API bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, double&);
    template<typename T>
    bool cmdLineParse(std::vector<std::string>&, std::vector<std::string>::iterator&, T&);
}

#include <ftk/Core/CmdLineInline.h>
