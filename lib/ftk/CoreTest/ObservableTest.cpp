// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/ObservableTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/ObservableList.h>
#include <ftk/Core/ObservableMap.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    namespace core_test
    {
        ObservableTest::ObservableTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::ObservableTest")
        {}

        ObservableTest::~ObservableTest()
        {}

        std::shared_ptr<ObservableTest> ObservableTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ObservableTest>(new ObservableTest(context));
        }
        
        void ObservableTest::run()
        {
            _value();
            _list();
            _map();
        }
        
        void ObservableTest::_value()
        {
            auto ovalue = Observable<bool>::create();
            ovalue = Observable<bool>::create(false);
            FTK_CHECK(!ovalue->get());                
            {
                auto observer = Observer<bool>::create(ovalue, [](bool) {});
                FTK_CHECK(ovalue->getObserversCount());                    
                {
                    bool value = false;
                    auto observer1 = Observer<bool>::create(
                        ovalue,
                        [&value](bool v)
                        {
                            value = v;
                        });

                    FTK_CHECK(ovalue->setIfChanged(true));
                    FTK_CHECK(!ovalue->setIfChanged(true));
                    FTK_CHECK(value);

                    FTK_CHECK(ovalue->setIfChanged(false));
                    FTK_CHECK(!value);

                    ovalue->setAlways(false);
                    FTK_CHECK(!value);
                    ovalue->setAlways(true);
                    FTK_CHECK(value);
                }
            }
            FTK_CHECK(!ovalue->getObserversCount());
        }

        void ObservableTest::_list()
        {
            auto olist = ObservableList<int>::create();
            std::vector<int> list = { 0, 0 };
            olist = ObservableList<int>::create(list);
            FTK_CHECK(list == olist->get());                
            {
                auto observer = ListObserver<int>::create(
                    olist,
                    [](const std::vector<int>&) {});
                FTK_CHECK(olist->getObserversCount());                    
                {
                    auto observer1 = ListObserver<int>::create(
                        olist,
                        [&list](const std::vector<int>& value)
                        {
                            list = value;
                        });

                    FTK_CHECK(olist->setIfChanged({ 1, 0 }));
                    FTK_CHECK(!olist->setIfChanged({ 1, 0 }));
                    FTK_CHECK(!list.empty() && list[0]);

                    FTK_CHECK(olist->setIfChanged({ 0, 0 }));
                    FTK_CHECK(!list.empty() && 0 == list[0]);
                    olist->setAlways({ 0, 0 });
                    FTK_CHECK(!list.empty() && 0 == list[0]);
                    olist->setAlways({ 1, 0 });
                    FTK_CHECK(!list.empty() && 1 == list[0]);

                    olist->clear();
                    FTK_CHECK(list.empty());

                    olist->pushBack(1);
                    olist->pushBack(2);
                    FTK_CHECK(2 == olist->getSize());
                    FTK_CHECK(!olist->isEmpty());
                    FTK_CHECK(2 == olist->getItem(1));
                    FTK_CHECK(olist->contains(2));
                    FTK_CHECK(1 == olist->indexOf(2));
                    FTK_CHECK(!list.empty() && 1 == list[0]);
                    olist->pushBack({ 3, 4 });
                    FTK_CHECK(4 == olist->getSize());

                    olist->setItem(0, 1);
                    FTK_CHECK(!olist->setItemOnlyIfChanged(0, 1));
                    FTK_CHECK(olist->setItemOnlyIfChanged(0, 2));
                    olist->replaceItems(2, 4, { 5, 6 });
                    FTK_CHECK(5 == olist->getItem(2));
                    FTK_CHECK(6 == olist->getItem(3));
                    olist->removeItems(2, 4);
                    olist->removeItem(1);
                    olist->removeItem(0);
                    FTK_CHECK(list.empty());
                }
            }
            FTK_CHECK(!olist->getObserversCount());
        }
        
        void ObservableTest::_map()
        {
            auto omap = ObservableMap<int, bool>::create();
            std::map<int, bool> map = { { 0, false }, { 0, false } };
            omap = ObservableMap<int, bool>::create(map);
            FTK_CHECK(map == omap->get());                
            {
                auto observer = MapObserver<int, bool>::create(
                    omap,
                    [](const std::map<int, bool>&) {});
                FTK_CHECK(omap->getObserversCount());                    
                {
                    auto observer1 = MapObserver<int, bool>::create(
                        omap,
                        [&map](const std::map<int, bool>& value)
                        {
                            map = value;
                        });

                    FTK_CHECK(omap->setIfChanged({ { 0, true }, { 1, false } }));
                    FTK_CHECK(!omap->setIfChanged({ { 0, true }, { 1, false } }));
                    FTK_CHECK(!map.empty() && map[0]);

                    FTK_CHECK(omap->setIfChanged({ { 0, false }, { 1, false } }));
                    FTK_CHECK(!map.empty() && !map[0]);

                    omap->setAlways({ { 0, false }, { 1, false } });
                    FTK_CHECK(!map.empty() && !map[0]);
                    omap->setAlways({ { 0, true }, { 1, false } });
                    FTK_CHECK(!map.empty() && map[0]);

                    omap->clear();
                    FTK_CHECK(map.empty());

                    omap->setItem(0, false);
                    omap->setItem(1, true);
                    FTK_CHECK(2 == omap->getSize());
                    FTK_CHECK(!omap->isEmpty());
                    FTK_CHECK(omap->hasKey(1));
                    FTK_CHECK(omap->getKeys() == std::vector<int>({ 0, 1 }));
                    FTK_CHECK(omap->getItem(1));
                    FTK_CHECK(!map.empty() && map[1]);

                    omap->setItem(1, false);
                    FTK_CHECK(!omap->setItemOnlyIfChanged(1, false));
                    FTK_CHECK(omap->setItemOnlyIfChanged(1, true));
                }
            }
            FTK_CHECK(!omap->getObserversCount());
        }
    }
}

