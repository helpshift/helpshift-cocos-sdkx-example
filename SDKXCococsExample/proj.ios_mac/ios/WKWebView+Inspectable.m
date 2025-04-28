//
//  WKWebView+Inspectable.m
//  Copyright © 2023 Helpshift. All rights reserved.
//

#import "WKWebView+Inspectable.h"
#import <objc/runtime.h>

// The method signature must match the corresponding C API method signature in Webkit. Original Webkit method -
// https://github.com/WebKit/WebKit/blob/5b7748a1c90e9320a8ba011bce9177a600162e88/Source/WebKit/UIProcess/API/C/WKPreferences.cpp#L689
// The first id param is actually a type called WKPreferencesRef
// Since we don't have access to that internal type, we use id instead
void WKPreferencesSetWebSecurityEnabled(id, bool);

// Since we don't have access to WKPreferencesRef type which is needed by above method, we instead pass in a fake object
// whose properties match WKPreferencesRef. This was found out by passing a random object to
// WKPreferencesSetWebSecurityEnabled(id, bool) and seeing what error it produces. It gives an
// "unrecognized selector _apiObject" error. So we create a property with that name in our fake object and the error is
// gone!
@interface HsFakeWebKitPointer: NSObject
@property (nonatomic) void *_apiObject;
@end

@implementation HsFakeWebKitPointer
@end

void HsWebViewSetInspectable(WKPreferences* prefs, bool inspectable) {
    // Get ref to private preferences ivar in WKPreferences class.
    Ivar ivar = class_getInstanceVariable([WKPreferences class], "_preferences");
    // Get ref to actual instance of preferences in passed prefs object.
    void* realPreferences = (void*)(((uintptr_t)prefs) + ivar_getOffset(ivar));
    // Hack to get rid of "unrecognized selector _apiObject" error.
    HsFakeWebKitPointer* fake = [HsFakeWebKitPointer new];
    fake._apiObject = realPreferences;
    // Call the internal WebKit API to actually set web security enabled/disabled
    WKPreferencesSetWebSecurityEnabled(fake, !inspectable);
}

@implementation WKWebView (Inspectable)

static BOOL _inspectable = false;

- (instancetype) initSwizzledWithFrame:(CGRect)frame configuration:(WKWebViewConfiguration *)configuration {
    self = [self initSwizzledWithFrame:frame configuration:configuration];
    if(self) {
        // This works for iOS 16.3 and below
        HsWebViewSetInspectable(self.configuration.preferences, _inspectable);
        // This works for iOS 16.4 and above
        [self hs_setInspectable:_inspectable];
    }
    return self;
}

+ (void) swizzleInits {
    Method originalInit = class_getInstanceMethod(self, @selector(initWithFrame:configuration:));
    Method swizzledInit = class_getInstanceMethod(self, @selector(initSwizzledWithFrame:configuration:));
    method_exchangeImplementations(originalInit, swizzledInit);
}

+ (void) load {
    [WKWebView swizzleInits];
}

+ (BOOL) inspectable {
    return _inspectable;
}

+ (void) setInspectable:(BOOL)inspectable {
    _inspectable = inspectable;
}

// iOS 16.4 onwards, WKWebView has a new property - isInspectable - which determines if webview instance is debuggable
// or not. Since we always want all SDK webviews to be debuggable based on demo app flag, we set this property for all
// webviews included in the demo app.
// We need to use objc runtime since the property was introduced in Xcode 14.3, while the demo app may be compiled with
// Xcode versions lower than that.
- (void) hs_setInspectable:(BOOL)inspectable {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
    SEL selector = NSSelectorFromString(@"setInspectable:");
    if([self respondsToSelector:selector]) {
        // NOTE - setInspectable: actually expects a BOOL, but there's no way to pass a primitive type via
        // performSelector. The way to pass a bool is -
        // Any NSObject will evaluate to true
        // nil will evaluate to false
        if(inspectable) {
            [self performSelector:selector withObject:@(inspectable)];
        } else {
            [self performSelector:selector withObject:nil];
        }
    }
#pragma clang diagnostic pop
}

@end
