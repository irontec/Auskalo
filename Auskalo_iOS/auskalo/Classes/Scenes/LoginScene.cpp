//
//  LoginScene.cpp
//  TrivialJokoa
//
//  Created by iker on 12/12/12.
//
//

#include "../Scenes/LoginScene.h"
#include "../Helpers/SQLHelper.h"
#include "../Helpers/VisibleRect.h"
#include "../Scenes/MainMenuScene.h"
#include "../Helpers/TrivialAPI.h"
#include "SimpleAudioEngine.h"
#include "../Helpers/SpriteButton.h"
#include "../Scenes/ProfilaScene.h"

using namespace CocosDenshion;


LoginScene::~LoginScene()
{
	
}

LoginScene::LoginScene()
{
    
}

bool LoginScene::init()
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //CCLayerGradient *pLayer = CCLayerGradient::create(ccc4(154, 182, 0, 255), ccc4(133, 160, 0, 255));
    
    
    CCLayerColor *pLayer = CCLayerColor::create();
    pLayer->setContentSize(winSize);
    
    CCSprite *fondo = CCSprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());

    pLayer->addChild(fondo);
    
    addChild(pLayer);
    
    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(LoginScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(ccp(VisibleRect::left().x, (VisibleRect::top().y - 4)));
    addChild(pBackButton);

   
    CCPoint p;
    
    pUsernameTextField = new TextFieldTTFActionTest(213, 56, "Erabiltzailea", p, 8);
    pUsernameTextField->autorelease();
    pUsernameTextField->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 56 - 20));
    addChild(pUsernameTextField, 2);
    
    pEmailTextField = new TextFieldTTFActionTest(213, 56, "E-mail", p, 50);
    pEmailTextField->autorelease();
    pEmailTextField->setPosition(ccp(VisibleRect::center().x, pUsernameTextField->getPosition().y - 56 - 15));
    addChild(pEmailTextField, 2);
    
    
    CCLabelTTF *pLabel = CCLabelTTF::create("Sartu", "fonts/PT_Sans-Web-Regular.ttf", 18.0);
    CCScale9Sprite *sprite = CCScale9Sprite::create("botoia_normal.png");
    
    CCControlButton *pSartu = CCControlButton::create(pLabel, sprite);
    pSartu->addTargetWithActionForControlEvents(this, cccontrol_selector(LoginScene::loginCallback), CCControlEventTouchUpInside);
    pSartu->setPreferredSize(CCSizeMake(213, 56));    
    pSartu->setPosition(ccp(origin.x + visibleSize.width/2, pEmailTextField->getPosition().y - 56 - 15));
    addChild(pSartu, 1);

	return true;
}

void LoginScene::backButton(CCObject *sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    ProfilaScene *scene = ProfilaScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LoginScene::loginCallback(CCObject* pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathForFilename("sounds/button.wav")).c_str());
    if (!LoginScene::chechUserInput()) {
        CCMessageBox("Erabiltzailea eta E-mail zuzena sartu", "error");
    } else {
        std::string *postData = new std::string("{\"method\":\"erabiltzaileBerria\", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":{\"izena\":\"");
        
        postData->append(pUsernameTextField->getText());
        postData->append("\", \"email\":\"");
        postData->append(pEmailTextField->getText());
        postData->append("\"}}");
        
        CCHttpRequest* request = new CCHttpRequest();
        request->setUrl(URL);
        request->setRequestType(CCHttpRequest::kHttpPost);
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/x-www-form-urlencoded");
        request->setHeaders(headers);
        request->setResponseCallback(this, callfuncND_selector(LoginScene::onHttpRequestCompleted));
        request->setRequestData(postData->c_str(), strlen(postData->c_str()));
        CCHttpClient::getInstance()->send(request);
    }
}

void LoginScene::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
        return;
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        CCLog("HTTP Status Code: %d", response->getResponseCode());
        
        std::vector<char> *buffer = response->getResponseData();
        
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
        
        CCMessageBox("Error", "Interneta beharrezkoa da");
        
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    
    char *uid = strdup(result->valuestring);
    
    cJSON_Delete(root);
    
    CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();
    
    pUserDefault->setStringForKey("username", pUsernameTextField->getText());
    pUserDefault->setStringForKey("email", pEmailTextField->getText());
    pUserDefault->setStringForKey("uid", uid);
    pUserDefault->flush();
    
    MainMenuScene *scene = MainMenuScene::create();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);    
}


bool LoginScene::chechUserInput()
{
    const char *pUsername = pUsernameTextField->getText();
    const char *pEmail = pEmailTextField->getText();
    
    if (pUsername == NULL || pEmail == NULL)
        return false;

    if (!isValidEmailAddress(pEmail))
        return false;
    
    return true;
}

