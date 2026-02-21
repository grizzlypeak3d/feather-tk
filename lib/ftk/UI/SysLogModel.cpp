// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the ibis project.

#include <ftk/UI/SysLogModel.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

namespace ftk
{
    namespace
    {
        const size_t messagesMax = 100;
        const size_t logMax = 1000;
    }

    struct SysLogModel::Private
    {
        std::shared_ptr<ObservableList<std::string> > messages;
        std::shared_ptr<ObservableList<std::string> > log;
        std::shared_ptr<ListObserver<LogItem> > logObserver;
    };

    void SysLogModel::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();

        p.messages = ObservableList<std::string>::create();

        p.log = ObservableList<std::string>::create();

        auto logSystem = context->getLogSystem();
        p.logObserver = ListObserver<LogItem>::create(
            logSystem->observeLogItems(),
            [this](const std::vector<LogItem>& items)
            {
                FTK_P();
                std::list<std::string> messages(p.messages->get().begin(), p.messages->get().end());
                std::list<std::string> log(p.log->get().begin(), p.log->get().end());
                for (const auto& item : items)
                {
                    switch (item.type)
                    {
                    case LogType::Warning:
                    case LogType::Error:
                        for (const auto& line : split(getLabel(item), '\n', SplitOptions::KeepEmpty))
                        {
                            messages.push_back(line);
                        }
                        break;
                    default: break;
                    }

                    for (const auto& line : split(getLabel(item), '\n', SplitOptions::KeepEmpty))
                    {
                        log.push_back(line);
                    }
                }

                while (messages.size() > messagesMax)
                {
                    messages.pop_front();
                }
                while (log.size() > logMax)
                {
                    log.pop_front();
                }

                p.messages->setIfChanged(std::vector<std::string>(messages.begin(), messages.end()));
                p.log->setIfChanged(std::vector<std::string>(log.begin(), log.end()));
            });
    }

    SysLogModel::SysLogModel() :
        _p(new Private)
    {}

    SysLogModel::~SysLogModel()
    {}

    std::shared_ptr<SysLogModel> SysLogModel::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<SysLogModel>(new SysLogModel);
        out->_init(context);
        return out;
    }

    std::shared_ptr<IObservableList<std::string> > SysLogModel::observeMessages() const
    {
        return _p->messages;
    }

    std::shared_ptr<IObservableList<std::string> > SysLogModel::observeLog() const
    {
        return _p->log;
    }
}
