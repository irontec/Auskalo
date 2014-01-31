//
//  Galdera.h
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#ifndef __TrivialJokoa__GalderaModel__
#define __TrivialJokoa__GalderaModel__

#include <iostream>
#include "cocos2d.h"
#include "../Helpers/json/cJSON.h"

USING_NS_CC;

class GalderaModel : public CCObject
{
private:
void init(cJSON *galderaJson);
    
public:
    GalderaModel();
    GalderaModel(cJSON *galderaJson);
    
    int idGaldera;
    int idMultzoa;

    std::string privateCode;
    std::string galdera;
    std::string zuzena;
    std::string erantzuna1;
    std::string erantzuna2;
    std::string erantzuna3;
};


#endif /* defined(__TrivialJokoa__Galdera__) */
