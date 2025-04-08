package org.cocos2dx.cpp;

public class DeepLinkActivity extends android.app.Activity {

    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        android.content.Intent intent = getIntent();
        android.net.Uri data = intent.getData();

        String scheme = data.getScheme();
        String host = data.getHost();

        java.util.HashMap<String, Object> configMap = new java.util.HashMap<>();

        if (scheme != null && host != null) {
            if (scheme.equals("myscheme") && host.equals("example.com")) {
                if (data.getQueryParameter("faqid") != null) {
                    com.helpshift.HelpshiftCocosBridge.showSingleFAQ(data.getQueryParameter("faqid"), configMap);
                } else if (data.getQueryParameter("sectionid") != null) {
                    com.helpshift.HelpshiftCocosBridge.showFAQSection(data.getQueryParameter("sectionid"), configMap);
                } else {
                    com.helpshift.HelpshiftCocosBridge.showFAQs(configMap);
                }
                finish();
            } else {
                android.widget.Toast.makeText(this, "Invalid link to handle", android.widget.Toast.LENGTH_SHORT).show();
            }
        }
    }
}