bool isCharacter(const char Character)
{
	return ( (Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
}

bool isNumber(const char Character)
{
	return ( Character >= '0' && Character <= '9');
}

bool isValidEmailAddress(const char * email)
{
	if(!email)
		return 0;
	if(!isCharacter(email[0]))
		return 0;
	int AtOffset = -1;
	int DotOffset = -1;
	unsigned int Length = strlen(email);
    
	for(unsigned int i = 0; i < Length; i++)
	{
		if(email[i] == '@')
			AtOffset = (int)i;
		else if(email[i] == '.')
			DotOffset = (int)i;
	}
    
	if(AtOffset == -1 || DotOffset == -1)
		return 0;
	if(AtOffset > DotOffset)
		return 0;
	return !(DotOffset >= ((int)Length-1));
}


static CCRect getRect(CCNode * pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

KeyboardNotificationLayer::KeyboardNotificationLayer()
: m_pTrackNode(0)
{
    setTouchEnabled(true);
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    /*if (! m_pTrackNode)
    {
        return;
    }
    
    CCRect rectTracked = getRect(m_pTrackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    int count = children->count();
    CCPoint pos;
    for (int i = 0; i < count; ++i)
    {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y += adjustVert - 160;
        node->setPosition(pos);
    }*/
}

void KeyboardNotificationLayer::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    
   /* CCRect rectTracked = getRect(m_pTrackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    int count = children->count();
    CCPoint pos;
    for (int i = 0; i < count; ++i)
    {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y -= adjustVert + 160;
        node->setPosition(pos);
    }*/
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    m_beginPos = pTouch->getLocation();
    return true;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (! m_pTrackNode)
    {
        return;
    }
    
    CCPoint endPos = pTouch->getLocation();
    
    float delta = 5.0f;
    if (::abs(endPos.x - m_beginPos.x) > delta
        || ::abs(endPos.y - m_beginPos.y) > delta)
    {
        // not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    CCRect rect;
    CCPoint point = convertTouchToNodeSpaceAR(pTouch);
    CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);
    
    rect = getRect(m_pTrackNode);
    CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
          rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    this->onClickTrackNode(rect.containsPoint(point));
    CCLOG("----------------------------------");
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFActionTest::subtitle()
{
    return "CCTextFieldTTF with action and char limit test";
}

TextFieldTTFActionTest::TextFieldTTFActionTest(int w, int h, const char* pPlaceHolder, CCPoint p, int length)
{
    this->m_pPlaceholder = pPlaceHolder;
    this->w = w;
    this->h = h;
    this->p = p;
    this->length = length;
}

void TextFieldTTFActionTest::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)m_pTrackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFActionTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();
    
    m_pUsername = CCSprite::create("input.png");
    m_pUsername->setPosition(p);
    addChild(m_pUsername);
    
    m_nCharLimit = length;
    
    m_pTextFieldAction = CCRepeatForever::create(
                                                 (CCActionInterval*)CCSequence::create(
                                                                                       CCFadeOut::create(0.25),
                                                                                       CCFadeIn::create(0.25),
                                                                                       0
                                                                                       ));
    m_pTextFieldAction->retain();
    m_bAction = false;
    
    // add CCTextFieldTTF
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder(m_pPlaceholder,
                                                            FONT_NAME,
                                                            FONT_SIZE);
    m_pTextField->setColor(ccc3(100, 100, 100));
    addChild(m_pTextField);
    
    m_pTextField->setDelegate(this);
    
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // on android, CCTextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    m_pTextField->setPosition(ccp(s.width / 2, s.height/2 + 50));
#else
    m_pTextField->setPosition(ccp(s.width / 2, s.height / 2));
#endif*/
    
    m_pTrackNode = m_pTextField;
}

void TextFieldTTFActionTest::onExit()
{
    KeyboardNotificationLayer::onExit();
    m_pTextFieldAction->release();
}

// CCTextFieldDelegate protocol
bool TextFieldTTFActionTest::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{
    if (! m_bAction)
    {
        m_pTextField->runAction(m_pTextFieldAction);
        m_bAction = true;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{
    if (m_bAction)
    {
        m_pTextField->stopAction(m_pTextFieldAction);
        m_pTextField->setOpacity(255);
        m_bAction = false;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    
    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (pSender->getCharCount() >= m_nCharLimit)
    {
        return true;
    }
    
    // create a insert text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::create(text, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    ccColor3B color = { 255, 255, 255};
    label->setColor(color);
    
    // move the sprite from top to position
    CCPoint endPos = pSender->getPosition();
    if (pSender->getCharCount())
    {
        endPos.x += pSender->getContentSize().width / 2;
    }
    CCSize  inputTextSize = label->getContentSize();
    CCPoint beginPos(endPos.x, CCDirector::sharedDirector()->getWinSize().height - inputTextSize.height * 2);
    
    float duration = 0.5;
    label->setPosition(beginPos);
    label->setScale(8);
    
    CCAction * seq = CCSequence::create(
                                        CCSpawn::create(
                                                        CCMoveTo::create(duration, endPos),
                                                        CCScaleTo::create(duration, 1),
                                                        CCFadeOut::create(duration),
                                                        0),
                                        CCCallFuncN::create(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
                                        0);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen)
{
    // create a delete text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::create(delText, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    
    // move the sprite to fly out
    CCPoint beginPos = pSender->getPosition();
    CCSize textfieldSize = pSender->getContentSize();
    CCSize labelSize = label->getContentSize();
    beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
    
    float duration = 1;
    float rotateDuration = 0.2f;
    int repeatTime = 5;
    label->setPosition(beginPos);
    
    CCAction * seq = CCSequence::create(
                                        CCSpawn::create(
                                                        CCMoveTo::create(duration, endPos),
                                                        CCRepeat::create(
                                                                         CCRotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),
                                                                         repeatTime),
                                                        CCFadeOut::create(duration),
                                                        0),
                                        CCCallFuncN::create(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
                                        0);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onDraw(CCTextFieldTTF * pSender)
{
    return false;
}

void TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction(CCNode * pNode)
{
    this->removeChild(pNode, true);
}
