//
//  WKWebView+Inspectable.h
//  Copyright © 2023 Helpshift. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <WebKit/WebKit.h>

NS_ASSUME_NONNULL_BEGIN

#ifndef HsWebViewSetInspectable_h
#define HsWebViewSetInspectable_h

/// Call this method to enable or disable inspection on WKWebView. Uses private APIs below iOS 16.4.
/// So, we need to ensure this method or it's implementation does not end up in release builds.
void HsWebViewSetInspectable(WKPreferences* prefs, bool inspectable);

#endif /* HsWebViewSetInspectable_h */

@interface WKWebView (Inspectable)

@property (class) BOOL inspectable;

@end

NS_ASSUME_NONNULL_END
