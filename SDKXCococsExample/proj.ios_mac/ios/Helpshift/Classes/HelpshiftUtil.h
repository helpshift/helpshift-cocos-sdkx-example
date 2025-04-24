//  HelpshiftUtil.h
//  Helpshift Cocos2dx iOS SDK
//

#include "cocos2d.h"
@import UIKit;

#pragma mark - Common Utility methods.
NSString *hsParseCString(const char *cstring);
NSDictionary *hsValueMapToNSDictionary(cocos2d::ValueMap& dict);
void hsAddObjectToNSArray(const cocos2d::Value& value, NSMutableArray *array);
void hsAddObjectToNSDict(const std::string& key, const cocos2d::Value& value, NSMutableDictionary *dict);
cocos2d::ValueMap hsNSDictionaryToValueMap(NSDictionary<NSString *, id> *dict);
char *formatLogMessage(const char *prefix, const char *tag, const char *log);
NSDictionary *checkAndModifyDateCIF(NSDictionary<NSString *, id>  *configMap);
void handleNestedArray(NSArray *nestedArray, std::vector<cocos2d::Value> &cppVector);
void handleNestedDictionary(NSDictionary *nestedDict, cocos2d::ValueMap &cppMap);
