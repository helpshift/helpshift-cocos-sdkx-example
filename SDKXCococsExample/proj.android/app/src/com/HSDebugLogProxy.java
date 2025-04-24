package com.helpshift;

public class HSDebugLogProxy {
    private enum LogLevel {
        DEBUG,
        ERROR,
        WARNING,
        VERBOSE,
        INFO,
        WTF
    }

    private static int logger(LogLevel logLevel, String logTag, String logMessage) {
        switch (logLevel) {
            case DEBUG:
                return HSDebugLog.d(logTag, logMessage);
            case ERROR:
                return HSDebugLog.e(logTag, logMessage);
            case WARNING:
                return HSDebugLog.w(logTag, logMessage);
            case VERBOSE:
                return HSDebugLog.v(logTag, logMessage);
            case INFO:
                return HSDebugLog.i(logTag, logMessage);
            default:
                return HSDebugLog.wtf(logTag, logMessage);
        }
    }

    public static int debug(String tag, String logMessage) {
        return logger(LogLevel.DEBUG, tag, logMessage);
    }

    public static int error(String tag, String logMessage) {
        return logger(LogLevel.ERROR, tag, logMessage);
    }

    public static int warning(String tag, String logMessage) {
        return logger(LogLevel.WARNING, tag, logMessage);
    }

    public static int verbose(String tag, String logMessage) {
        return logger(LogLevel.VERBOSE, tag, logMessage);
    }

    public static int info(String tag, String logMessage) {
        return logger(LogLevel.INFO, tag, logMessage);
    }

    public static int wtf(String tag, String logMessage) {
        return logger(LogLevel.WTF, tag, logMessage);
    }
}

