//
//  HelpshiftUtil.mm
//  Helpshift Cocos2dx iOS SDKX
//

#import "HelpshiftUtil.h"

// Recursive function to handle nested dictionaries
void handleNestedDictionary(NSDictionary *nestedDict, cocos2d::ValueMap &cppMap) {
    for (id key in nestedDict) {
        id value = nestedDict[key];
        if ([value isKindOfClass:[NSNumber class]]) {
            double cppValue = [value doubleValue];
            cppMap[std::string([key UTF8String])] = cocos2d::Value(cppValue);
        }
        else if ([value isKindOfClass:[NSString class]]) {
            std::string cppValue = [value UTF8String];
            cppMap[std::string([key UTF8String])] = cocos2d::Value(cppValue);
        }
        else if ([value isKindOfClass:[NSArray class]]) {
            std::vector<cocos2d::Value> nestedCppVector;
            handleNestedArray((NSArray *)value, nestedCppVector);
            cppMap[std::string([key UTF8String])] = cocos2d::Value(nestedCppVector);
        }
        else if ([value isKindOfClass:[NSDictionary class]]) {
            cocos2d::ValueMap nestedCppMap;
            handleNestedDictionary((NSDictionary *)value, nestedCppMap);
            cppMap[std::string([key UTF8String])] = cocos2d::Value(nestedCppMap);
        }
        // Add more else if blocks to handle other types within the nested dictionary as needed
    }
}


// Recursive function to handle nested arrays
void handleNestedArray(NSArray *nestedArray, std::vector<cocos2d::Value> &cppVector) {
    for (id nestedElement in nestedArray) {
        if ([nestedElement isKindOfClass:[NSNumber class]]) {
            double cppElement = [nestedElement doubleValue];
            cppVector.push_back(cocos2d::Value(cppElement));
        }
        else if ([nestedElement isKindOfClass:[NSString class]]) {
            std::string cppElement = [nestedElement UTF8String];
            cppVector.push_back(cocos2d::Value(cppElement));
        }
        else if ([nestedElement isKindOfClass:[NSArray class]]) {
            std::vector<cocos2d::Value> nestedCppVector;
            handleNestedArray((NSArray *)nestedElement, nestedCppVector);
            cppVector.push_back(cocos2d::Value(nestedCppVector));
        }
        else if ([nestedElement isKindOfClass:[NSDictionary class]]) {
            cocos2d::ValueMap nestedCppMap;
            handleNestedDictionary((NSDictionary *)nestedElement, nestedCppMap);
            cppVector.push_back(cocos2d::Value(nestedCppMap));
        }
        // Add more else if blocks to handle other types within the nested array as needed
    }
}

NSString* hsParseCString(const char *cstring) {
    if (cstring == NULL) {
        return NULL;
    }
    NSString * nsstring = [[NSString alloc] initWithBytes:cstring
                                                   length:strlen(cstring)
                                                 encoding:NSUTF8StringEncoding];
    return [nsstring autorelease];
}

void hsAddObjectToNSArray(const cocos2d::Value& value, NSMutableArray *array)
{
    if(value.isNull()) {
        return;
    }
    // add string into array
    if (value.getType() == cocos2d::Value::Type::STRING) {
        NSString *element = [NSString stringWithCString:value.asString().c_str() encoding:NSUTF8StringEncoding];
        [array addObject:element];
    } else if (value.getType() == cocos2d::Value::Type::FLOAT) {
        NSNumber *number = [NSNumber numberWithFloat:value.asFloat()];
        [array addObject:number];
    } else if (value.getType() == cocos2d::Value::Type::DOUBLE) {
        NSNumber *number = [NSNumber numberWithDouble:value.asDouble()];
        [array addObject:number];
    } else if (value.getType() == cocos2d::Value::Type::BOOLEAN) {
        NSNumber *element = [NSNumber numberWithBool:value.asBool()];
        [array addObject:element];
    } else if (value.getType() == cocos2d::Value::Type::INTEGER) {
        NSNumber *element = [NSNumber numberWithInt:value.asInt()];
        [array addObject:element];
    } else if (value.getType() == cocos2d::Value::Type::VECTOR) {
        NSMutableArray *element = [[NSMutableArray alloc] init];
        cocos2d::ValueVector valueArray = value.asValueVector();
        for (const auto &e : valueArray) {
            hsAddObjectToNSArray(e, element);
        }
        [array addObject:element];
    } else if (value.getType() == cocos2d::Value::Type::MAP) {
        NSMutableDictionary *element = [NSMutableDictionary dictionary];
        auto valueDict = value.asValueMap();
        for (auto iter = valueDict.begin(); iter != valueDict.end(); ++iter) {
            hsAddObjectToNSDict(iter->first, iter->second, element);
        }
        [array addObject:element];
    }
}

