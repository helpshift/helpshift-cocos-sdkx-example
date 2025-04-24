//
//  HelpshiftProactiveConfigCollector.h
//  Helpshift Cocos2dx iOS SDK
//

#import <Foundation/Foundation.h>
#include "cocos2d.h"
@import HelpshiftX;

NS_ASSUME_NONNULL_BEGIN

@interface HelpshiftProactiveConfigCollector : NSObject <HelpshiftProactiveAPIConfigCollectorDelegate>

+ (HelpshiftProactiveConfigCollector *) sharedInstance;

@property (nonatomic) cocos2d::ValueMap (*getAPIConfigCollector) ();

@end

NS_ASSUME_NONNULL_END
