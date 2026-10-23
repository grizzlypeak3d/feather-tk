// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "FileBrowserIOSPrivate.h"
#include "UIKitPrivate.h"

#import <UIKit/UIKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include <functional>
#include <vector>

// The picker's delegate: holds the callback, and the picker itself while
// it is up, since the picker holds its delegate only weakly.
@interface FTKDocumentPickerDelegate : NSObject <UIDocumentPickerDelegate>
@property (nonatomic, assign) std::function<void(const std::vector<ftk::Path>&)> callback;
@property (nonatomic, assign) std::function<void(void)> cancelled;
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
    // again from the recent files without the picker. Folders are kept
    // in a list of their own: a project's folder grants everything in
    // it, and is not to be pushed out by the files picked since.
    NSString* const bookmarksKey = @"ftk.FileBrowser.Bookmarks";
    NSString* const folderBookmarksKey = @"ftk.FileBrowser.FolderBookmarks";
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

    bool isFolder(NSURL* url)
    {
        NSNumber* directory = nil;
        if ([url getResourceValue:&directory forKey:NSURLIsDirectoryKey error:nil] &&
            directory)
        {
            return directory.boolValue;
        }
        return url.hasDirectoryPath;
    }

    void remember(NSArray<NSURL*>* urls)
    {
        NSUserDefaults* defaults = NSUserDefaults.standardUserDefaults;
        NSMutableArray* files = [[defaults arrayForKey:bookmarksKey] mutableCopy];
        NSMutableArray* folders = [[defaults arrayForKey:folderBookmarksKey] mutableCopy];
        if (!files)
        {
            files = [NSMutableArray array];
        }
        if (!folders)
        {
            folders = [NSMutableArray array];
        }
        for (NSURL* url in urls)
        {
            if (NSData* data = [url
                bookmarkDataWithOptions:0
                includingResourceValuesForKeys:nil
                relativeToURL:nil
                error:nil])
            {
                [isFolder(url) ? folders : files insertObject:data atIndex:0];
            }
        }
        while (files.count > bookmarksMax)
        {
            [files removeLastObject];
        }
        while (folders.count > bookmarksMax)
        {
            [folders removeLastObject];
        }
        [defaults setObject:files forKey:bookmarksKey];
        [defaults setObject:folders forKey:folderBookmarksKey];
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
    const auto cancelled = keep.cancelled;
    keep.picker = nil;
    delegate = nil;
    if (cancelled)
    {
        cancelled();
    }
}

@end

namespace ftk
{
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

    void restoreDocumentAccess()
    {
        // Each bookmark resolved and its grant taken up again; the lists
        // are written back without the ones that no longer resolve, and
        // without a file listed twice.
        //
        // Resolved off the main thread: a bookmark into a share the Files
        // app mounts is resolved by the file provider, which blocked for
        // half a minute when the share was away, and an application that
        // does not draw for that long is killed by the watchdog. The
        // grants are taken on the main thread once the resolving is done,
        // as a picker's are; a recent file on such a share is not opened
        // until then, which is the wait it would have been anyway.
        NSUserDefaults* defaults = NSUserDefaults.standardUserDefaults;
        NSArray* keys = @[ folderBookmarksKey, bookmarksKey ];
        NSMutableArray* lists = [NSMutableArray array];
        for (NSString* key in keys)
        {
            NSArray* list = [defaults arrayForKey:key];
            [lists addObject:list ? list : @[]];
        }
        dispatch_async(
            dispatch_get_global_queue(QOS_CLASS_UTILITY, 0),
            ^{
                NSMutableArray* keptLists = [NSMutableArray array];
                NSMutableArray<NSURL*>* urls = [NSMutableArray array];
                NSMutableSet<NSString*>* seen = [NSMutableSet set];
                for (NSArray* list in lists)
                {
                    NSMutableArray* kept = [NSMutableArray array];
                    for (id item in list)
                    {
                        // Only what was written: a list from an earlier
                        // version, or a damaged one, is not trusted with a
                        // message.
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
                                options:NSURLBookmarkResolutionWithoutUI |
                                    NSURLBookmarkResolutionWithoutMounting
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
                        [urls addObject:url];
                        [kept addObject:item];
                    }
                    [keptLists addObject:kept];
                }
                dispatch_async(
                    dispatch_get_main_queue(),
                    ^{
                        for (NSURL* url in urls)
                        {
                            access(url);
                        }
                        for (NSUInteger i = 0; i < keys.count; ++i)
                        {
                            [defaults setObject:keptLists[i] forKey:keys[i]];
                        }
                    });
            });
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

    void exportDocument(
        const Path& file,
        const std::function<void(const std::vector<Path>&)>& callback,
        const std::function<void(void)>& cancelled)
    {
        if (delegate)
        {
            return;
        }
        NSURL* url = [NSURL fileURLWithPath:
            [NSString stringWithUTF8String:file.get().c_str()]];
        // Moved rather than copied: the file was written for this, and a
        // copy left behind in the application's files would be a second
        // one to find.
        UIDocumentPickerViewController* picker =
            [[UIDocumentPickerViewController alloc]
                initForExportingURLs:@[ url ]
                asCopy:NO];
        delegate = [FTKDocumentPickerDelegate new];
        delegate.callback = callback;
        delegate.cancelled = cancelled;
        delegate.picker = picker;
        picker.delegate = delegate;
        [topViewController() presentViewController:picker animated:YES completion:nil];
    }
}
