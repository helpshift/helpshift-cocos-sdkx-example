/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */

 #include "HelpshiftUtil.h"


 using namespace std;

 /** Parses the C++ valueMap and creates a Java HashMap to pass to the native layer
  *
  *  @param env : The JNIEnv
  *  @param valueMap : The map which contains various config options accepted by SDK X
  */

 jobject HelpshiftUtil::parseValueMapToHashMap(JNIEnv *env, cocos2d::ValueMap valueMap) {
     if (valueMap.empty()) {
         return NULL;
     }
     const char *hashmap_class_name = "java/util/HashMap";
     jclass clsHashMap = env->FindClass(hashmap_class_name);
     jmethodID constructorID = env->GetMethodID(clsHashMap, "<init>", "()V");

     jobject jmapobj = env->NewObject(clsHashMap, constructorID);

     jmethodID map_put_method = 0;
     map_put_method = env->GetMethodID(clsHashMap, "put",
                                       "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

     jstring key;

     for (auto iter = valueMap.begin(); iter != valueMap.end(); ++iter) {
         key = env->NewStringUTF(iter->first.c_str());
         if (iter->second.getType() == cocos2d::Value::Type::STRING) {
             std::string valueStr = valueMap[iter->first].asString();
             if (!valueStr.empty()) {
                 jstring value = env->NewStringUTF(valueStr.c_str());
                 env->CallObjectMethod(jmapobj, map_put_method, key, value);
             }
         } else if (iter->second.getType() == cocos2d::Value::Type::INTEGER) {
             int value = valueMap[iter->first].asInt();
             jobject jvalue = env->NewObject(
                     env->FindClass("java/lang/Integer"),
                     env->GetMethodID(env->FindClass("java/lang/Integer"), "<init>", "(I)V"),
                     value);
             env->CallObjectMethod(jmapobj, map_put_method, key, jvalue);
         } else if (iter->second.getType() == cocos2d::Value::Type::DOUBLE) {
             double value = valueMap[iter->first].asDouble();
             jobject jvalue = env->NewObject(
                     env->FindClass("java/lang/Double"),
                     env->GetMethodID(env->FindClass("java/lang/Double"), "<init>", "(D)V"), value);
             env->CallObjectMethod(jmapobj, map_put_method, key, jvalue);
         } else if (iter->second.getType() == cocos2d::Value::Type::BOOLEAN) {
             bool value = valueMap[iter->first].asBool();
             jobject jvalue = env->NewObject(
                     env->FindClass("java/lang/Boolean"),
                     env->GetMethodID(env->FindClass("java/lang/Boolean"), "<init>", "(Z)V"),
                     static_cast<jboolean>(value));
             env->CallObjectMethod(jmapobj, map_put_method, key, jvalue);
         } else if (iter->second.getType() == cocos2d::Value::Type::VECTOR) {
             cocos2d::ValueVector &array = iter->second.asValueVector();
             if (array.front().getType() == cocos2d::Value::Type::MAP) {
                 env->CallObjectMethod(jmapobj, map_put_method, key,
                                       HelpshiftUtil::parseValueVectorToArrayOfHashMaps(env,
                                                                                        array));
             } else {
                 env->CallObjectMethod(jmapobj, map_put_method, key,
                                       HelpshiftUtil::parseValueVectorToArray(env, array));
             }
         } else if (iter->second.getType() == cocos2d::Value::Type::MAP) {
             env->CallObjectMethod(jmapobj, map_put_method, key,
                                   parseValueMapToHashMap(env, iter->second.asValueMap()));
         }
     }
     return jmapobj;
 }

 /** Parses the C++ vector and creates array of Java HashMaps to pass to the native layer
   *
   *  @param env : The JNIEnv
   *  @param valueVector : The vector which contains array of Hashmaps as config options accepted by SDK X
   */

 jobject HelpshiftUtil::parseValueVectorToArrayOfHashMaps(JNIEnv *env, cocos2d::ValueVector &valueVector) {
     if (valueVector.empty()) {
         return NULL;
     }
     const char *arraylist_class_name = "java/util/ArrayList";
     jclass clsArrayList = env->FindClass(arraylist_class_name);
     jmethodID arrayConstructorID = env->GetMethodID(clsArrayList, "<init>", "()V");
     jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
     jmethodID array_add_method = 0;
     array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

     for (const auto &map: valueVector) {
         jobject value = HelpshiftUtil::parseValueMapToHashMap(env,
                                                               (cocos2d::ValueMap &) map.asValueMap());
         env->CallBooleanMethod(jarrayobj, array_add_method, value);
     }
     return jarrayobj;
 }

 /** Parses the C++ vector and creates Java array to pass to the native layer
    *
    *  @param env : The JNIEnv
    *  @param valueVector : The vector which contains array elements as config options accepted by SDK X
    */

 jobject HelpshiftUtil::parseValueVectorToArray (JNIEnv *env, cocos2d::ValueVector& valueVector) {
     if (valueVector.empty()) {
         return NULL;
     }
     const char *arraylist_class_name = "java/util/ArrayList";
     jclass clsArrayList = env->FindClass(arraylist_class_name);
     jmethodID arrayConstructorID = env->GetMethodID(clsArrayList, "<init>", "()V");
     jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
     jmethodID array_add_method = 0;
     array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

     for (const auto &val: valueVector) {
         std::string valStr = val.asString();
         if (!valStr.empty()) {
             jstring value = env->NewStringUTF(valStr.c_str());
             env->CallBooleanMethod(jarrayobj, array_add_method, value);
         }
     }
     return jarrayobj;
 }

 /**
   * Convert java Map to cocos2d::ValueMap to use in cpp files
   * @param env : The JNIEnv
   * @param javaMap : Java Map from Java Layer
   * @return
   */

  cocos2d::ValueMap HelpshiftUtil::convertJavaMapToValueMap(JNIEnv *env, jobject javaMap) {
     jclass mapClass = env->GetObjectClass(javaMap);
     jmethodID entrySetMethod = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
     jclass setClass = env->FindClass("java/util/Set");
     jmethodID iteratorMethod = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
     jclass iteratorClass = env->FindClass("java/util/Iterator");
     jmethodID hasNextMethod = env->GetMethodID(iteratorClass, "hasNext", "()Z");
     jmethodID nextMethod = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

     jclass entryClass = env->FindClass("java/util/Map$Entry");
     jmethodID getKeyMethod = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
     jmethodID getValueMethod = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

     // Call entrySet() on the Java map to get the set of key-value pairs
     jobject entrySet = env->CallObjectMethod(javaMap, entrySetMethod);

     // Get an iterator for the set of entries
     jobject iterator = env->CallObjectMethod(entrySet, iteratorMethod);

     cocos2d::ValueMap valueMap;

     // Iterate over each entry in the map
     while (env->CallBooleanMethod(iterator, hasNextMethod)) {
         jobject entry = env->CallObjectMethod(iterator, nextMethod);

         // Get the key and value from the entry
         jobject key = env->CallObjectMethod(entry, getKeyMethod);
         jobject value = env->CallObjectMethod(entry, getValueMethod);

         // Convert the key to a C++ string
         auto keyString = static_cast<jstring>(key);
         const char *keyChars = env->GetStringUTFChars(keyString, nullptr);
         std::string keyStr(keyChars);
         env->ReleaseStringUTFChars(keyString, keyChars);

         // Convert the value to a C++ Value
         cocos2d::Value cocosValue = HelpshiftUtil::convertJavaObjectToValue(env, value);

         // Add the key-value pair to the ValueMap
         valueMap[keyStr] = cocosValue;

         // Release local references
         env->DeleteLocalRef(entry);
     }

     // Release local references
     env->DeleteLocalRef(iterator);
     env->DeleteLocalRef(entrySet);

     return valueMap;
 }

  cocos2d::Value HelpshiftUtil::convertJavaObjectToValue(JNIEnv *env, jobject javaObject) {

     if (javaObject == nullptr) {
         return {};
     }

     jclass objectClass = env->GetObjectClass(javaObject);

     // Check for specific types and convert accordingly
     if (env->IsInstanceOf(javaObject, env->FindClass("java/lang/String"))) {
         auto valueString = static_cast<jstring>(javaObject);
         const char *valueChars = env->GetStringUTFChars(valueString, nullptr);
         std::string valueStr(valueChars);
         env->ReleaseStringUTFChars(valueString, valueChars);
         return cocos2d::Value(valueStr);
     } else if (env->IsInstanceOf(javaObject, env->FindClass("java/lang/Integer"))) {
         jint intValue = env->CallIntMethod(javaObject,
                                            env->GetMethodID(objectClass, "intValue", "()I"));
         return cocos2d::Value(static_cast<int>(intValue));
     } else if (env->IsInstanceOf(javaObject, env->FindClass("java/lang/Float"))) {
         jfloat floatValue = env->CallFloatMethod(javaObject,
                                                  env->GetMethodID(objectClass, "floatValue",
                                                                   "()F"));
         return cocos2d::Value(static_cast<float>(floatValue));
     } else if (env->IsInstanceOf(javaObject, env->FindClass("java/lang/Double"))) {
         jdouble doubleValue = env->CallDoubleMethod(javaObject,
                                                     env->GetMethodID(objectClass, "doubleValue",
                                                                      "()D"));
         return cocos2d::Value(static_cast<double>(doubleValue));
     } else if (env->IsInstanceOf(javaObject, env->FindClass("java/lang/Boolean"))) {
         jboolean boolValue = env->CallBooleanMethod(javaObject,
                                                     env->GetMethodID(objectClass, "booleanValue",
                                                                      "()Z"));
         return cocos2d::Value(static_cast<bool>(boolValue));
     } else {
        /**
         * Converting Java object to String
         */
         cocos2d::JniMethodInfo minfo;
         bool hasMethod = cocos2d::JniHelper::getMethodInfo(minfo,
                                                            "java/lang/Object",
                                                            "toString",
                                                            "()Ljava/lang/String;");
         if (hasMethod) {
             auto result = (jstring)env->CallObjectMethod(javaObject, minfo.methodID);
             if (result != nullptr) {
                 const char *utfString = env->GetStringUTFChars(result, nullptr);
                 std::string cppString = utfString;
                 env->ReleaseStringUTFChars(result, utfString);
                 env->DeleteLocalRef(result);
                 return cocos2d::Value(cppString);
             }
         }
     }
     return {};
 }