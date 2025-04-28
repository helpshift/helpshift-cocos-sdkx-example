//
//  HelpshiftProactiveConfigCollector.mm
//  Helpshift Cocos2dx iOS SDK
//

#import "HelpshiftProactiveConfigCollector.h"
#import "HelpshiftUtil.h"

@implementation HelpshiftProactiveConfigCollector

+ (HelpshiftProactiveConfigCollector *) sharedInstance {
    static HelpshiftProactiveConfigCollector *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[HelpshiftProactiveConfigCollector alloc] init];
    });
    return instance;
}

- (nonnull NSDictionary *) getAPIConfig {
    cocos2d::ValueMap localMap = [HelpshiftProactiveConfigCollector  sharedInstance].getAPIConfigCollector();
    return  hsValueMapToNSDictionary(localMap);
}

@end
