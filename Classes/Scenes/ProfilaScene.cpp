//
//  ProfilaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 14/01/13.
//
//

#include "../Scenes/ProfilaScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Scenes/MainMenuScene.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/SQLHelper.h"
#include "../Helpers/TrivialAPI.h"
#include "../Models/MultzoaModel.h"
#include "../Scenes/LoginScene.h"
#include "SimpleAudioEngine.h"
#include "../Helpers/ImageManager.h"


using namespace CocosDenshion;

ProfilaScene::~ProfilaScene()
{
    
}

ProfilaScene::ProfilaScene()
{
    
    UserDefault *pUserDefault = UserDefault::getInstance();
    std::string uid = pUserDefault->getStringForKey("uid");
    
    pSQL = SQLHelper::getInstance();
    
    Vector<Ref*> pMultzoak = *pSQL->queryMultzoak(-1);
    
    int puntuazioa = 0;
    
    for (auto item: pMultzoak)
    {
        MultzoaModel *multzoa = (MultzoaModel*) item;
        puntuazioa += multzoa->kalifikazioa;
    }
    
    std::string *postData = new std::string("{\"method\":\"puntuazioa\", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":{\"uid\":\"");
    
    postData->append(uid);
    postData->append("\", \"puntuazioa\":\"");
    
    char punt[10];
    sprintf(punt, "%d", puntuazioa);
    postData->append(punt);
    postData->append("\"}}");
    
    log("%s", postData->c_str());
    
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl(URL);
    request->setRequestType(HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    request->setHeaders(headers);
    request->setResponseCallback(CC_CALLBACK_2(ProfilaScene::onHttpRequestCompleted, this));
    request->setRequestData(postData->c_str(), strlen(postData->c_str()));
    cocos2d::network::HttpClient::getInstance()->send(request);

}

// on "init" you need to initialize your instance
bool ProfilaScene::init()
{    
  
    layer = LayerColor::create();
    layer->initWithColor(Color4B(255, 255, 255, 255), VisibleRect::right().x , VisibleRect::top().y);
    layer->setPosition(VisibleRect::center());
    layer->setAnchorPoint(Vec2(0.5f, 0.5f));
    layer->setIgnoreAnchorPointForPosition(false);
    addChild(layer, 0);
    
    Sprite *fondo = Sprite::create("fondo.png");
    fondo->setPosition(VisibleRect::center());
    
    addChild(fondo);

    auto pBackButton = SpriteButton::create(ImageManager::getImage("go-back"), 1.0f, CC_CALLBACK_1(ProfilaScene::backButton, this));
    pBackButton->setAnchorPoint(Vec2(0, 1));
    pBackButton->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y));
    addChild(pBackButton);
    
    Sprite *pTitulo = Sprite::create("sailkapena.png");
    pTitulo->setPosition(Vec2(VisibleRect::center().x, VisibleRect::top().y - (pTitulo->getContentSize().height - 20)));
    addChild(pTitulo);
    
    pSailkapenaLayer = Layer::create();
    pSailkapenaLayer->setContentSize(Size(320, 230));
    Point pSailkapenaPoint = Vec2(0, pTitulo->getPosition().y - pTitulo->getContentSize().height / 2 - pSailkapenaLayer->getContentSize().height - 15);
    pSailkapenaLayer->setPosition(pSailkapenaPoint);
    
    pAbisua = Label::createWithTTF("Kargatzen...", "fonts/PT_Sans-Web-Regular.ttf", 23);
    pAbisua->setDimensions(200, 100);
    pAbisua->setPosition(Vec2(VisibleRect::getVisibleRect().size.width / 2, VisibleRect::getVisibleRect().size.height / 2 - 20));
    pAbisua->setColor(Color3B(0, 0, 0));
    
    addChild(pAbisua);
    
    addChild(pSailkapenaLayer);
    
	return true;
}

