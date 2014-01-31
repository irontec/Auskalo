//
//  TrivialAPI.cpp
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#include "TrivialAPI.h"


TrivialAPI::TrivialAPI()
{
    _sqlHelper = SQLHelper::sharedInstance();
}

TrivialAPI::~TrivialAPI()
{
    CCHttpClient::getInstance()->destroyInstance();
    _finishCalbback->release();
    _sqlHelper->release();
}

void TrivialAPI::updateData()
{
    //_sqlHelper->resetDB();
    emptyPrivateCode = (PrivateCode*) malloc(sizeof(PrivateCode));
    strcpy(emptyPrivateCode->privateCode, "Publikoa");
    checkHASH(emptyPrivateCode);
}

void TrivialAPI::updatePrivateData()
{
    /*std::vector<PrivateCode*> cPrivateCodes = _sqlHelper->queryPrivateCodes();
    
    if (cPrivateCodes.empty()) {
        _finishCalbback->execute();
    } else {
        for(int i = 0; i < cPrivateCodes.size(); ++i) {
            checkHASH(cPrivateCodes[i]);
        }
    }*/
}

void TrivialAPI::checkHASH(PrivateCode *privateCode)
{
    CCHttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"generalToken\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    
    request->setTag("generalToken");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

CCHttpRequest* TrivialAPI::createRequest(PrivateCode *privateCode)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(URL);
    request->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    request->setHeaders(headers);
    /*if (strcmp(privateCode->privateCode, "Publikoa"))
    {
        request->setResponseCallback(this, callfuncND_selector(TrivialAPI::onPrivateHttpRequestCompleted));
        request->setUserData(privateCode);
    } else {*/
        request->setResponseCallback(this, callfuncND_selector(TrivialAPI::onHttpRequestCompleted));
    //}
    
    return request;
}

void TrivialAPI::setCallback(CCCallFunc *callBack)
{
    _finishCalbback = callBack;
    _finishCalbback->retain();
}

void TrivialAPI::setProgressCallback(CCCallFunc *progressCallBack)
{
    _progressCalbback = progressCallBack;
    _progressCalbback->retain();
}

