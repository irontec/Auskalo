//
//  Galdera.cpp
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#include "GalderaModel.h"

GalderaModel::GalderaModel()
{
}


GalderaModel::GalderaModel(cJSON *galderaJson)
{
    init(galderaJson);
}

void GalderaModel::init(cJSON *galderaJson)
{
    idGaldera = cJSON_GetObjectItem(galderaJson, "idGaldera")->valueint;
    idMultzoa = cJSON_GetObjectItem(galderaJson, "idMultzoa")->valueint;
    privateCode = cJSON_GetObjectItem(galderaJson, "privateCode")->valuestring;
    galdera = cJSON_GetObjectItem(galderaJson, "galdera")->valuestring;
    zuzena = cJSON_GetObjectItem(galderaJson, "zuzena")->valuestring;
    erantzuna1 = cJSON_GetObjectItem(galderaJson, "erantzuna1")->valuestring;
    erantzuna2 = cJSON_GetObjectItem(galderaJson, "erantzuna2")->valuestring;
    erantzuna3 = cJSON_GetObjectItem(galderaJson, "erantzuna3")->valuestring;
}