void ProfilaScene::onHttpRequestCompleted(HttpClient* client, HttpResponse* response)
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
        
        return;
    }
    
    pAbisua->setVisible(false);
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    log("%s", str.c_str());
    
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    
    int num = cJSON_GetArraySize(result);
    
    bool myResult = false;
    bool showButton = false;
    
    UserDefault *pUserDefault = UserDefault::getInstance();
    std::string uid = pUserDefault->getStringForKey("uid");
    
    if (uid == "") {
        showButton = true;
    } else {
        myResult = true;
    }
    
    int padding = 4;
    Point startPoint = Vec2(0, 226);
    
    for (int i = 1; i <= num; i++)
    {
        cJSON *sailkapenaJSON = cJSON_GetArrayItem(result, i-1);
        
        char *sailkapena = strdup(cJSON_GetObjectItem(sailkapenaJSON, "sailkapena")->valuestring);
        char *izena = strdup(cJSON_GetObjectItem(sailkapenaJSON, "izena")->valuestring);
        char *puntuazioa = strdup(cJSON_GetObjectItem(sailkapenaJSON, "puntuazioa")->valuestring);
        
        Layer *pLineaLayer = LayerColor::create();
        pLineaLayer->setContentSize(Size(320, 32));
        
        Point p;
        Color4B color;
        
        if (i == num && myResult) {
            p = Vec2(0, 32 + padding*4);
            color = Color4B(0, 0, 0, 255);
        } else {
            p = Vec2(0, 32 + padding);
            color = Color4B(40, 56, 0, 255);
        }
        
        
        startPoint = Vec2(startPoint - p);
        pLineaLayer->setPosition(startPoint);
        
        LayerColor *pNombreIzar = LayerColor::create(color);
        pNombreIzar->setContentSize(Size(VisibleRect::getVisibleRect().size.width - 20, 32));
        pNombreIzar->setPosition(Vec2(VisibleRect::left().x + 10, 0));
        
        Label *pNumberLabel = Label::createWithTTF(sailkapena, "fonts/PT_Sans-Web-Regular.ttf", 12);
        pNumberLabel->setVerticalAlignment(TextVAlignment::CENTER);
        pNumberLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        pNumberLabel->setPosition(Vec2(10, 16));
        pNumberLabel->setColor(Color3B(255, 255, 255));
        pNumberLabel->setDimensions(30, 60);
        pNombreIzar->addChild(pNumberLabel);
        
        Label *pIzenaLabel = Label::createWithTTF(izena, "fonts/PT_Sans-Web-Regular.ttf", 12);
        pIzenaLabel->setVerticalAlignment(TextVAlignment::CENTER);
        pIzenaLabel->setPosition(Vec2(15 + pNumberLabel->getPosition().x + pNumberLabel->getContentSize().width / 2 + pIzenaLabel->getContentSize().width / 2, 16));
        pIzenaLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        pIzenaLabel->setColor(Color3B(255, 255, 255));
        pNombreIzar->addChild(pIzenaLabel);
        
        Label *pPuntuazioaLabel = Label::createWithTTF(puntuazioa, "fonts/PT_Sans-Web-Regular.ttf", 12);
        pPuntuazioaLabel->setVerticalAlignment(TextVAlignment::CENTER);
        pPuntuazioaLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
        pPuntuazioaLabel->setPosition(Vec2(pNombreIzar->getContentSize().width - pPuntuazioaLabel->getContentSize().width/2 - 10, 16));
        pPuntuazioaLabel->setColor(Color3B(255, 255, 255));
        pNombreIzar->addChild(pPuntuazioaLabel);
        
        pLineaLayer->addChild(pNombreIzar);
        
        pSailkapenaLayer->addChild(pLineaLayer);
        
    }
    
    cJSON_Delete(root);
    
    if(showButton)
    {
        Label *pLabel = Label::createWithTTF("Neurea bidali", "fonts/PT_Sans-Web-Regular.ttf", 17.0);
        ui::Scale9Sprite *sprite = ui::Scale9Sprite::create("botoia_normal.png");
        
        ControlButton *pSartu = ControlButton::create(pLabel, sprite);
        pSartu->addTargetWithActionForControlEvents(this, cccontrol_selector(ProfilaScene::loginCallback), Control::EventType::TOUCH_UP_INSIDE);
        pSartu->setPreferredSize(Size(213, 56));
        Point sailkapenaPoint = pSailkapenaLayer->getPosition();
        Size sailkapenaSize = pSailkapenaLayer->getContentSize();
        pSartu->setPosition(Vec2(VisibleRect::center().x, sailkapenaPoint.y));
        addChild(pSartu, 1);
    }
}

void ProfilaScene::loginCallback(Ref* pSender, cocos2d::extension::Control::EventType pCCControlEvent)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    
    LoginScene *scene = LoginScene::create();
    
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}


void ProfilaScene::backButton(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename("sounds/button.wav")).c_str());
    MainMenuScene *scene = MainMenuScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void ProfilaScene::reset(Ref* sender)
{
    pSQL->resetGameData();
    
    MainMenuScene *scene = MainMenuScene::create();
    Director *pDirector = Director::getInstance();
    TransitionFade *tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
