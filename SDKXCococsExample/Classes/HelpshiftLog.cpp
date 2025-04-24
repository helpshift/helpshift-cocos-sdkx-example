
#include "HelpshiftLog.h"
#include <string>
#include <jni/JniHelper.h>

using namespace std;

/** Calls the Helpshift HSDebugLogProxy class with provided arguments
 *
 * @param logFunction : corresponds to the log level
 * @param tagString : the debug tag to use
 * @param logString : the log message
 */
int logger(const char *logLevel, const char *tagString, const char *logString) {
    if (logString == NULL || strlen(logString) == 0) {
        return -1;
    }

    cocos2d::JniMethodInfo minfo;
    int retVal = -1;
    if (cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                "com/helpshift/HSDebugLogProxy",
                                                logLevel,
                                                "(Ljava/lang/String;Ljava/lang/String;)I")) {
        jstring logStr = minfo.env->NewStringUTF(logString);
        jstring tagStr = minfo.env->NewStringUTF(tagString);
        retVal = minfo.env->CallStaticIntMethod(minfo.classID,
                                                minfo.methodID,
                                                tagStr,
                                                logStr);
        minfo.env->DeleteLocalRef(tagStr);
        minfo.env->DeleteLocalRef(logStr);
    }

    return retVal;
}

/**
 * Add additional debug logs to the issue filed by user
 * @param tag : Tag for identifying the log
 * @param logMessage : message that you want to log
 * @return
 */
int HelpshiftLog::d(const char *tag, const char *logMessage) {
    return logger("debug", tag, logMessage);
}

int HelpshiftLog::e(const char *tag, const char *logMessage) {
    return logger("error", tag, logMessage);
}

int HelpshiftLog::w(const char *tag, const char *logMessage) {
    return logger("warning", tag, logMessage);
}

int HelpshiftLog::i(const char *tag, const char *logMessage) {
    return logger("info", tag, logMessage);
}

int HelpshiftLog::v(const char *tag, const char *logMessage) {
    return logger("verbose", tag, logMessage);
}

int HelpshiftLog::log(const char *log) {
    return logger("wtf", "", log);
}