std::string* TrivialAPI::generatePostData(std::string method, PrivateCode *privateCode)
{    
    std::string *url = new std::string("{\"method\":");
    
    url->append(method);
    url->append(", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":{\"privateCode\":\"Publikoa\"}}");
      
    
    CCLog("POST DATA: %s", url->c_str());
    
    return url;
}


void TrivialAPI::updateKategoriak(PrivateCode *privateCode)
{    
    CCHttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"kategoriak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    
    request->setTag("kategoriak");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void TrivialAPI::updateMultzoak(PrivateCode *privateCode)
{
    CCHttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"multzoak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));

    request->setTag("multzoak");
    
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void TrivialAPI::updateGalderak(PrivateCode *privateCode)
{
    CCHttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"galderak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    
    request->setTag("galderak");
    
    CCHttpClient::getInstance()->send(request);
    request->release();
}

std::string TrivialAPI::parseGeneralToken(std::string str)
{
    CCLog("%s", str.c_str());
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    
    char *pToken = strdup(result->valuestring);
    cJSON_Delete(root);
    
    return std::string(pToken);
}

void TrivialAPI::parseKategoriak(std::string str)
{   
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *kategoriak = cJSON_GetArrayItem(result, 0);
    
    if (kategoriak != NULL) {
    
        int numKategoriak = cJSON_GetArraySize(kategoriak);
        
        for (int i = 0; i < numKategoriak; i++)
        {
            cJSON * kategoria = cJSON_GetArrayItem(kategoriak, i);
            KategoriaModel *katModel = new KategoriaModel(kategoria);
            _sqlHelper->insertKategoria(katModel);
        }
    }
    
    cJSON_Delete(root);
}

void TrivialAPI::parseGalderak(std::string str)
{   
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *galderak = cJSON_GetArrayItem(result, 0);
    
    if (galderak != NULL) {
        
        int numGalderak = cJSON_GetArraySize(galderak);
        
        for (int i = 0; i < numGalderak; i++)
        {
//            _progress = (i * 100) / numGalderak;
//            _progressCalbback->execute();
            cJSON *galdera = cJSON_GetArrayItem(galderak, i);
            GalderaModel *galModel = new GalderaModel(galdera);
            _sqlHelper->insertGaldera(galModel);
        }
        
        cJSON_Delete(root);
    }
}

int TrivialAPI::getProgress()
{
    return _progress;
}

void TrivialAPI::parseMultzoak(std::string str)
{
    cJSON *root = cJSON_Parse(str.c_str());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *multzoak = cJSON_GetArrayItem(result, 0);
    
    if (multzoak != NULL) {
        
        int numMultzoak = cJSON_GetArraySize(multzoak);
        
        for (int i = 0; i < numMultzoak; i++)
        {
            cJSON * multzoa = cJSON_GetArrayItem(multzoak, i);
            MultzoaModel *mulModel = new MultzoaModel(multzoa);
            _sqlHelper->insertMultzoa(mulModel);
        }
    }

    cJSON_Delete(root);
}


//Callback
void TrivialAPI::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    const char *responseTag;
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
        
        CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
        std::string hash = pUserDefaults->getStringForKey("hash");
        
        if (hash.length() == 0) {
            CCMessageBox("Interneta beharrezkoa da", "Error");
        } else {
             _finishCalbback->execute();
        }

    } else {
        responseTag = response->getHttpRequest()->getTag();
        int statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, responseTag);
        //CCLog(statusString);
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
        
    if (!strcmp(responseTag, "generalToken")) {
        CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();
        std::string savedHash = pUserDefault->getStringForKey("hash");
        std::string token = parseGeneralToken(str);
        if (savedHash.compare(token) != 0) {
            _hash = token;
            _sqlHelper->resetDB();
            updateKategoriak(emptyPrivateCode);
        } else {
            //updatePrivateData();
            _finishCalbback->execute();
        }
    } else if(!strcmp(responseTag, "kategoriak")) {
        parseKategoriak(str);
        updateMultzoak(emptyPrivateCode);
    } else if(!strcmp(responseTag, "multzoak")) {
        parseMultzoak(str);
        updateGalderak(emptyPrivateCode);
    } else if(!strcmp(responseTag, "galderak")) {
        parseGalderak(str);
        
        CCUserDefault *pUserDefault = CCUserDefault::sharedUserDefault();
        std::string savedHash = pUserDefault->getStringForKey("hash");
        
        if (savedHash.empty()) {
            _sqlHelper->initUserData();
        } else {
            _sqlHelper->updateUserData();
        }
        
        pUserDefault->setStringForKey("hash", _hash);
        pUserDefault->flush();
        
        //updatePrivateData();
        _finishCalbback->execute();
    }
}

void TrivialAPI::onPrivateHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    const char *responseTag;
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    PrivateCode *privateCode = (PrivateCode*)response->getHttpRequest()->getUserData();
    
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
    } else {
        responseTag = response->getHttpRequest()->getTag();
        int statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, responseTag);
        //CCLog(statusString);
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    if (!strcmp(responseTag, "generalToken")) {
        std::string token = parseGeneralToken(str);
        if (!strcmp(privateCode->privateToken, str.c_str())) {
            
            _sqlHelper->resetPrivateGameData();
            
            //free(privateCode->privateToken);
            strcpy(privateCode->privateToken, token.c_str());
            _sqlHelper->updatePrivateCode(privateCode);
            updateKategoriak(privateCode);
            
        } else {
            _finishCalbback->execute();
        }
        
    } else if(!strcmp(responseTag, "kategoriak")) {
        parseKategoriak(str);
        updateMultzoak(privateCode);
    } else if(!strcmp(responseTag, "multzoak")) {
        parseMultzoak(str);
        updateGalderak(privateCode);
    } else if(!strcmp(responseTag, "galderak")) {
        parseGalderak(str);
        if (privateCode->lastCode) {
            //MAL
            _sqlHelper->updateUserData();
            _finishCalbback->execute();
        }
    }
}