//
//  MultzoaModel.h
//  TrivialJokoa
//
//  Created by iker on 10/12/12.
//
//

#ifndef __TrivialJokoa__MultzoaModel__
#define __TrivialJokoa__MultzoaModel__

#include <iostream>
#include "CCObject.h"
#include "cJSON.h"

class MultzoaModel : public cocos2d::CCObject
{
//Variables
private:
    void init(cJSON *multzoa);
public:
    MultzoaModel();
    MultzoaModel(cJSON *multzoa);
    int idMultzoa;
    int idKategoria;
    std::string privateCode;
    int zailtasuna;
    std::string izena;
        
    //Variable hauek dinamikoki kalkulatzen dira
    int kalifikazioa;
    //1 true
    int bukatuta;
    int currentGaldera;
};

#endif /* defined(__TrivialJokoa__MultzoaModel__) */
