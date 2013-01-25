//
//  ProfilaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 14/01/13.
//
//

#include "ProfilaScene.h"
#include "VisibleRect.h"
#include "MainMenuScene.h"
#include "SpriteButton.h"
#include "SQLHelper.h"
#include "TrivialAPI.h"
#include "MultzoaModel.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

ProfilaScene::~ProfilaScene()
{
    
}

ProfilaScene::ProfilaScene()
{
    
    CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();
    std::string uid = pUserDefault->getStringForKey("uid");
    
    pSQL = SQLHelper::sharedInstance();
    
    CCArray *pMultzoak = pSQL->queryMultzoak(-1);
    
    CCObject *item;
    int puntuazioa = 0;
    
    CCARRAY_FOREACH(pMultzoak, item)
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
    
    CCLog("%s", postData->c_str());
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(URL);
    request->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    request->setHeaders(headers);
    request->setResponseCallback(this, callfuncND_selector(ProfilaScene::onHttpRequestCompleted));
    request->setRequestData(postData->c_str(), strlen(postData->c_str()));
    CCHttpClient::getInstance()->send(request);

}

// on "init" you need to initialize your instance
bool ProfilaScene::init()
{    
  
    CCNodeLoaderLibrary *pNodeLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    pNodeLibrary->registerCCNodeLoader("ProfilaLayer", ProfilaLayerLoader::loader());
    
    CCBReader *pReader = new CCBReader(pNodeLibrary);
    
    node = (CCLayer*) pReader->readNodeGraphFromFile("ccbi/Profila.ccbi");

    CCTexture2D* t = CCTextureCache::sharedTextureCache()->addImage("go-back.png");
    SpriteButton *pBackButton = SpriteButton::create(t, this, callfuncO_selector(ProfilaScene::backButton));
    pBackButton->setAnchorPoint(ccp(0, 1));
    pBackButton->setPosition(VisibleRect::leftTop());
    node->addChild(pBackButton);
    
    
    CCSprite *pTitulo = (CCSprite*) node->getChildByTag(1);
    
    pSailkapenaLayer = CCLayer::create();
    pSailkapenaLayer->setContentSize(CCSizeMake(320, 230));
    CCPoint pSailkapenaPoint = ccp(0, pTitulo->getPosition().y - pTitulo->getContentSize().height / 2 - pSailkapenaLayer->getContentSize().height - 15);
    pSailkapenaLayer->setPosition(pSailkapenaPoint);
    
    
    pAbisua = CCLabelTTF::create("Interneta behar da sailkapena ikusteko", "fonts/PT_Sans-Web-Regular.ttf", 23);
    pAbisua->setDimensions(CCSizeMake(200, 100));
    pAbisua->setPosition(ccp(VisibleRect::getVisibleRect().size.width / 2, VisibleRect::getVisibleRect().size.height / 2 - 20));
    pAbisua->setColor(ccc3(0, 0, 0));

    node->addChild(pAbisua);
    
    node->addChild(pSailkapenaLayer);
    
    this->addChild(node);
    
	return true;
}

void ProfilaScene::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
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
        
        return;
    }
    
    pAbisua->setVisible(false);
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    
    int num = cJSON_GetArraySize(result);
    
    int padding = 4;
    CCPoint startPoint = ccp(0, 226);
    
    for (int i = 1; i <= num; i++)
    {
        cJSON *sailkapenaJSON = cJSON_GetArrayItem(result, i-1);
        
        char *sailkapena = strdup(cJSON_GetObjectItem(sailkapenaJSON, "sailkapena")->valuestring);
        char *izena = strdup(cJSON_GetObjectItem(sailkapenaJSON, "izena")->valuestring);
        char *puntuazioa = strdup(cJSON_GetObjectItem(sailkapenaJSON, "puntuazioa")->valuestring);
        
        CCLayer *pLineaLayer = CCLayerColor::create();
        pLineaLayer->setContentSize(CCSizeMake(320, 32));
        
        CCPoint p;
        ccColor4B color;
        
        if (i == num) {
            p = ccp(0, 32 + padding*4);
            color = ccc4(0, 0, 0, 255);
        } else {
            p = ccp(0, 32 + padding);
            color = ccc4(40, 56, 0, 255);
        }
        
        
        startPoint = ccpSub(startPoint, p);
        pLineaLayer->setPosition(startPoint);
        
        CCLayerColor *pNombreIzar = CCLayerColor::create(color);
        pNombreIzar->setContentSize(CCSizeMake(300, 32));
        pNombreIzar->setPosition(CCPointMake(10, 0));
        
        CCLabelTTF *pNumberLabel = CCLabelTTF::create(sailkapena, "fonts/PT_Sans-Web-Regular.ttf", 18);
        pNumberLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        pNumberLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
        pNumberLabel->setPosition(ccp(40, 16));
        pNumberLabel->setColor(ccc3(255, 255, 255));
        //pNumberLabel->setDimensions(CCSizeMake(30, 60));
        pNombreIzar->addChild(pNumberLabel);
        
        CCLabelTTF *pIzenaLabel = CCLabelTTF::create(izena, "fonts/PT_Sans-Web-Regular.ttf", 18);
        pIzenaLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        pIzenaLabel->setPosition(ccp(15 + pNumberLabel->getPosition().x + pNumberLabel->getContentSize().width / 2 + pIzenaLabel->getContentSize().width / 2, 16));
        pIzenaLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        pIzenaLabel->setColor(ccc3(255, 255, 255));
        pNombreIzar->addChild(pIzenaLabel);
        
        CCLabelTTF *pPuntuazioaLabel = CCLabelTTF::create(puntuazioa, "fonts/PT_Sans-Web-Regular.ttf", 18);
        pPuntuazioaLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        pPuntuazioaLabel->setPosition(ccp(280 - pPuntuazioaLabel->getContentSize().width / 2 - 10, 16));
        pPuntuazioaLabel->setColor(ccc3(255, 255, 255));
        pNombreIzar->addChild(pPuntuazioaLabel);
        
        /*CCSprite *pIzarrak = CCSprite::create("5-izar-sailkapena.png");
         pIzarrak->setPosition(ccp(pIzenaLabel->getPosition().x + pIzenaLabel->getContentSize().width / 2 + pIzarrak->getContentSize().width / 2 + 20, 8 + pIzarrak->getContentSize().height / 2));
         pNombreIzar->addChild(pIzarrak);*/
        
        pLineaLayer->addChild(pNombreIzar);
        
        pSailkapenaLayer->addChild(pLineaLayer);
        
    }
    
    cJSON_Delete(root);
}


void ProfilaScene::backButton(CCObject *sender, CCControlEvent controlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/button.wav")).c_str());
    MainMenuScene *scene = MainMenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void ProfilaScene::reset(CCObject *sender, CCControlEvent controlEvent)
{
    pSQL->resetGameData();
    
    MainMenuScene *scene = MainMenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}