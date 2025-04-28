//
//  HelpshiftCocos2dxDelegate.mm
//  Helpshift Cocos2dx iOS SDK Delegates
//

#import "HelpshiftCocos2dxDelegate.h"
#include "cocos2d.h"
#import "HelpshiftUtil.h"
@implementation HelpshiftCocos2dxDelegate

+ (HelpshiftCocos2dxDelegate *) sharedInstance {
    static HelpshiftCocos2dxDelegate *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[HelpshiftCocos2dxDelegate alloc] init];
    });
    return instance;
}

- (void) authenticationFailedForUserWithReason:(HelpshiftAuthenticationFailureReason)reason {
    if ([HelpshiftCocos2dxDelegate sharedInstance].authenticationFailedForUserHandler) {

        HSAuthenticationFailureReason externalReason = HSAuthenticationFailureReason::Unkown;
        switch (reason) {
            case HelpshiftAuthenticationFailureReasonAuthTokenNotProvided:
                externalReason = HSAuthenticationFailureReason::HSAuthTokenNotProvided;
                break;
            case HelpshiftAuthenticationFailureReasonInvalidAuthToken:
                externalReason = HSAuthenticationFailureReason::HSInvalidAuthToken;
                break;
            default:
                externalReason = HSAuthenticationFailureReason::Unkown;
                break;
        }
        [HelpshiftCocos2dxDelegate sharedInstance].authenticationFailedForUserHandler(externalReason);
    }
}

- (void) handleHelpshiftEvent:(nonnull NSString *)eventName withData:(nullable NSDictionary *)data {
    if ([HelpshiftCocos2dxDelegate sharedInstance].helpshiftEventHandler) {
        cocos2d::ValueMap valueMap = hsNSDictionaryToValueMap(data);
        [HelpshiftCocos2dxDelegate sharedInstance].helpshiftEventHandler([eventName  UTF8String],valueMap);
    }
}

@end
