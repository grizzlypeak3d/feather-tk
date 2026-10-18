// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

// For the Objective-C++ of the iOS build only.

#include <string>

#if defined(__OBJC__)
@class UIViewController;
#endif // __OBJC__

namespace ftk
{
#if defined(__OBJC__)
    //! The view controller to present over: the one on top on the
    //! device's own screen, where the fingers are, rather than on a
    //! television connected beside it, which has no way to dismiss what
    //! is put there.
    UIViewController* topViewController();
#endif // __OBJC__

    //! Open a URL on iOS: a web page in the browser, and a page among the
    //! application's own files -- the documentation, the legal notices --
    //! in a sheet over the application, since the browser cannot read
    //! another application's files. Set as the handler of openURL() as
    //! the application starts.
    void openURLIOS(const std::string&);
}
