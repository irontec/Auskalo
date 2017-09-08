//
//  LoginScene.h
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#ifndef __TrivialJokoa__LoginScene__
#define __TrivialJokoa__LoginScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;

#define FONT_NAME       "Arial"
#define FONT_SIZE       22

bool isCharacter(const char Character);
bool isNumber(const char Character);
bool isValidEmailAddress(const char * email);

class LoginScene : public Scene
{
public:
	LoginScene();
	~LoginScene();
    
	virtual bool init();
	CREATE_FUNC(LoginScene);
        
    
private:
	void loginCallback(Ref* pSender, cocos2d::extension::Control::EventType pCCControlEvent);
    void backButton(Ref* pSender);
    bool chechUserInput();
    void initUserData();
    void addTextInputLayer();
    void onHttpRequestCompleted(HttpClient* client, HttpResponse* response);

    
private:
    Sprite *pLoginButton;
    ui::EditBox *pUsernameTextField, *pEmailTextField;
};


//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public Layer, public IMEDelegate
{
public:
    KeyboardNotificationLayer();
    
    virtual std::string subtitle() = 0;
    virtual void onClickTrackNode(bool bClicked) = 0;
    
    virtual void registerTouchEvents();
    virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(IMEKeyboardNotificationInfo& info);
    
    // Layer
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
protected:
    Node * m_pTrackNode;
    Point  m_beginPos;
};

#endif /* defined(__TrivialJokoa__LoginScene__) */
