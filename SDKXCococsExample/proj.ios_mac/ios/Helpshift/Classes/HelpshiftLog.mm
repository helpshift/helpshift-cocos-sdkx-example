//
//  HelpshiftLog.mm
//  Helpshift Cocos2dx iOS SDK
//

#include "HelpshiftLog.h"
#import "HelpshiftUtil.h"

@import HelpshiftX;

int HelpshiftLog::v(const char *tag, const char *log)
{
    const char *prefix = "HelpshiftLog:Verbose::";
    [Helpshift addLog:hsParseCString(formatLogMessage(prefix,tag,log))];
    return 0;
}

int HelpshiftLog::d(const char *tag, const char *log)
{
    const char *prefix = "HelpshiftLog:Debug::";
    [Helpshift addLog:hsParseCString(formatLogMessage(prefix,tag,log))];
    return 0;
}

int HelpshiftLog::i(const char *tag, const char *log)
{
    const char *prefix = "HelpshiftLog:Info::";
    [Helpshift addLog:hsParseCString(formatLogMessage(prefix,tag,log))];
    return 0;
}

int HelpshiftLog::w(const char *tag, const char *log)
{
    const char *prefix = "HelpshiftLog:Warn::";
    [Helpshift addLog:hsParseCString(formatLogMessage(prefix,tag,log))];
    return 0;
}

int HelpshiftLog::e(const char *tag, const char *log)
{
    const char *prefix = "HelpshiftLog:Error::";
    [Helpshift addLog:hsParseCString(formatLogMessage(prefix,tag,log))];
    return 0;
}

int HelpshiftLog::log(const char *log)
{
    [Helpshift addLog:hsParseCString(log)];
    return 0;
}