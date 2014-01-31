//
//  TrivialAPI.h
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#ifndef __TrivialJokoa__TrivialAPI__
#define __TrivialJokoa__TrivialAPI__

#include "cocos2d.h"
#include "HttpClient.h"
#include "../Helpers/SQLHelper.h"
#include "../Helpers/json/cJSON.h"
#include "../Models/KategoriaModel.h"
#include "../Models/MultzoaModel.h"
#include "../Models/GalderaModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const char * URL = "http://auskalo.net/kontrola/api";
//static const char * URL = "http://m.jakina.eu/api";

class TrivialAPI : public CCObject
{
public:
    TrivialAPI();
    virtual ~TrivialAPI();
    void updateData();
    void updatePrivateData();
    void setCallback(CCCallFunc *callBack);
    void setProgressCallback(CCCallFunc *progressCallBack);
    int getProgress();
    
private:
    SQLHelper *_sqlHelper;
    CCCallFunc *_finishCalbback;
    CCCallFunc *_progressCalbback;
    std::string _hash;
    PrivateCode *emptyPrivateCode;
    int _progress;
    
private:
    void checkHASH(PrivateCode *privateCode);
    void updateGalderak(PrivateCode *privateCode);
    void updateMultzoak(PrivateCode *privateCode);
    void updateKategoriak(PrivateCode *privateCode);
    CCHttpRequest* createRequest(PrivateCode *privateCode);
    std::string* generatePostData(std::string method, PrivateCode *privateCode);

    
    //Http Response Callback
    void onHttpRequestCompleted(CCNode *sender, void *data);
    void onPrivateHttpRequestCompleted(CCNode *sender, void *data);
                                                                         
    //Json Parser
    void parseKategoriak(std::string str);
    void parseMultzoak(std::string str);
    void parseGalderak(std::string str);
    std::string parseGeneralToken(std::string str);
};

#endif /* defined(__TrivialJokoa__TrivialAPI__) */


