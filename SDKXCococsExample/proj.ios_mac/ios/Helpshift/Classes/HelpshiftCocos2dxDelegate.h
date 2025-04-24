//
//  HelpshiftCocos2dxDelegate.h
//  Helpshift Cocos2dx iOS SDK Delegates
//
#import <Foundation/Foundation.h>
#import "HelpshiftCocos2dx.h"
#include "cocos2d.h"

@import HelpshiftX;

NS_ASSUME_NONNULL_BEGIN

@interface HelpshiftCocos2dxDelegate : NSObject <HelpshiftDelegate>

+ (HelpshiftCocos2dxDelegate *) sharedInstance;

@property (nonatomic) void (*helpshiftEventHandler) (const char *eventName, cocos2d::ValueMap& data);
@property (nonatomic) void (*authenticationFailedForUserHandler) (HSAuthenticationFailureReason reason);

@end

NS_ASSUME_NONNULL_END
