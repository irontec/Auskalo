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
#include "../Helpers/ImageManager.h"

using namespace CocosDenshion;


LoginScene::~LoginScene()
{
	
}

LoginScene::LoginScene()
{
    
}

bool LoginScene::init()
{
    
    Size winSize = Director::getInstance()->getWinSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    //LayerGradient *pLayer = LayerGradient::create(Color4B(154, 182, 0, 255), Color4B(133, 160, 0, 255));
    
    
    LayerColor *pLayer = LayerColor::create();
    pLayer->setContentSize(winSize);
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());

    pLayer->addChild(fondo);
    
    addChild(pLayer);
    
    auto pBackButton = SpriteButton::create(ImageManager::getImage("go-back"), 1.0f, CC_CALLBACK_1(LoginScene::backButton, this));
    pBackButton->setAnchorPoint(Vec2(0, 1));
    pBackButton->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y));
    addChild(pBackButton);

   
    Point p;
    
    pUsernameTextField = ui::EditBox::create(Size(213, 56), ui::Scale9Sprite::create("input.png"));
    pUsernameTextField->setPlaceHolder("Erabiltzailea");
    pUsernameTextField->setFontSize(20);
    pUsernameTextField->setPlaceholderFontSize(20);
    pUsernameTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    pUsernameTextField->setFontColor(Color3B(0,0,0));
    pUsernameTextField->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
    pUsernameTextField->setPosition(Vec2(VisibleRect::center().x, VisibleRect::top().y - 56 - 20));
    addChild(pUsernameTextField, 2);
    
    pEmailTextField = ui::EditBox::create(Size(213, 56), ui::Scale9Sprite::create("input.png"));
    pEmailTextField->setPlaceHolder("E-mail");
    pEmailTextField->setFontSize(20);
    pEmailTextField->setPlaceholderFontSize(20);
    pEmailTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    pEmailTextField->setFontColor(Color3B(0,0,0));
    pEmailTextField->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    pEmailTextField->setPosition(Vec2(VisibleRect::center().x, pUsernameTextField->getPosition().y - 56 - 15));
    addChild(pEmailTextField, 2);

    
    
    Label *pLabel = Label::createWithTTF("Sartu", "fonts/PT_Sans-Web-Regular.ttf", 18.0);
    ui::Scale9Sprite *sprite = ui::Scale9Sprite::create("botoia_normal.png");
    
    ControlButton *pSartu = ControlButton::create(pLabel, sprite);
    pSartu->addTargetWithActionForControlEvents(this, cccontrol_selector(LoginScene::loginCallback), cocos2d::extension::Control::EventType::TOUCH_UP_INSIDE);
    pSartu->setPreferredSize(Size(213, 56));
    pSartu->setPosition(Vec2(origin.x + visibleSize.width/2, pEmailTextField->getPosition().y - 56 - 15));
    addChild(pSartu, 1);

	return true;
}

void LoginScene::backButton(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    ProfilaScene *scene = ProfilaScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LoginScene::loginCallback(Ref* pSender, cocos2d::extension::Control::EventType pCCControlEvent)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    if (!LoginScene::chechUserInput()) {
        MessageBox("Erabiltzailea eta E-mail zuzena sartu", "error");
    } else {
        std::string *postData = new std::string("{\"method\":\"erabiltzaileBerria\", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":{\"izena\":\"");
        
        postData->append(pUsernameTextField->getText());
        postData->append("\", \"email\":\"");
        postData->append(pEmailTextField->getText());
        postData->append("\"}}");
        
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl(URL);
        request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/x-www-form-urlencoded");
        request->setHeaders(headers);
        request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
        request->setRequestData(postData->c_str(), strlen(postData->c_str()));
        cocos2d::network::HttpClient::getInstance()->send(request);
    }
}

void LoginScene::onHttpRequestCompleted(HttpClient* client, HttpResponse* response)
{
    
    if (!response)
        return;
    
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        log("HTTP Status Code: %ld", response->getResponseCode());
        
        std::vector<char> *buffer = response->getResponseData();
        
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
        
        MessageBox("Error", "Interneta beharrezkoa da");
        
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    
    char *uid = strdup(result->valuestring);
    
    cJSON_Delete(root);
    
    UserDefault *pUserDefault = UserDefault::getInstance();
    
    pUserDefault->setStringForKey("username", pUsernameTextField->getText());
    pUserDefault->setStringForKey("email", pEmailTextField->getText());
    pUserDefault->setStringForKey("uid", uid);
    pUserDefault->flush();
    
    MainMenuScene *scene = MainMenuScene::create();
    
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
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
	unsigned int Length = (int) strlen(email);
    
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


static Rect getRect(Node * pNode)
{
    Rect rc;
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

void KeyboardNotificationLayer::registerTouchEvents()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void KeyboardNotificationLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    if (! m_pTrackNode)
    {
        return;
    }
    
    Rect rectTracked = getRect(m_pTrackNode);
    log("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    log("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    auto children = getChildren();
    Node * node = 0;
    int count = (int) children.size();
    Point pos;
    for (int i = 0; i < count; ++i)
    {
        node = (Node*)children.at(i);
        pos = node->getPosition();
        pos.y += adjustVert - 160;
        node->setPosition(pos);
    }
}

void KeyboardNotificationLayer::keyboardDidHide(IMEKeyboardNotificationInfo& info)
{
    
    Rect rectTracked = getRect(m_pTrackNode);
    log("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
          rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    log("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    auto children = getChildren();
    Node * node = 0;
    int count = (int) children.size();
    Point pos;
    for (int i = 0; i < count; ++i)
    {
        node = (Node*)children.at(i);
        pos = node->getPosition();
        pos.y -= adjustVert + 160;
        node->setPosition(pos);
    }
}

// Layer function

bool KeyboardNotificationLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    log("++++++++++++++++++++++++++++++++++++++++++++");
    m_beginPos = pTouch->getLocation();
    return true;
}

void KeyboardNotificationLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (! m_pTrackNode)
    {
        return;
    }
    
    Point endPos = pTouch->getLocation();
    
    float delta = 5.0f;
    if (std::abs(endPos.x - m_beginPos.x) > delta
        || std::abs(endPos.y - m_beginPos.y) > delta)
    {
        // not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    Rect rect;
    Point point = convertTouchToNodeSpaceAR(pTouch);
    log("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);
    
    rect = getRect(m_pTrackNode);
    log("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
          rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    this->onClickTrackNode(rect.containsPoint(point));
    log("----------------------------------");
}
