/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */

#include "HelpshiftCocos2dx.h"
#include "HelpshiftUtil.h"

#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <string>

using namespace std;

static void (*handleLoginFailure) (const char  *reason, const cocos2d::ValueMap &map) = nullptr;
static void (*handleLoginSuccess) () = nullptr;
static void (*handleHelpshiftEvent) (const char *eventName, cocos2d::ValueMap &eventData) = nullptr;
static void (*handleAuthFailureEvent) (HSAuthenticationFailureReason reason) = nullptr;
static cocos2d::ValueMap (*getAPIConfigFromCocos) () = nullptr;

/**
 * Opens the conversation screen
 * @param config : configuration map to configure conversation screen.
 */
void HelpshiftCocos2dx::showConversation(cocos2d::ValueMap& config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "showConversation",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}

/**
 * Opens the FAQs screen with FAQ sections and FAQs search
 * @param config : configuration map to configure faq screen.
 */
void HelpshiftCocos2dx::showFAQs(cocos2d::ValueMap& config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "showFAQs",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}

/**
 * Opens the FAQ Section screen
 * @param config : configuration map to configure faq screen.
 * @param sectionPublishId : Publish Id of the section
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId, cocos2d::ValueMap& config) {
    if (sectionPublishId == NULL || strlen(sectionPublishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "showFAQSection",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring sectionPubId = minfo.env->NewStringUTF(sectionPublishId);
        jobject hashMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        sectionPubId,
                                        hashMap);
    }
}

/**
 * Opens the Single FAQ screen
 * @param config : configuration map to configure faq screen.
 * @param publishId : Publish Id of FAQ
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId, cocos2d::ValueMap& config) {
    if (publishId == NULL || strlen(publishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "showSingleFAQ",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring pubId = minfo.env->NewStringUTF(publishId);
        jobject hashMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        pubId,
                                        hashMap);
    }
}

/**
 * Set SDK Language
 * @param locale : language code that you want to set as SDK Language
 */
void HelpshiftCocos2dx::setLanguage(const char *locale) {
    if (locale == NULL || strlen(locale) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "setLanguage",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(locale);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}

/**
 * Forward notification from Helpshift server to helpshift sdk
 * @param notificationData : The notification object received.
 */

void HelpshiftCocos2dx::handlePushNotification(cocos2d::ValueMap& notificationData) {
    if (notificationData.empty()) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "handlePushNotification",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject notificationMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env,
                                                                        notificationData);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        notificationMap);
    }
}

/**
 * Register device token with Helpshift server
 * @param deviceToken : Firebase device token
 */
void HelpshiftCocos2dx::registerPushToken(const char *deviceToken) {
    if (deviceToken == NULL || strlen(deviceToken) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "registerPushToken",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring devTokenStr = minfo.env->NewStringUTF(deviceToken);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        devTokenStr);
    }
}

/**
 * Fetch unread messages count from the server, get count via delegate.
 * @param shouldFetchFromServer : if (true) - fetch count from server,
 *                                if (false) return locally stored count.
 */
void HelpshiftCocos2dx::requestUnreadMessageCount(bool shouldFetchFromServer) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "requestUnreadMessageCount",
                                                             "(Z)V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        shouldFetchFromServer);
    }
}


/**
 * \brief Logs in a given user. This method accepts JSON Web Token (JWT) string for user identities, and an
 * additional `ValueMap` for login config. The result is communicated via success and failure callbacks.
 * For details on how to create the JWT to be passed to this method, please refer
 * https://developers.helpshift.com/sdkx-cocos2dx/identity-user-hub/
 * @param identityJWT `NSString` JSON Web Token representing user identity to be logged in.
 *
 * @param loginConfig `NSDictionary` representing additional config to be passed.
 *
 * @param successCallback callback block that is executed when login succeeds.
 *
 * @param failureCallback callback block that is executed when login fails.
 *       `reason` indicates the failure reason.
 *       `errors` is a optional Map containing extra details about the failure reason. Can be nil.
 */
void HelpshiftCocos2dx::loginWithIdentity(const char *identityJWT,
                                                 cocos2d::ValueMap &loginConfig,
                                                 void (*helpshiftLoginSuccessCallback)(void),
                                                 void (*helpshiftLoginFailureCallback)(
                                                         const char *reason,
                                                         const cocos2d::ValueMap &errors)) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "loginWithIdentity",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");

    if (hasMethod) {
        jstring identitiesJwtStr = minfo.env->NewStringUTF(identityJWT);
        jobject loginConfigMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, loginConfig);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, identitiesJwtStr,
                                        loginConfigMap);
    }

    if (helpshiftLoginFailureCallback != nullptr) {
        handleLoginFailure = helpshiftLoginFailureCallback;
    }

    if (helpshiftLoginSuccessCallback != nullptr) {
        handleLoginSuccess = helpshiftLoginSuccessCallback;
    }
}

