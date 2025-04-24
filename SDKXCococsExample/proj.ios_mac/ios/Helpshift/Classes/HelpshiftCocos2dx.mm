//
//  HelpshiftCocos2dx.mm
//  Helpshift Cocos2dx iOS SDK
//

#import "HelpshiftCocos2dx.h"
#import "HelpshiftUtil.h"
#import "HelpshiftCocos2dxDelegate.h"
#import "HelpshiftProactiveConfigCollector.h"

@import HelpshiftX;

void HelpshiftCocos2dx::install(const char *appID,const char *domain, cocos2d::ValueMap& config) {
    config["sdkType"] = HS_SDK_TYPE;
    config["pluginVersion"] = HS_COCOS_SDKX_VERSION;
    config["runtimeVersion"] = cocos2d::cocos2dVersion();
    [Helpshift installWithPlatformId:hsParseCString(appID)
                              domain:hsParseCString(domain)
                              config:hsValueMapToNSDictionary(config)];
}

void HelpshiftCocos2dx::showConversation(cocos2d::ValueMap& config) {
    [Helpshift showConversationWithConfig:hsValueMapToNSDictionary(config)];
}

void HelpshiftCocos2dx::showFAQs(cocos2d::ValueMap& config) {
    [Helpshift showFAQsWithConfig:hsValueMapToNSDictionary(config)];
}

void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId, cocos2d::ValueMap& config) {
    [Helpshift showFAQSection:hsParseCString(sectionPublishId) withConfig:hsValueMapToNSDictionary(config)];
}

void HelpshiftCocos2dx::showSingleFAQ(const char *publishId, cocos2d::ValueMap& config) {
    [Helpshift showSingleFAQ:hsParseCString(publishId) withConfig:hsValueMapToNSDictionary(config)];
}

bool HelpshiftCocos2dx::login(cocos2d::ValueMap& userData) {
   return [Helpshift loginUser:hsValueMapToNSDictionary(userData)];
}

void HelpshiftCocos2dx::loginWithIdentity(const char *identityJWT,
                                          cocos2d::ValueMap& loginConfig,
                                          void (*helpshiftLoginSuccessCallback)(void),
                                          void (*helpshiftLoginFailureCallback)(const char *reason, const cocos2d::ValueMap& errors)) {
    [Helpshift loginWithIdentity:hsParseCString(identityJWT) config:hsValueMapToNSDictionary(loginConfig) success:^{
        helpshiftLoginSuccessCallback();
    } failure:^(NSString *reason, NSDictionary<NSString *,NSString *> *errors) {
        helpshiftLoginFailureCallback(reason.UTF8String, hsNSDictionaryToValueMap(errors));
    }];
}

void HelpshiftCocos2dx::logout() {
    [Helpshift logout];
}

void HelpshiftCocos2dx::clearAnonymousUserOnLogin(bool shouldClearAnonymousUser) {
    [Helpshift clearAnonymousUserOnLogin:shouldClearAnonymousUser];
}

void HelpshiftCocos2dx::addUserIdentities(const char *identitiesJWT) {
    [Helpshift addUserIdentities:hsParseCString(identitiesJWT)];
}

void HelpshiftCocos2dx::updateMasterAttributes(cocos2d::ValueMap &attributes) {
    [Helpshift updateMasterAttributes:hsValueMapToNSDictionary(attributes)];
}

void HelpshiftCocos2dx::updateAppAttributes(cocos2d::ValueMap &attributes) {
    [Helpshift updateAppAttributes:hsValueMapToNSDictionary(attributes)];
}

void HelpshiftCocos2dx::leaveBreadCrumb(const char *breadCrumb) {
    [Helpshift leaveBreadcrumb:hsParseCString(breadCrumb)];
}

void HelpshiftCocos2dx::clearBreadCrumbs() {
    [Helpshift clearBreadcrumbs];
}

void HelpshiftCocos2dx::addUserTrail(const char *userTrail) {
    [Helpshift addUserTrail:hsParseCString(userTrail)];
}

void HelpshiftCocos2dx::setLanguage(const char *locale) {
    [Helpshift setLanguage:hsParseCString(locale)];
}

void HelpshiftCocos2dx::handleProactiveLink(const char *proactiveLink) {
    [Helpshift handleProactiveLink:hsParseCString(proactiveLink)];
}

bool HelpshiftCocos2dx::handlePushNotification(cocos2d::ValueMap& notificationData,bool isAppLaunch) {
    return [Helpshift handleNotificationWithUserInfoDictionary:hsValueMapToNSDictionary(notificationData) isAppLaunch:isAppLaunch];
}

void HelpshiftCocos2dx::pauseDisplayOfInAppNotification(bool shouldPauseInAppNotification) {
    [Helpshift pauseDisplayOfInAppNotification:shouldPauseInAppNotification];
}

void HelpshiftCocos2dx::requestUnreadMessageCount(bool shouldFetchFromServer) {
    [Helpshift requestUnreadMessageCount:shouldFetchFromServer];
}

const char* HelpshiftCocos2dx::sdkVersion() {
    return HS_COCOS_SDKX_VERSION;
}

void HelpshiftCocos2dx::setHelpshiftEventsListener(void (*helpshiftEventHandler) (const char *eventName, cocos2d::ValueMap& config), void (*helpshiftAuthenticationHandler) (HSAuthenticationFailureReason reason)) {
    [HelpshiftCocos2dxDelegate sharedInstance].helpshiftEventHandler = helpshiftEventHandler;
    [HelpshiftCocos2dxDelegate sharedInstance].authenticationFailedForUserHandler = helpshiftAuthenticationHandler;
    Helpshift.sharedInstance.delegate = HelpshiftCocos2dxDelegate.sharedInstance;
}

void HelpshiftCocos2dx::setHelpshiftProactiveConfigCollector(cocos2d::ValueMap (*getAPIConfig)()) {
    [HelpshiftProactiveConfigCollector sharedInstance].getAPIConfigCollector = getAPIConfig;
    Helpshift.sharedInstance.proactiveAPIConfigCollectorDelegate = HelpshiftProactiveConfigCollector.sharedInstance;
}

void HelpshiftCocos2dx::closeSession() {
    [Helpshift closeSession];
}
