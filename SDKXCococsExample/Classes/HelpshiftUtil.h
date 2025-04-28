/**
 * Copyright (c) 2015-2019 by Helpshift, Inc. All Rights Reserved.
 *
 */
#ifndef HelpshiftUtil_h
#define HelpshiftUtil_h

#include "cocos2d.h"


class HelpshiftUtil
{
public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static jobject parseValueVectorToArray (JNIEnv *env, cocos2d::ValueVector& valueVector);
    static jobject parseValueMapToHashMap (JNIEnv *env, cocos2d::ValueMap valueMap);
    static jobject parseValueVectorToArrayOfHashMaps (JNIEnv *env, cocos2d::ValueVector& valueVector);

    static cocos2d::ValueMap convertJavaMapToValueMap(JNIEnv *env, jobject javaMap);
    static cocos2d::Value convertJavaObjectToValue(JNIEnv *env, jobject javaObject);
#endif
};

#endif
