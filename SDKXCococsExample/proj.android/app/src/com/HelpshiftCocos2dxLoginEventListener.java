package com.helpshift;

import java.util.Map;

public class HelpshiftCocos2dxLoginEventListener implements HelpshiftUserLoginEventsListener {
    @Override
    public void onLoginSuccess() {
        HelpshiftCocosBridge.onLoginSuccess();
    }

    @Override
    public void onLoginFailure(String reason, Map<String, String> map) {
        HelpshiftCocosBridge.onLoginFailure(reason, map);
    }
}
