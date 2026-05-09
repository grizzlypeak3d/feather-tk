// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/TestLib/ITest.h>

#include <iostream>

namespace ftk
{
    namespace test
    {
        ITest::ITest(
            const std::shared_ptr<Context>& context,
            const std::string& name) :
            _context(context),
            _name(name)
        {}

        ITest::~ITest()
        {}

        const std::string& ITest::getName() const
        {
            return _name;
        }
        
        void ITest::_print(const std::string& value)
        {
            std::cout << _name << ": " << value << std::endl;
        }
        
        void ITest::_error(const std::string& value)
        {
            std::cout << _name << " ERROR: " << value << std::endl;
        }
    }
}

