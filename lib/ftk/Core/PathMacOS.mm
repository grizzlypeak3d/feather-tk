// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "PathMacOSPrivate.h"

#import <Foundation/Foundation.h>

std::string getHomePath()
{
    NSString *path = NSHomeDirectoryForUser(NSUserName());
    return std::string([path fileSystemRepresentation]);
}

std::string getDesktopPath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, YES);
    NSString *path = [paths firstObject];
    return std::string([path fileSystemRepresentation]);
}

std::string getDocsPath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths firstObject];
    return std::string([path fileSystemRepresentation]);
}

std::string getDownloadsPath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDownloadsDirectory, NSUserDomainMask, YES);
    NSString *path = [paths firstObject];
    return std::string([path fileSystemRepresentation]);
}
