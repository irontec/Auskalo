//facebook
/*if(![[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"fb://profile/181239321918026"]])
{
[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://www.facebook.com/plungeinteractive"]];
}

//twitter
     if(![[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"twitter://user?screen_name=plungeint"]])
     {
     [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://twitter.com/plungeint"]];
         }*/









//
//  TrivialJokoaAppDelegate.h
//  TrivialJokoa
//
//  Created by iker on 05/12/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"

/**
@brief    The cocos2d Application.

The reason to implement with private inheritance is to hide some interface details of CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function is called when the application enters the background
    @param  the pointer of the application instance
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function is called when the application enters the foreground
    @param  the pointer of the application instance
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

