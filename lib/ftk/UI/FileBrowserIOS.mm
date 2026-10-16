// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "FileBrowserIOSPrivate.h"

#import <UIKit/UIKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include <functional>
#include <vector>

// The picker's delegate: holds the callback, and the picker itself while
// it is up, since the picker holds its delegate only weakly.
@interface FTKDocumentPickerDelegate : NSObject <UIDocumentPickerDelegate>
@property (nonatomic, assign) std::function<void(const std::vector<ftk::Path>&)> callback;
@property (nonatomic, strong) UIDocumentPickerViewController* picker;
@end

namespace
{
    // The delegate of the picker that is up, if one is, and the files
    // chosen so far: a file chosen in place is readable only while its
    // security scope is held, and it is held until the application ends,
    // since what was opened may be read again at any time after.
    FTKDocumentPickerDelegate* delegate = nil;
    NSMutableArray<NSURL*>* accessed = nil;

    // The files chosen lately, as bookmarks that survive the run: the
    // grant for a file picked in place ends with the application, and
    // a bookmark is what gets it back, so that the file can be opened
    // again from the recent files without the picker.
    NSString* const bookmarksKey = @"ftk.FileBrowser.Bookmarks";
    const NSUInteger bookmarksMax = 20;

    void access(NSURL* url)
    {
        if ([url startAccessingSecurityScopedResource])
        {
            if (!accessed)
            {
                accessed = [NSMutableArray array];
            }
            [accessed addObject:url];
        }
    }

    void remember(NSArray<NSURL*>* urls)
    {
        NSUserDefaults* defaults = NSUserDefaults.standardUserDefaults;
        NSMutableArray* list = [[defaults arrayForKey:bookmarksKey] mutableCopy];
        if (!list)
        {
            list = [NSMutableArray array];
        }
        for (NSURL* url in urls)
        {
            if (NSData* data = [url
                bookmarkDataWithOptions:0
                includingResourceValuesForKeys:nil
                relativeToURL:nil
                error:nil])
            {
                [list insertObject:data atIndex:0];
            }
        }
        while (list.count > bookmarksMax)
        {
            [list removeLastObject];
        }
        [defaults setObject:list forKey:bookmarksKey];
    }

    UIViewController* topViewController()
    {
        // The picker goes on the device's own screen, where the fingers
        // are. The connected scenes are an unordered set, and with a
        // television on the adapter the first one found could be the
        // secondary window's, which has no way to dismiss the picker.
        UIWindow* window = nil;
        UIWindow* other = nil;
        for (UIScene* scene in UIApplication.sharedApplication.connectedScenes)
        {
            if ([scene isKindOfClass:[UIWindowScene class]])
            {
                UIWindowScene* windowScene = (UIWindowScene*)scene;
                UIWindow* sceneWindow = windowScene.keyWindow;
                if (!sceneWindow)
                {
                    sceneWindow = windowScene.windows.firstObject;
                }
                if (!sceneWindow)
                {
                    continue;
                }
                if (windowScene.screen == UIScreen.mainScreen)
                {
                    window = sceneWindow;
                    break;
                }
                if (!other)
                {
                    other = sceneWindow;
                }
            }
        }
        if (!window)
        {
            window = other;
        }
        UIViewController* out = window.rootViewController;
        while (out.presentedViewController)
        {
            out = out.presentedViewController;
        }
        return out;
    }
}

@implementation FTKDocumentPickerDelegate

- (void)documentPicker:(UIDocumentPickerViewController*)controller
    didPickDocumentsAtURLs:(NSArray<NSURL*>*)urls
{
    // Kept for the length of this: the picker holds its delegate weakly,
    // and letting go of the one strong reference below freed the object
    // this is running on.
    FTKDocumentPickerDelegate* keep = self;
    std::vector<ftk::Path> paths;
    for (NSURL* url in urls)
    {
        access(url);
        paths.push_back(ftk::Path(std::string([url fileSystemRepresentation])));
    }
    remember(urls);
    const auto callback = keep.callback;
    if (!paths.empty() && callback)
    {
        callback(paths);
    }
    keep.picker = nil;
    delegate = nil;
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController*)controller
{
    FTKDocumentPickerDelegate* keep = self;
    keep.picker = nil;
    delegate = nil;
}

@end

namespace ftk
{
    void restoreDocumentAccess()
    {
        // Each bookmark resolved and its grant taken up again; the list is
        // written back without the ones that no longer resolve, and
        // without a file listed twice.
        NSUserDefaults* defaults = NSUserDefaults.standardUserDefaults;
        NSMutableArray* kept = [NSMutableArray array];
        NSMutableSet<NSString*>* seen = [NSMutableSet set];
        NSArray* list = [defaults arrayForKey:bookmarksKey];
        for (id item in list)
        {
            // Only what was written: a list from an earlier version, or a
            // damaged one, is not trusted with a message.
            if (![item isKindOfClass:[NSData class]])
            {
                continue;
            }
            NSURL* url = nil;
            @try
            {
                BOOL stale = NO;
                url = [NSURL
                    URLByResolvingBookmarkData:(NSData*)item
                    options:NSURLBookmarkResolutionWithoutUI
                    relativeToURL:nil
                    bookmarkDataIsStale:&stale
                    error:nil];
            }
            @catch (NSException*)
            {
                url = nil;
            }
            NSString* path = url ? url.path : nil;
            if (!path || [seen containsObject:path])
            {
                continue;
            }
            [seen addObject:path];
            access(url);
            [kept addObject:item];
        }
        [defaults setObject:kept forKey:bookmarksKey];
    }

    void openDocumentPicker(
        const FileBrowserOpenOptions& options,
        const std::function<void(const std::vector<Path>&)>& callback)
    {
        // One at a time: a second request while one is up would stack a
        // picker over a picker.
        if (delegate)
        {
            return;
        }
        NSArray<UTType*>* types = nil;
        if (FileBrowserMode::Dir == options.mode)
        {
            types = @[ UTTypeFolder ];
        }
        else
        {
            // Every file, as the extension filter would list them: the
            // media the application reads has no type the system knows
            // under one name, and a type for each extension would still
            // leave out what the application can open and the system has
            // never heard of.
            types = @[ UTTypeItem ];
        }
        UIDocumentPickerViewController* picker =
            [[UIDocumentPickerViewController alloc]
                initForOpeningContentTypes:types
                asCopy:NO];
        picker.allowsMultipleSelection =
            FileBrowserMode::Open == options.mode && options.multiple;
        if (!options.path.empty())
        {
            picker.directoryURL = [NSURL fileURLWithPath:
                [NSString stringWithUTF8String:options.path.c_str()]];
        }
        delegate = [FTKDocumentPickerDelegate new];
        delegate.callback = callback;
        delegate.picker = picker;
        picker.delegate = delegate;
        [topViewController() presentViewController:picker animated:YES completion:nil];
    }
}
