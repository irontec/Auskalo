//
//  SQLHelper.h
//  TrivialJokoa
//
//  Created by iker on 05/12/12.
//
//

#ifndef __TrivialJokoa__SQLHelper__
#define __TrivialJokoa__SQLHelper__

#include <fstream>
#include "SQLite/sqlite3.h"
#include "cocos2d.h"
#include "../Models/GalderaModel.h"
#include "../Models/KategoriaModel.h"
#include "../Models/MultzoaModel.h"


USING_NS_CC;

typedef struct {
    char privateCode[50];
    char privateToken[50];
    bool lastCode;
} PrivateCode;


class SQLHelper : public CCObject
{
//Variables
private:
    //Variables
    sqlite3 *_db;
    std::string _dbPath;
        
//Methods
private:
    void init();
        
    //Create methods
    void createDB();
    void createUserTables();
    void createGameDataTables();
        
public:
    SQLHelper();
    virtual ~SQLHelper();
    static SQLHelper* sharedInstance();
    
    //Initilize or insert
    void initUserData();
    void insertKategoria(KategoriaModel* cat);
    void insertGaldera(GalderaModel* gal);
    void insertMultzoa(MultzoaModel* mul);
    void insertPrivateCode(PrivateCode *);
        
    //Remove methods
    void resetDB();
    void resetGameData();
    void resetPrivateGameData();
    void resetLevel(MultzoaModel *multzoa);
        
    //Update methods
    void updateUserData();
    void updateErabiltzaileMultzoa(MultzoaModel* pMultzoa);
    void updatePrivateCode(PrivateCode *privateCode);
    
    //Queries
    std::vector<PrivateCode *> queryPrivateCodes();
    CCArray* queryKategoriak();
    CCArray* queryMultzoak(int idKategoria);
    CCArray* queryGalderak(int idMultzoa);

};

#endif /* defined(__TrivialJokoa__SQLHelper__) */
