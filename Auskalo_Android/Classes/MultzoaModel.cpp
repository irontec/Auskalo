//
//  MultzoaModel.cpp
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#include "MultzoaModel.h"


MultzoaModel::MultzoaModel()
{

}

MultzoaModel::MultzoaModel(cJSON *multzoa)
{
    init(multzoa);
}

void MultzoaModel::init(cJSON *multzoa)
{
    idMultzoa = cJSON_GetObjectItem(multzoa, "idMultzoa")->valueint;
    idKategoria = cJSON_GetObjectItem(multzoa, "idKategoria")->valueint;
    privateCode = cJSON_GetObjectItem(multzoa, "privateCode")->valuestring;
    izena = cJSON_GetObjectItem(multzoa, "izena")->valuestring;
    zailtasuna = cJSON_GetObjectItem(multzoa, "zailtasuna")->valueint;
    currentGaldera = 1;
}
