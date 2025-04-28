package com.helpshift;


import androidx.annotation.NonNull;
import java.util.Map;

public class HelpshiftCocos2dxDelegates implements HelpshiftEventsListener {
    @Override
    public void onEventOccurred(@NonNull String eventName, Map<String, Object> eventData) {
        HelpshiftCocosBridge.onEventOccurred(eventName, eventData);
    }

    @Override
    public void onUserAuthenticationFailure(HelpshiftAuthenticationFailureReason helpshiftAuthenticationFailureReason) {
        HelpshiftCocosBridge.onUserAuthenticationFailure(helpshiftAuthenticationFailureReason.name());
    }
}
