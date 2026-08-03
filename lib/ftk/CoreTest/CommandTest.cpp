// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/CommandTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Command.h>

namespace ftk
{
    namespace core_test
    {
        CommandTest::CommandTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::CommandTest")
        {}

        CommandTest::~CommandTest()
        {}

        std::shared_ptr<CommandTest> CommandTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<CommandTest>(new CommandTest(context));
        }
        
        namespace
        {
            struct Data
            {
                int value = 0;
            };

            class AddCommand : public ICommand
            {
            public:
                AddCommand(
                    int value,
                    const std::shared_ptr<Data>& data) :
                    _value(value),
                    _data(data)
                {}

                virtual ~AddCommand() {}

                void exec() override
                {
                    _data->value += _value;
                }
                void undo() override
                {
                    _data->value -= _value;
                }

            private:
                int _value = 0;
                std::shared_ptr<Data> _data;
            };
        }

        void CommandTest::run()
        {
            {
                auto data = std::make_shared<Data>();

                auto commandStack = CommandStack::create();
                bool hasUndo = false;
                bool hasRedo = false;
                auto undoObserver = Observer<bool>::create(
                    commandStack->observeHasUndo(),
                    [&hasUndo](bool value)
                    {
                        hasUndo = value;
                    });
                auto redoObserver = Observer<bool>::create(
                    commandStack->observeHasRedo(),
                    [&hasRedo](bool value)
                    {
                        hasRedo = value;
                    });
                FTK_CHECK(!hasUndo);
                FTK_CHECK(!hasRedo);

                commandStack->push(std::make_shared<AddCommand>(1, data));
                commandStack->push(std::make_shared<AddCommand>(2, data));
                commandStack->push(std::make_shared<AddCommand>(3, data));
                FTK_CHECK(6 == data->value);
                FTK_CHECK(hasUndo);

                commandStack->undo();
                FTK_CHECK(3 == data->value);
                FTK_CHECK(hasRedo);
                commandStack->redo();
                FTK_CHECK(6 == data->value);
                commandStack->undo();
                FTK_CHECK(3 == data->value);

                commandStack->undo();
                FTK_CHECK(1 == data->value);
                commandStack->undo();
                FTK_CHECK(0 == data->value);
                FTK_CHECK(!hasUndo);

                commandStack->redo();
                FTK_CHECK(1 == data->value);
                commandStack->redo();
                FTK_CHECK(3 == data->value);
                commandStack->redo();
                FTK_CHECK(6 == data->value);

                commandStack->clear();
                FTK_CHECK(!hasUndo);
                FTK_CHECK(!hasRedo);

                commandStack->push(std::make_shared<AddCommand>(1, data));
                commandStack->undo();
                FTK_CHECK(hasRedo);
                commandStack->push(std::make_shared<AddCommand>(2, data));
                FTK_CHECK(!hasRedo);
            }
        }
    }
}

