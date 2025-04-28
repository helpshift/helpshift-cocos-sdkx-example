// iOSHelper.mm

#import "iOSHelper.h"
#import <UserNotifications/UserNotifications.h>

void scheduleLocalNotification(const std::string& title, const std::string& body, int delaySeconds) {
    UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];

    // Request authorization to display notifications
    [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert + UNAuthorizationOptionSound)
                          completionHandler:^(BOOL granted, NSError * _Nullable error) {
        // Handle authorization result if needed
    }];

    // Create notification content
    UNMutableNotificationContent *content = [UNMutableNotificationContent new];
    content.title = [NSString stringWithUTF8String:title.c_str()];
    content.body = [NSString stringWithUTF8String:body.c_str()];
    content.sound = [UNNotificationSound defaultSound];

    // Create a trigger based on the delay
    UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:delaySeconds repeats:NO];

    // Create a notification request
    UNNotificationRequest *request = [UNNotificationRequest requestWithIdentifier:@"LocalNotification" content:content trigger:trigger];

    // Schedule the notification
    [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
        // Handle completion if needed
    }];
}

void purgeData() {
    // Delete all data from data container.
    NSMutableArray<NSURL *> *urls = [[NSMutableArray alloc] initWithCapacity:0];
    [urls addObjectsFromArray:[[NSFileManager defaultManager] URLsForDirectory:NSLibraryDirectory inDomains:NSUserDomainMask]];
    [urls addObjectsFromArray:[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask]];
    for(NSURL *url in urls)
    {
        for(NSURL *file in [[NSFileManager defaultManager] contentsOfDirectoryAtURL:url includingPropertiesForKeys:nil options:NSDirectoryEnumerationSkipsSubdirectoryDescendants error:nil])
        {
            BOOL success = [[NSFileManager defaultManager] removeItemAtURL:file error:nil];
            if(!success) {
                NSLog(@"Could not delete file at path:%@",[file absoluteString]);
            }
        }
    }

    // Delete all data from keychain.
    BOOL (^deleteAllKeysForSecClass)(CFTypeRef) = ^BOOL (CFTypeRef secClass) {
        id dict = @{ (__bridge id) kSecClass: (__bridge id) secClass };
        OSStatus result = SecItemDelete((__bridge CFDictionaryRef) dict);
        return result == noErr || result == errSecItemNotFound;
    };
    deleteAllKeysForSecClass(kSecClassInternetPassword);
    deleteAllKeysForSecClass(kSecClassGenericPassword);
    deleteAllKeysForSecClass(kSecClassCertificate);
    deleteAllKeysForSecClass(kSecClassKey);
    deleteAllKeysForSecClass(kSecClassIdentity);

    // 2 sec grace time to finish purging, then exit the app.
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        exit(0);
    });
}