/**
* Login users with user name, user id, user email and user auth token
* @param userData : map of userId, userName, userEmail and userAuthToken
*/
bool HelpshiftCocos2dx::login(cocos2d::ValueMap& userData) {
    if (userData.empty()) {
        return false;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "login",
                                                             "(Ljava/util/HashMap;)Z");
    if (hasMethod) {
        jobject userMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, userData);
        return minfo.env->CallStaticBooleanMethod(minfo.classID,
                                        minfo.methodID,
                                        userMap);
    }
    return false;
}

/**
 * Logout current user
 */
 void HelpshiftCocos2dx::logout() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "logout",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
 }

/**
* \brief Adds identities for the current logged in user. This method accepts JSON Web Token (JWT) string for
* user identities. For details on how to create the JWT to be passed to this method, please refer
* https://developers.helpshift.com/sdkx-cocos2dx/identity-user-hub/. Only applicable if the user has been logged in
* using `loginWithIdentity` method.
* @param identitiesJWT `NSString` JSON Web Token representing user identities to be logged in.
*/
void HelpshiftCocos2dx::addUserIdentities(const char *identitiesJWT) {
    if (identitiesJWT == NULL || strlen(identitiesJWT) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "addUserIdentities",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring identitiesJwtStr = minfo.env->NewStringUTF(identitiesJWT);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        identitiesJwtStr);
    }
}

/**
 * \brief Updates the master attributes for logged in user with the given dictionary of key-value pairs. Only
 * applicable if the user has been logged in using the `loginWithIdentity` method.
 *
 * @param attributes Dictionary containing the master attributes.
 */
void HelpshiftCocos2dx::updateMasterAttributes(cocos2d::ValueMap &attributes) {

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "updateMasterAttributes",
                                                             "(Ljava/util/HashMap;)V");


    if (hasMethod) {
        jobject userMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, attributes);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        userMap);
    }
}

/**
 * \brief Updates the app attributes for logged in user with the given dictionary of key-value pairs. Only
 * applicable if the user has been logged in using the `loginWithIdentity` method.
 *
 * @param attributes Dictionary containing the app attributes.
 */
void HelpshiftCocos2dx::updateAppAttributes(cocos2d::ValueMap &attributes) {

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "updateAppAttributes",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject userMap = HelpshiftUtil::parseValueMapToHashMap(minfo.env, attributes);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        userMap);
    }
}

 /**
  * Clear anonymous user in login
  * @param shouldClearAnonymousUser : clear anonymous user details on login
  */
 void HelpshiftCocos2dx::clearAnonymousUserOnLogin(bool shouldClearAnonymousUser) {
     cocos2d::JniMethodInfo minfo;
     bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                              "com/helpshift/HelpshiftCocosBridge",
                                                              "clearAnonymousUserOnLogin",
                                                              "(Z)V");
     if (hasMethod) {
         minfo.env->CallStaticVoidMethod(minfo.classID,
                                         minfo.methodID,
                                         shouldClearAnonymousUser);
     }
 }

/**
 * Outbound Support API
 * @param proactiveLink : URL encoded data config that you can pass to Helpshift
 */
void HelpshiftCocos2dx::handleProactiveLink(const char *proactiveLink) {
    if (proactiveLink == NULL || strlen(proactiveLink) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "handleProactiveLink",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring proactiveLinkStr = minfo.env->NewStringUTF(proactiveLink);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        proactiveLinkStr);
    }
}

/**
 * Add breadcrumbs to track events or end-user actions
 * @param breadCrumb : log message to track events or actions
 */

void HelpshiftCocos2dx::leaveBreadCrumb(const char *breadCrumb) {
    if (breadCrumb == NULL || strlen(breadCrumb) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "leaveBreadCrumb",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring breadCrumbStr = minfo.env->NewStringUTF(breadCrumb);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        breadCrumbStr);
    }
}

/**
 * Clear Breadcrumbs
 */

void HelpshiftCocos2dx::clearBreadCrumbs() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "clearBreadCrumbs",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
}

/**
 * Track user page visits before accessing Helpshift
 * @param userTrail : add user trail to track user
 */
void HelpshiftCocos2dx::addUserTrail(const char *userTrail) {
    if (userTrail == NULL || strlen(userTrail) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "addUserTrail",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring userTrailStr = minfo.env->NewStringUTF(userTrail);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        userTrailStr);
    }
}

const char* HelpshiftCocos2dx::sdkVersion() {
    return HS_COCOS_SDKX_VERSION;
}

void HelpshiftCocos2dx::closeSession() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "closeSession",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
}

