package org.cocos2dx.cpp;

import static com.helpshift.R.drawable.hs__chat_icon;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.helpshift.HelpshiftCocosBridge;
import com.helpshift.log.HSLogger;

public class MyFirebaseMessagingService extends FirebaseMessagingService {

    private static final String TAG = "HelpshiftSDK_Push";

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        java.util.Map<String, String> data = remoteMessage.getData();

        // Handling push notification from Helpshift.
        // This handling is for notifications sent from Helpshift when agent replies to an issue from Helpshift dashboard.
        String origin = data.get("origin");
        HSLogger.d(TAG, data.toString());

        if (origin != null && origin.equals("helpshift")) {
            HelpshiftCocosBridge.handlePushNotification(getApplicationContext(), data);
        } else {
            // This is not a notification from Helpshift system
            // This is your existing push notification system

            // Sample key in notification data where you will put the proactive support link generated from Helpshift dashboard
            android.content.Intent intent = new android.content.Intent(getApplicationContext(), org.cocos2dx.cpp.NotificationActivity.class);
            intent.setFlags(android.content.Intent.FLAG_ACTIVITY_NEW_TASK);

            if (data.containsKey("helpshift_proactive_link")) {
                String proactiveUrl = data.get("helpshift_proactive_link");

                // Sample code to generate Push notification
                intent.putExtra("proactiveNotification", true);
                intent.putExtra("proactiveLink", proactiveUrl);

                createNotification(intent, data);
            }
            else if (data.containsKey("close_helpshift_session")) {
                intent.putExtra("closeHelpshiftSession", true);
                createNotification(intent, data);
            }

        }
    }
    private void createNotification(android.content.Intent intent, java.util.Map<String, String> data) {

        android.content.Context context = getApplicationContext();

        int pendingIntentFlag = android.os.Build.VERSION.SDK_INT < 23 ? 0 : android.app.PendingIntent.FLAG_IMMUTABLE;
        android.app.PendingIntent pendingIntent = android.app.PendingIntent.getActivity(
                context, new java.util.Random().nextInt(), intent, pendingIntentFlag);

        android.app.NotificationManager mNotificationManager = (android.app.NotificationManager) getSystemService(android.content.Context.NOTIFICATION_SERVICE);

        android.app.Notification.Builder builder = new android.app.Notification.Builder(context)
                .setContentTitle(data.get("title"))
                .setContentText(data.get("message"))
                .setContentIntent(pendingIntent)
                .setSmallIcon(hs__chat_icon)
                .setAutoCancel(true);
        if (builder != null) {
            android.app.Notification notification = builder.build();
            notification = attachChannelId(notification, context, mNotificationManager);
            mNotificationManager.notify(0, notification);
        }
    }

    private android.app.Notification attachChannelId(android.app.Notification notification, android.content.Context context, android.app.NotificationManager mNotificationManager) {
        String id = "COCOS_SAMPLE_APP_CHANNEL";
        String name = "CocosChannel";

        int importance = 0;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N) {
            importance = android.app.NotificationManager.IMPORTANCE_HIGH;
        }

        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O &&
                com.helpshift.util.ApplicationUtil.getTargetSDKVersion(context) >= android.os.Build.VERSION_CODES.O) {

            android.app.NotificationChannel mChannel = new android.app.NotificationChannel(id, name, importance);

            mNotificationManager.createNotificationChannel(mChannel);
            // Not setting channel id on NotificationCompat#Builder because the api is only present
            // starting with support lib version 26.
            android.app.Notification.Builder builder = android.app.Notification.Builder.recoverBuilder(context, notification);
            builder.setChannelId(id);
            return builder.build();
        }
        return notification;
    }

    @Override
    public void onNewToken(String newToken) {
        HSLogger.d(TAG, "Push token received: " + newToken);
        if (!newToken.isEmpty()) {
            HelpshiftCocosBridge.registerPushToken(newToken);
        }
    }
}
