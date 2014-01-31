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

USING_NS_CC;
USING_NS_CC_EXT;

#define FONT_NAME       "Arial"
#define FONT_SIZE       22

class TextFieldTTFActionTest;

bool isCharacter(const char Character);
bool isNumber(const char Character);
bool isValidEmailAddress(const char * email);

class LoginScene : public CCScene
{
public:
	LoginScene();
	~LoginScene();
    
	virtual bool init();
	CREATE_FUNC(LoginScene);
        
    
private:
	void loginCallback(CCObject* pSender);
    void backButton(CCObject *sender);
    bool chechUserInput();
    void initUserData();
    void addTextInputLayer();
    void onHttpRequestCompleted(CCNode *sender, void *data);

    
private:
    CCSprite *pLoginButton;
    TextFieldTTFActionTest *pUsernameTextField, *pEmailTextField;
};


//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate
{
public:
    KeyboardNotificationLayer();
    
    virtual std::string subtitle() = 0;
    virtual void onClickTrackNode(bool bClicked) = 0;
    
    virtual void registerWithTouchDispatcher();
    virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);
    
    // CCLayer
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    cocos2d::CCNode * m_pTrackNode;
    cocos2d::CCPoint  m_beginPos;
};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFActionTest : public KeyboardNotificationLayer, public cocos2d::CCTextFieldDelegate
{
    cocos2d::CCTextFieldTTF *    m_pTextField;
    cocos2d::CCSprite *    m_pUsername;
    cocos2d::CCAction *          m_pTextFieldAction;
    bool                m_bAction;
    int                 m_nCharLimit;       // the textfield max char limit
    int h,w, length;
    CCPoint p;
    const char *m_pPlaceholder;
    
    
public:
    TextFieldTTFActionTest(int w, int h, const char* pPlaceHolder, CCPoint p, int length);
    TextFieldTTFActionTest();
    
    void callbackRemoveNodeWhenDidAction(cocos2d::CCNode * pNode);
    const char *getText() {return m_pTextField->getString();}
    
    // KeyboardNotificationLayer
    virtual std::string subtitle();
    virtual void onClickTrackNode(bool bClicked);
    
    // CCLayer
    virtual void onEnter();
    virtual void onExit();
    
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * pSender);
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF * pSender, const char * delText, int nLen);
    virtual bool onDraw(cocos2d::CCTextFieldTTF * pSender);
};


#endif /* defined(__TrivialJokoa__LoginScene__) */