void verifyAndSetEventsListener() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "setHelpshiftEventsListener",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
/**
 * Set Helpshift Event Listener to listen to Helpshift Events
 * @param helpshiftEventHandler : Event handle to get event name and event data
 * @param helpshiftAuthenticationHandler : Auth failure handler to get auth failure reason
 */
void HelpshiftCocos2dx::setHelpshiftEventsListener(
        void (*helpshiftEventHandler)(const char *eventName, cocos2d::ValueMap &eventData),
        void (*helpshiftAuthenticationHandler)(HSAuthenticationFailureReason reason)) {

    verifyAndSetEventsListener();

    if (helpshiftEventHandler != nullptr) {
        handleHelpshiftEvent = helpshiftEventHandler;
    }

    if (helpshiftAuthenticationHandler != nullptr) {
        handleAuthFailureEvent = helpshiftAuthenticationHandler;
    }
}

void verifyAndSetProactiveConfigCollector() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftCocosBridge",
                                                             "setHelpshiftProactiveConfigCollector",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
/**
 * Set Helpshift proactive config collector callback to provide local config
 * @param getAPIConfig : Config collector to provide local config to outbound support.
 */
void HelpshiftCocos2dx::setHelpshiftProactiveConfigCollector(cocos2d::ValueMap (*getAPIConfig)()) {
    verifyAndSetProactiveConfigCollector();
    getAPIConfigFromCocos = getAPIConfig;
}

/**
 * This code defines C++ functions that can be called from Java using the JNI.
 */
extern "C" {

    // The function returns the SDK X Cocos plugin version as a Java string.
    JNIEXPORT jstring JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_getCocosPluginVersion(JNIEnv *env,
                                                            jclass object) {
        return env->NewStringUTF(HS_COCOS_SDKX_VERSION);
    }

    // The function returns the SDK type as a Java string.
    JNIEXPORT jstring JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_getSDKType(JNIEnv *env,
                                                              jclass object) {
        return env->NewStringUTF(HS_SDK_TYPE);
    }

    // The function returns the current version of the Cocos2d-x game engine as a Java string.
    JNIEXPORT jstring JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_getRuntimeVersion(JNIEnv *env,
                                                              jclass object) {
        return env->NewStringUTF(cocos2d::cocos2dVersion());
    }

    // Helpshift Event Handler callback to delegate Helpshift SDK events to cocos
    JNIEXPORT void JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_onEventOccurred(JNIEnv *env,
                                                            jclass clazz,
                                                            jstring event_name,
                                                            jobject event_data) {
        if (handleHelpshiftEvent) {
            const char *eventName = env->GetStringUTFChars(event_name, nullptr);
            cocos2d::ValueMap eventData = HelpshiftUtil::convertJavaMapToValueMap(env, event_data);
            handleHelpshiftEvent(eventName, eventData);
        }
    }

    // Helpshift Event Handler callback to delegate Authentication Failure event to cocos
    JNIEXPORT void JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_onUserAuthenticationFailure(JNIEnv *env,
                                                                        jclass clazz,
                                                                        jstring authFailureReason) {
        if (handleAuthFailureEvent) {
            const char *reasonChars = env->GetStringUTFChars(authFailureReason, nullptr);
            std::string reasonString(reasonChars);
            env->ReleaseStringUTFChars(authFailureReason, reasonChars);

            if (reasonString == "REASON_AUTH_TOKEN_NOT_PROVIDED") {
                handleAuthFailureEvent(HSAuthenticationFailureReason::HSAuthTokenNotProvided);
            } else if (reasonString == "REASON_INVALID_AUTH_TOKEN") {
                handleAuthFailureEvent(HSAuthenticationFailureReason::HSInvalidAuthToken);
            } else {
                handleAuthFailureEvent(HSAuthenticationFailureReason::Unkown);
            }
        }
    }

    // Helpshift local config collector callback to receive local config from cocos layer
    JNIEXPORT jobject JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_getAPIConfigFromCocos(JNIEnv *env, jclass clazz) {
        jobject hashMap = HelpshiftUtil::parseValueMapToHashMap(env, getAPIConfigFromCocos());
        return hashMap;
    }

    JNIEXPORT void JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_onLoginFailure(JNIEnv *env,
                                                           jclass clazz,
                                                           jstring error,
                                                           jobject map) {
        if (handleLoginFailure) {
            const char *reason = env->GetStringUTFChars(error, nullptr);
            cocos2d::ValueMap errorMap;

            if (map == nullptr) {
                handleLoginFailure(reason, errorMap);
            } else {
                errorMap = HelpshiftUtil::convertJavaMapToValueMap(env, map);
                handleLoginFailure(reason, errorMap);
            }
        }
    }

    JNIEXPORT void JNICALL
    Java_com_helpshift_HelpshiftCocosBridge_onLoginSuccess(JNIEnv *env, jclass clazz) {
        if (handleLoginSuccess){
            handleLoginSuccess();
        }
    }
}

