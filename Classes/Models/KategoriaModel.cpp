//
//  Kategoria.cpp
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#include "KategoriaModel.h"
#include "MultzoaModel.h"


KategoriaModel::KategoriaModel(cJSON *kategoria)
{
    init(kategoria);
}


KategoriaModel::KategoriaModel()
{
}

KategoriaModel::~KategoriaModel()
{
}

void KategoriaModel::init(cJSON *kategoria)
{
    idKategoria = cJSON_GetObjectItem(kategoria, "idKategoria")->valueint;
    privateCode = cJSON_GetObjectItem(kategoria, "privateCode")->valuestring;
    izena = cJSON_GetObjectItem(kategoria, "izena")->valuestring;
}
