package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Bundle;

import com.helpshift.HelpshiftCocosBridge;

public class NotificationActivity extends Activity {
    @Override
    protected void onCreate(@androidx.annotation.Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        android.os.Bundle bundle = getIntent().getExtras();
        if (bundle != null && bundle.getBoolean("proactiveNotification")) {
            String url = bundle.getString("proactiveLink");
            HelpshiftCocosBridge.handleProactiveLink(getApplicationContext(), url);
        }
        else if (bundle != null && bundle.getBoolean("closeHelpshiftSession")) {
            HelpshiftCocosBridge.closeSession();
        }
        finish();
    }
}
