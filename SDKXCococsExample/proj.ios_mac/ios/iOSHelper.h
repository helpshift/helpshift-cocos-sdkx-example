
// iOSHelper.h

#ifndef iOSHelper_h
#define iOSHelper_h

#include <string>

void scheduleLocalNotification(const std::string& title, const std::string& body, int delaySeconds);
void purgeData();
#endif /* iOSHelper_h */
