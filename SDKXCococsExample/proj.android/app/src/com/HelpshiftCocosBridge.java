/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 */
package com.helpshift;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.helpshift.log.HSLogger;
import com.helpshift.sdkxcocosproxy.HSCocosInstallHelper;
import com.helpshift.sdkxcocosproxy.utils.InstallUtils;
import com.helpshift.util.Utils;
import com.helpshift.util.JsonUtils;
import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.HashMap;
import java.util.Map;

/**
 * This class represents a proxy class for all the SDK X APIs
 * All the methods of this class will be invoked by cocos layer
 */
public class HelpshiftCocosBridge {

    public static String TAG = "HelpshiftCocos2dx";
    static Activity activity;

    // Get cocos plugin version from Cocos through JNI
    public static native String getCocosPluginVersion();

    // Get SDK Type through JNI
    private static native String getSDKType();

    // Get runtimeVersion from cocos through JNI
    public static native String getRuntimeVersion();

    public static native void onEventOccurred(String eventName, Map<String, Object> eventData);

    public static native void onUserAuthenticationFailure(String authFailureReason);

    public static native Map<String, Object> getAPIConfigFromCocos();

    public static native void onLoginSuccess();

    public static native void onLoginFailure(String error, Map<String, String> map);

    /**
     * SDK X install call bridge
     */
    public static void install(Activity a, String platformId, String domain, HashMap<String, Object> config) {

        // Initialise HelpshiftCocosBridge activity to use it in other APIs
        HelpshiftCocosBridge.activity = a;

        config.put("sdkType", getSDKType());
        config.put("pluginVersion", getCocosPluginVersion());
        config.put("runtimeVersion", getRuntimeVersion());
        HSCocosInstallHelper.install(a.getApplication(), platformId, domain, JsonUtils.mapToJsonString(config));
    }

    /**
     * Helpshift Event Listener
     */
    public static void setHelpshiftEventsListener() {
        Helpshift.setHelpshiftEventsListener(new HelpshiftCocos2dxDelegates());
    }

    /**
     * Helpshift proactive outbound config collector
     */
    public static void setHelpshiftProactiveConfigCollector() {
        Helpshift.setHelpshiftProactiveConfigCollector(new HSCocos2dxProactiveListener());
    }

