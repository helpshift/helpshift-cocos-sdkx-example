/**
 * Copyright (c) 2015-2019 by Helpshift, Inc. All Rights Reserved.
 */

#ifndef HelpshiftLog_h
#define HelpshiftLog_h

class HelpshiftLog
{
public:
    static int v(const char *tag, const char *log);
    static int d(const char *tag, const char *log);
    static int i(const char *tag, const char *log);
    static int w(const char *tag, const char *log);
    static int e(const char *tag, const char *log);
    static int log(const char *log);
};

#endif /* HelpshiftLog_h */
