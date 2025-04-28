package com.helpshift;

import com.helpshift.proactive.HelpshiftProactiveAPIConfigCollector;

import java.util.Map;

public class HSCocos2dxProactiveListener implements HelpshiftProactiveAPIConfigCollector {
    @Override
    public Map<String, Object> getAPIConfig() {
        return HelpshiftCocosBridge.getAPIConfigFromCocos();
    }
}
