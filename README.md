# Helpshift SDK X Cocos Example App

Sample Cocos project demonstrating the integration of Helpshift Cocos SDK X

## Requirements

* See Helpshift SDK X requirements [here](https://developers.helpshift.com/sdkx-cocos2dx/getting-started/)

## Import project

1. Clone the repositiory
2. For Android app, Open `SDKXCococsExample/proj.android/` in Android Studio 

## Building the project

---
**FOR ANDROID**

Please follow these steps to build the app:
* Update your Helpshift App credentials in `SDKXCococsExample/proj.android/app/src/org/cocos2dx/cpp/AppActivity.java` file. 
    * Replace domain name in `<your-domain>.helpshift.com` and appId in `<your-app-android-app-id>` string placeholders in this file.
    * To get your Helpshift app credentials please check [here](https://developers.helpshift.com/sdkx-cocos2dx/getting-started-android/).



FCM push notification is already integrated in the example app but we have not provided the `google-services.json` file.

* You can configure FCM by providing your own `google-services.json` file at `SDKXCococsExample/proj.android/app/` location.
* You can then provide the FCM API Key in Helpshift Dashboard as mentioned [here](https://developers.helpshift.com/sdkx-cocos2dx/notifications-android#push-via-helpshift)
* Make sure to have same package name for the app your `google-services.json` file as it is in Cocos Android Porject

---

Build the project in your app Editor and Run on your device.

## Example feature implementations

### Initializing Helpshift SDK via `Install`

* Refer to `SDKXCococsExample/proj.android/app/src/org/cocos2dx/cpp/AppActivity.java` or `ios-file-path` class.
* Notice that we have initialized the SDK as soon as the app is launched.


NOTE: `Helpshift Install` must be called before invoking any other api in the Helpshift SDK. 

### User Hub

* Refer to the following functions in `SDKXCococsExample/Classes/IdentityUserScene.cpp` in example code for User Hub related integration.
    * `loginWithIdentity`
    * `addUserIdentities`
    * `updateMasterAttributes`
    * `updateAppAttributes`
* Please check Developer Documentation for more on [User Hub APIs](https://developers.helpshift.com/sdkx-cocos2dx/identity-user-hub/)

### User Identification & Management

* Refer to the following functions in `SDKXCococsExample/Classes/LegacyUserScene.cpp` example code for User Identification and Management related integration
    * `login`
    * `logout`
* Please check Developer Documentation for more on [User Management](https://developers.helpshift.com/sdkx_android/users/)

### SDK Configurations

* Refer to the following function in `SDKXCococsExample/Classes/HelloWorldScene.cpp` for SDK configurations:
    * `getConfigMap`: This function reads config as set by you from UI and constructs a Dictionary as expected by the SDK. 
* It contains custom example for CIF, please modify according to your needs.
* Many other configurations are picked from the example app UI.
* Developer Documentation for [Configurations](https://developers.helpshift.com/sdkx-cocos2dx/sdk-configuration/)

### Showing Conversation/FAQ screens, Breadcrumbs, Logs, setting Language etc

* For example code of various other features please refer to code examples in [HelloWorldScene](SDKXCococsExample/Classes/HelloWorldScene.cpp)
    * `showConversation`
    * `ShowFAQs`
    * `ShowFAQSection`
    * `AddDebugLog`
    * `setLanguage`
    * `leaveBreadcrumb`
    * `clearBreadCrumbs`
    * `requestUnreadMessageCount`
    * many others...
* The code is easy to interpret since each button on UI has been linked with a feature.
* For example if you need example code for showing Conversation Screen, refer function `showConversation` in the HelloWorldScene.cpp file.
* Developer Documentation for [Helpshift's APIs](https://developers.helpshift.com/sdkx-cocos2dx/support-tools/)

## License

```
Copyright 2025, Helpshift, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