void hsAddObjectToNSDict(const std::string& key, const cocos2d::Value& value, NSMutableDictionary *dict)
{
    if(value.isNull() || key.empty()) {
        return;
    }
    NSString *keyStr = [NSString stringWithCString:key.c_str() encoding:NSUTF8StringEncoding];
    if (value.getType() == cocos2d::Value::Type::MAP) {
        NSMutableDictionary *dictElement = [[NSMutableDictionary alloc] init];
        cocos2d::ValueMap subDict = value.asValueMap();
        for (auto iter = subDict.begin(); iter != subDict.end(); ++iter) {
            hsAddObjectToNSDict(iter->first, iter->second, dictElement);
        }
        [dict setObject:dictElement forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::FLOAT) {
        NSNumber *number = [NSNumber numberWithFloat:value.asFloat()];
        [dict setObject:number forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::DOUBLE) {
        NSNumber *number = [NSNumber numberWithDouble:value.asDouble()];
        [dict setObject:number forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::BOOLEAN) {
        NSNumber *element = [NSNumber numberWithBool:value.asBool()];
        [dict setObject:element forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::INTEGER) {
        NSNumber *element = [NSNumber numberWithInt:value.asInt()];
        [dict setObject:element forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::STRING) {
        NSString *strElement = [NSString stringWithCString:value.asString().c_str() encoding:NSUTF8StringEncoding];
        [dict setObject:strElement forKey:keyStr];
    } else if (value.getType() == cocos2d::Value::Type::VECTOR) {
        NSMutableArray *arrElement = [NSMutableArray array];
        cocos2d::ValueVector array = value.asValueVector();
        for(const auto& v : array) {
            hsAddObjectToNSArray(v, arrElement);
        }
        [dict setObject:arrElement forKey:keyStr];
    }
}

NSDictionary *hsValueMapToNSDictionary(cocos2d::ValueMap& dict) {
    NSMutableDictionary *nsDict = [NSMutableDictionary dictionary];
    for (auto iter = dict.begin(); iter != dict.end(); ++iter)
    {
        hsAddObjectToNSDict(iter->first, iter->second, nsDict);
    }
    NSMutableDictionary *dateConvertedDict = checkAndModifyDateCIF(nsDict);
    return dateConvertedDict;
}

cocos2d::ValueMap hsNSDictionaryToValueMap(NSDictionary<NSString *, id> *dict) {
    cocos2d::ValueMap cppMap;

    for (NSString* key in dict) {
        std::string cppKey = [key UTF8String];
        id value = [dict objectForKey:key];

        if ([value isKindOfClass:[NSNumber class]]) {
            // If the value is a number, convert it to a double and insert into the ValueMap
            double cppValue = [value doubleValue];
            cppMap[cppKey] = cocos2d::Value(cppValue);
        }
        else if ([value isKindOfClass:[NSString class]]) {
            // If the value is a string, convert it to a string and insert into the ValueMap
            std::string cppValue = [value UTF8String];
            cppMap[cppKey] = cocos2d::Value(cppValue);
        }
        else if ([value isKindOfClass:[NSDictionary class]]) {
            // If the value is another NSDictionary, convert it to a ValueMap recursively
            NSDictionary* subDict = (NSDictionary*)value;
            cppMap[cppKey] = cocos2d::Value(hsNSDictionaryToValueMap(subDict));
        }
        else if ([value isKindOfClass:[NSArray class]]) {
            // If the value is an NSArray, convert it to a ValueVector
            NSArray* array = (NSArray*)value;
            cocos2d::ValueVector cppVector;
            for (id element in array) {
                if ([element isKindOfClass:[NSNumber class]]) {
                    // Convert number elements to double
                    double cppElement = [element doubleValue];
                    cppVector.push_back(cocos2d::Value(cppElement));
                }
                else if ([element isKindOfClass:[NSString class]]) {
                    // Convert string elements
                    std::string cppElement = [element UTF8String];
                    cppVector.push_back(cocos2d::Value(cppElement));
                } else if ([element isKindOfClass:[NSArray class]]) {
                    // Handle nested arrays
                    std::vector<cocos2d::Value> nestedCppVector;
                    handleNestedArray((NSArray *)element, nestedCppVector);
                    cppVector.push_back(cocos2d::Value(nestedCppVector));
                } else if ([element isKindOfClass:[NSDictionary class]]) {
                    // Handle nested dictionaries
                    cocos2d::ValueMap nestedCppMap;
                    handleNestedDictionary((NSDictionary *)element, nestedCppMap);
                    cppVector.push_back(cocos2d::Value(nestedCppMap));
                }
                // Add more else if blocks to handle other types within the array as needed
            }
            cppMap[cppKey] = cocos2d::Value(cppVector);
        }
        // Add more else if blocks to handle other types as needed
    }

    return cppMap;
}

char* formatLogMessage(const char *prefix, const char *tag, const char *log) {
    size_t prefixLength = strlen(prefix);
    size_t tagLength = (tag != nullptr) ? strlen(tag) : 0;
    size_t logLength = (log != nullptr) ? strlen(log) : 0;
    size_t totalLength = prefixLength + tagLength + logLength + 3; // 3 for "::" and null-terminator

    char *formattedLog = new char[totalLength];
    strcpy(formattedLog, prefix);

    if (tag != nullptr) {
        strcat(formattedLog, tag);
    }

    strcat(formattedLog, "::");

    if (log != nullptr) {
        strcat(formattedLog, log);
    }

    return formattedLog;
}

NSDictionary *checkAndModifyDateCIF(NSDictionary<NSString *, id>  *configMap) {

    NSMutableDictionary *mutableConfigMap = [configMap mutableCopy];
    NSArray *keys = [[mutableConfigMap allKeys] copy];

    for (NSString *key in keys) {
        id value = mutableConfigMap[key];
        if ([value isKindOfClass:[NSDictionary class]]) {
            NSDictionary *subDict = checkAndModifyDateCIF((NSDictionary*) value);
            [mutableConfigMap setObject:subDict forKey:key];
        } else if ([key isEqualToString:@"type"] && [value isKindOfClass:[NSString class]] && (([((NSString *)value) isEqualToString:@"date"]) || ([((NSString *)value) isEqualToString:@"dt"]))) {
            NSString *valueString = mutableConfigMap[@"value"];
            long longValue;
            if (valueString) {
                longValue = [valueString longLongValue];
            }
            if (longValue) {
                mutableConfigMap[@"value"] = @(longValue);
            }
        }
    }
    return mutableConfigMap;
}
