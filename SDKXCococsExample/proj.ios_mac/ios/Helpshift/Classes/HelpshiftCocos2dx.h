/**
 * Copyright (c) 2015-2019 by Helpshift, Inc. All Rights Reserved.
 *
 */

#ifndef HelpshiftCocos2dx_h
#define HelpshiftCocos2dx_h

#include "cocos2d.h"

/** DO NOT CHANGE OR EDIT THIS VERSION!! **/ #define HS_COCOS_SDKX_VERSION "10.4.0"
#define HS_SDK_TYPE "sdkx-cocos2dx"

enum class HSAuthenticationFailureReason{
    HSAuthTokenNotProvided,
    HSInvalidAuthToken,
    Unkown
};

class HelpshiftCocos2dx
{
public:

    /*!\brief Shows faqs screen. This will show list of sections with search.
     * \param config: ValueMap
     */
    static void showFAQs(cocos2d::ValueMap& config);

    /*!\brief You can use this api call to provide a way for the user to send feedback or start a new conversation.
     * \param config: ValueMap
     */
    static void showConversation(cocos2d::ValueMap& config);

    /*!\brief Shows FAQ section screen.
     * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section.
     * \param: string publish id of faq section
     */
    static void showFAQSection(const char *sectionId, cocos2d::ValueMap& config);

    /*!\brief Shows question screen.This provides show question view provided a publish id of that question.
     * \param publishId id specifying a question
     */
    static void showSingleFAQ(const char *publishId, cocos2d::ValueMap& config);

    /** \brief Set user language
     * @param locale: char // language code
     */
    static void setLanguage(const char *locale);

    /**
     * \brief If you want a count of all new messages received in an existing conversation
     * @param shouldFetchFromServer: bool
     */
    static void requestUnreadMessageCount(bool shouldFetchFromServer);

    // User management APIs
    /**
     * \brief Login users with user name, user id, user email and user auth token
     * @param userData : map of userId, userName, userEmail and userAuthToken
     * @return boolean value for sucess or failure.
     */
    static bool login(cocos2d::ValueMap& userData);

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
     *      * `reason` indicates the failure reason.
     *      * `errors` is a optional Map containing extra details about the failure reason. Can be nil.
     */
    static void loginWithIdentity(const char *identityJWT,
                                  cocos2d::ValueMap& loginConfig,
                                  void (*helpshiftLoginSuccessCallback)(void),
                                  void (*helpshiftLoginFailureCallback)(const char *reason, const cocos2d::ValueMap& errors));

    /**
     * \brief Clear anonymous user in login
     * @param shouldClearAnonymousUser: bool
     */
    static void clearAnonymousUserOnLogin(bool shouldClearAnonymousUser);

    /**
     * \brief Logout current user
     */
    static void logout();

    /**
     * \brief Adds identities for the current logged in user. This method accepts JSON Web Token (JWT) string for
     * user identities. For details on how to create the JWT to be passed to this method, please refer
     * https://developers.helpshift.com/sdkx-cocos2dx/identity-user-hub/. Only applicable if the user has been logged in
     * using `loginWithIdentity` method.
     * @param identitiesJWT `NSString` JSON Web Token representing user identities to be logged in.
     */
    static void addUserIdentities(const char *identitiesJWT);

    /**
     * \brief Updates the master attributes for logged in user with the given dictionary of key-value pairs. Only
     * applicable if the user has been logged in using the `loginWithIdentity` method.
     *
     * @param attributes Dictionary containing the master attributes.
     */
    static void updateMasterAttributes(cocos2d::ValueMap& attributes);

    /**
     * \brief Updates the app attributes for logged in user with the given dictionary of key-value pairs. Only
     * applicable if the user has been logged in using the `loginWithIdentity` method.
     *
     * @param attributes Dictionary containing the app attributes.
     */
    static void updateAppAttributes(cocos2d::ValueMap& attributes);

    // Outbound support APIs

    /**
     * \brief proactively engage with consumers to solve problems within the app
     * @param proactiveLink: char //
     */
    static void handleProactiveLink(const char *proactiveLink);

    // Tracking APIs
    /**
     * \brief Breadcrumbs can be used to track events or end-user actions.
     * @param breadCrumb: char
     */
    static void leaveBreadCrumb(const char *breadCrumb);

    /**
     * \brief Breadcrumbs can be used to clear the breadcrumbs
     */
    static void clearBreadCrumbs();

    static void addUserTrail(const char *userTrail);

    // Delegate APIs
    /**
     * \brief register helpshift delegate
     */
    static void setHelpshiftEventsListener(void (*helpshiftEventHandler)(const char *eventName, cocos2d::ValueMap& data),
                                           void (*helpshiftAuthenticationHandler)(HSAuthenticationFailureReason reason));
    /*! \brief registeres proactive collector delegate
     */
    static void setHelpshiftProactiveConfigCollector(cocos2d::ValueMap (*getAPIConfig)());

    /*! returns sdkx version
     */
    static const char* sdkVersion();

    /*! closes helpshfit session
     */
    static void closeSession();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Push Notifications
    static void handlePushNotification(cocos2d::ValueMap& notificationData);
    static void registerPushToken(const char *deviceToken);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    /*!\brief Initialize helpshift support
     * \param domain This is your domain name without any http:// or forward slashes
     * \param appID This is the unique ID assigned to your app
     * \param config This is the dictionary which contains additional configuration options for the HelpshiftSDK.
     */
    static void install(const char *appID,const char *domain, cocos2d::ValueMap& config);

    /**
     * \brief pause push notifications
     * @param shouldPauseInAppNotification: bool
     */
    static void pauseDisplayOfInAppNotification(bool shouldPauseInAppNotification);

    // Push Notifications
    /**
     * \brief handle notification
     * @param notificationData: ValueMap
     */
    static bool handlePushNotification(cocos2d::ValueMap& notificationData,bool isAppLaunch);
#endif
};

#endif
