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
    _sqlHelper = SQLHelper::getInstance();
}

TrivialAPI::~TrivialAPI()
{
    HttpClient::getInstance()->destroyInstance();
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
    HttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"generalToken\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    request->setTag("generalToken");
    HttpClient::getInstance()->send(request);
    request->release();
}

HttpRequest* TrivialAPI::createRequest(PrivateCode *privateCode)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl(URL);
    request->setRequestType(HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    request->setHeaders(headers);
    request->setResponseCallback(CC_CALLBACK_2(TrivialAPI::onHttpRequestCompleted, this));
    
    return request;
}

void TrivialAPI::setCallback(CallFunc *callBack)
{
    _finishCalbback = callBack;
    _finishCalbback->retain();
}

void TrivialAPI::setProgressCallback(CallFunc *progressCallBack)
{
    _progressCalbback = progressCallBack;
    _progressCalbback->retain();
}

std::string* TrivialAPI::generatePostData(std::string method, PrivateCode *privateCode)
{
    std::string *url = new std::string("{\"method\":");
    
    url->append(method);
    url->append(", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":{\"privateCode\":\"Publikoa\"}}");
    
    log("POST DATA: %s", url->c_str());
    
    return url;
}


void TrivialAPI::updateKategoriak(PrivateCode *privateCode)
{
    HttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"kategoriak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    request->setTag("kategoriak");
    HttpClient::getInstance()->send(request);
    request->release();
}

void TrivialAPI::updateMultzoak(PrivateCode *privateCode)
{
    HttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"multzoak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    request->setTag("multzoak");
    HttpClient::getInstance()->send(request);
    request->release();
}

void TrivialAPI::updateGalderak(PrivateCode *privateCode)
{
    HttpRequest *request = createRequest(privateCode);
    
    //write the post data
    std::string *post = TrivialAPI::generatePostData("\"galderak\"", privateCode);
    request->setRequestData(post->c_str(), strlen(post->c_str()));
    request->setTag("galderak");
    HttpClient::getInstance()->send(request);
    request->release();
}

std::string TrivialAPI::parseGeneralToken(std::string str)
{
    log("%s", str.c_str());
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
        auto kategoriakModels = new Vector<Ref*>();
        for (int i = 0; i < numKategoriak; i++)
        {
            cJSON * kategoria = cJSON_GetArrayItem(kategoriak, i);
            KategoriaModel *katModel = new KategoriaModel(kategoria);
            kategoriakModels->pushBack(katModel);
        }
        _sqlHelper->insertKategoria(*kategoriakModels);
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
        auto galderakModels = new Vector<Ref*>();
        for (int i = 0; i < numGalderak; i++)
        {
            cJSON *galdera = cJSON_GetArrayItem(galderak, i);
            GalderaModel *galModel = new GalderaModel(galdera);
            galderakModels->pushBack(galModel);
        }
        _sqlHelper->insertGaldera(*galderakModels);
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
        auto multzoakModels = new Vector<Ref*>();
        for (int i = 0; i < numMultzoak; i++)
        {
            cJSON * multzoa = cJSON_GetArrayItem(multzoak, i);
            MultzoaModel *mulModel = new MultzoaModel(multzoa);
            multzoakModels->pushBack(mulModel);
        }
        _sqlHelper->insertMultzoa(*multzoakModels);
    }
    
    cJSON_Delete(root);
}


//Callback
void TrivialAPI::onHttpRequestCompleted(HttpClient* client, HttpResponse* response)
{
    const char *responseTag;
    
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
        
        UserDefault *pUserDefaults = UserDefault::getInstance();
        std::string hash = pUserDefaults->getStringForKey("hash");
        
        if (hash.length() == 0) {
            MessageBox("Interneta beharrezkoa da", "Error");
        } else {
            _finishCalbback->execute();
        }
        
    } else {
        responseTag = response->getHttpRequest()->getTag();
        int statusCode = (int) response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, responseTag);
        //log(statusString);
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str(buffer->begin(), buffer->end());
    
    if (!strcmp(responseTag, "generalToken")) {
        UserDefault *pUserDefault = UserDefault::getInstance();
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
        
        UserDefault *pUserDefault = UserDefault::getInstance();
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
