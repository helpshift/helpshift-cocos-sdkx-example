//
//  Cocos3xNotificationService.m
//  Cocos3xNSExtension
//
//  Created by Arif Ashraf on 17/03/26.
//

#import "Cocos3xNotificationService.h"
#import <HelpshiftX/HelpshiftX.h>

@interface Cocos3xNotificationService ()

@property (nonatomic, strong) void (^contentHandler)(UNNotificationContent *contentToDeliver);
@property (nonatomic, strong) UNMutableNotificationContent *bestAttemptContent;

@end

@implementation Cocos3xNotificationService

- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler {
    self.contentHandler = contentHandler;
    self.bestAttemptContent = [request.content mutableCopy];

    NSString *origin = request.content.userInfo[@"origin"];
    if ([origin isKindOfClass:[NSString class]] && [origin isEqualToString:@"helpshift"]) {
        [Helpshift handleBackgroundNotification:request
                             withContentHandler:contentHandler];
    } else {
        contentHandler(self.bestAttemptContent);
    }
}

- (void)serviceExtensionTimeWillExpire {
    // Called just before the extension will be terminated by the system.
    // Use this as an opportunity to deliver your "best attempt" at modified content, otherwise the original push payload will be used.
    self.contentHandler(self.bestAttemptContent);
}

@end
