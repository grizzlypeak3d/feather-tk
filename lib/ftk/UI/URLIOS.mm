// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "UIKitPrivate.h"

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

// A page among the application's own files, in a sheet: the browser on
// iOS cannot read another application's files, so the documentation and
// the legal notices that ship inside the application are shown here.
// Links between the pages stay in the sheet; a link out to the web goes to
// the browser, where the web belongs.
@interface FTKPageViewController : UIViewController <WKNavigationDelegate, WKUIDelegate>
@property (nonatomic, strong) NSURL* url;
@property (nonatomic, strong) WKWebView* webView;
@end

@implementation FTKPageViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.webView = [[WKWebView alloc]
        initWithFrame:self.view.bounds
        configuration:[[WKWebViewConfiguration alloc] init]];
    self.webView.autoresizingMask =
        UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.webView.navigationDelegate = self;
    self.webView.UIDelegate = self;
    self.webView.allowsBackForwardNavigationGestures = YES;
    [self.view addSubview:self.webView];

    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]
        initWithBarButtonSystemItem:UIBarButtonSystemItemDone
        target:self
        action:@selector(done)];

    // Read access to the directory the page is in, so that its
    // stylesheet, its images and the pages it links to load as well.
    [self.webView
        loadFileURL:self.url
        allowingReadAccessToURL:[self.url URLByDeletingLastPathComponent]];
}

- (void)done
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)webView:(WKWebView*)webView
    didFinishNavigation:(WKNavigation*)navigation
{
    self.title = webView.title;
}

- (void)webView:(WKWebView*)webView
    decidePolicyForNavigationAction:(WKNavigationAction*)action
    decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    NSURL* url = action.request.URL;
    if (url && !url.isFileURL &&
        WKNavigationTypeLinkActivated == action.navigationType)
    {
        [UIApplication.sharedApplication openURL:url options:@{} completionHandler:nil];
        decisionHandler(WKNavigationActionPolicyCancel);
        return;
    }
    decisionHandler(WKNavigationActionPolicyAllow);
}

// A link that asks for a new window: one of the application's own pages
// opens here, and anything else in the browser.
- (WKWebView*)webView:(WKWebView*)webView
    createWebViewWithConfiguration:(WKWebViewConfiguration*)configuration
    forNavigationAction:(WKNavigationAction*)action
    windowFeatures:(WKWindowFeatures*)features
{
    NSURL* url = action.request.URL;
    if (url.isFileURL)
    {
        [webView loadRequest:action.request];
    }
    else if (url)
    {
        [UIApplication.sharedApplication openURL:url options:@{} completionHandler:nil];
    }
    return nil;
}

@end

namespace ftk
{
    void openURLIOS(const std::string& value)
    {
        NSURL* url = [NSURL URLWithString:[NSString stringWithUTF8String:value.c_str()]];
        if (!url)
        {
            return;
        }
        if (url.isFileURL)
        {
            FTKPageViewController* page = [[FTKPageViewController alloc] init];
            page.url = url;
            UINavigationController* navigation =
                [[UINavigationController alloc] initWithRootViewController:page];
            navigation.modalPresentationStyle = UIModalPresentationPageSheet;
            [topViewController() presentViewController:navigation animated:YES completion:nil];
        }
        else
        {
            [UIApplication.sharedApplication openURL:url options:@{} completionHandler:nil];
        }
    }
}