    /**
     * Convert date type CIFs from String to Long value
     * @param configMap : config from which date type CIFs is to be read
     * @return
     */
    private static Map<String, Object> checkAndModifyDateCIF(Map<String, Object> configMap) {
        try {
            for (Map.Entry<String, Object> entry : configMap.entrySet()) {
                Object value = entry.getValue();
                if (value instanceof Map) {
                    // Recursively process nested maps
                    checkAndModifyDateCIF((Map<String, Object>) value);
                } else if (entry.getKey().equals("type") && value instanceof String &&
                        (value.equals("date") || value.equals("dt"))) {
                    // Convert the value to a long if the type is "date"
                    String valueString = (String) configMap.get("value");
                    long longValue = 0;
                    if (valueString != null) {
                        longValue = Long.parseLong(valueString);
                    }
                    configMap.put("value", longValue);
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "Failed to convert string type date to Long", e);
        }
        return configMap;
    }

    private static boolean checkAndVerifyActivityInstance() {
        if (activity != null) {
            return true;
        }
        activity = (Activity) Cocos2dxActivity.getContext();
        return activity != null;
    }

    /**
     * SDK X showConversation bridge
     */
    public static void showConversation(HashMap<String, Object> configMap) {
        if (checkAndVerifyActivityInstance()) {
            Helpshift.showConversation(activity, checkAndModifyDateCIF(configMap));
        } else {
            HSLogger.e(TAG, "Unable to find Activity to start session");
        }
    }

    /**
     * SDK X showFAQs bridge
     */
    public static void showFAQs(HashMap<String, Object> configMap) {
        if (checkAndVerifyActivityInstance()) {
            Helpshift.showFAQs(activity, checkAndModifyDateCIF(configMap));
        } else {
            HSLogger.e(TAG, "Unable to find Activity to start session");
        }
    }

    /**
     * SDK X showFAQSection bridge
     */
    public static void showFAQSection(String sectionId, HashMap<String, Object> configMap) {
        if (checkAndVerifyActivityInstance()) {
            Helpshift.showFAQSection(activity, sectionId, checkAndModifyDateCIF(configMap));
        } else {
            HSLogger.e(TAG, "Unable to find Activity to start session");
        }
    }

    /**
     * SDK X showSingleFAQ bridge
     */
    public static void showSingleFAQ(String faqId, HashMap<String, Object> configMap) {
        if (checkAndVerifyActivityInstance()) {
            Helpshift.showSingleFAQ(activity, faqId, checkAndModifyDateCIF(configMap));
        } else {
            HSLogger.e(TAG, "Unable to find Activity to start session");
        }
    }

    /**
     * SDK X setLanguage bridge
     */
    public static void setLanguage(String locale) {
        Helpshift.setLanguage(locale);
    }

    /**
     * SDK X handlePushNotification bridge
     */
    public static void handlePushNotification(Context context, Map<String, String> notificationData) {
        InstallUtils.installWithCachedValues(context);
        Helpshift.handlePush(notificationData);
    }

    /**
     * SDK X requestUnreadMessageCount bridge
     */
    public static void requestUnreadMessageCount(boolean shouldFetchFromServer) {
        Helpshift.requestUnreadMessageCount(shouldFetchFromServer);
    }

    /**
     * SDK X registerPushToken bridge
     */
    public static void registerPushToken(String token) {
        Helpshift.registerPushToken(token);
    }

    /**
     * SDK X loginWithIdentity bridge
     */
    public static void loginWithIdentity(String identityJwt, HashMap<String, Object> loginConfig){
        Helpshift.loginWithIdentity(identityJwt, loginConfig, new HelpshiftCocos2dxLoginEventListener());
    }

    /**
     * SDK X login bridge
     */
    public static boolean login(HashMap<String, String> userData) {
        return Helpshift.login(userData);
    }

    /**
     * SDK X logout bridge
     */
    public static void logout() {
        Helpshift.logout();
    }

    /**
     * SDK X addUserIdentities bridge
     */
    public static void addUserIdentities(String identitiesJWT) {
        Helpshift.addUserIdentities(identitiesJWT);
    }

    /**
     * SDK X updateMasterAttributes bridge
     */
    public static void updateMasterAttributes(HashMap<String, Object> attributes) {
        Helpshift.updateMasterAttributes(attributes);
    }

    /**
     * SDK X updateAppAttributes bridge
     */
    public static void updateAppAttributes(HashMap<String, Object> attributes) {
        Helpshift.updateAppAttributes(attributes);
    }

    /**
     * SDK X clearAnonymousUserOnLogin bridge
     */
    public static void clearAnonymousUserOnLogin(boolean clearAnonymousUser) {
        Helpshift.clearAnonymousUserOnLogin(clearAnonymousUser);
    }

    /**
     * SDK X handleProactiveLink bridge
     */
    public static void handleProactiveLink(Context context, String proactiveLink) {
        InstallUtils.installWithCachedValues(context);
        Helpshift.handleProactiveLink(proactiveLink);
    }

    /**
     * SDK X leaveBreadCrumb bridge
     */
    public static void leaveBreadCrumb(String breadCrumb) {
        Helpshift.leaveBreadCrumb(breadCrumb);
    }

    /**
     * SDK X clearBreadCrumbs bridge
     */
    public static void clearBreadCrumbs() {
        Helpshift.clearBreadCrumbs();
    }

    /**
     * SDK X addUserTrail bridge
     */
    public static void addUserTrail(String trail) {
        Helpshift.addUserTrail(trail);
    }

    /**
     * SDK X closeSession bridge
     */
    public static void closeSession() {
        Helpshift.closeSession();
    }
}