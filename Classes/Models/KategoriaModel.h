//
//  Kategoria.h
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#ifndef __TrivialJokoa__KategoriaModel__
#define __TrivialJokoa__KategoriaModel__

#include <iostream>
#include "cocos2d.h"
#include "../Helpers/json/cJSON.h"

USING_NS_CC;

class KategoriaModel : public Ref
{
private:
    void init(cJSON *kategoria);
    
public:
    KategoriaModel(cJSON *kategoria);
    KategoriaModel();
    virtual ~KategoriaModel();    
    
public:
    int idKategoria;
    std::string privateCode;
    std::string izena;
};

#endif /* defined(__TrivialJokoa__Kategoria__) */
