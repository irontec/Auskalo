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


using namespace cocos2d;
using namespace std;

typedef struct {
    char privateCode[50];
    char privateToken[50];
    bool lastCode;
} PrivateCode;


class SQLHelper : public Ref
{
    //Variables
private:
    //Variables
    sqlite3 *_db;
    
    //Methods
private:
    void init();
    
    //Create methods
    void createDB();
    void createUserTables();
    void createGameDataTables();
    
protected:
    SQLHelper();
    static SQLHelper * _instance;
    
public:
    virtual ~SQLHelper();
    static SQLHelper* getInstance();
    
    //Initilize or insert
    void initUserData();
    
    void insertMultzoa(Vector<Ref*> multzoak);
    void insertKategoria(Vector<Ref*> kategoriak);
    void insertGaldera(Vector<Ref*> galderak);
    
    //    void insertKategoria(KategoriaModel* cat);
    //    void insertGaldera(GalderaModel* gal);
    //    void insertMultzoa(MultzoaModel* mul);
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
    Vector<KategoriaModel*> * queryKategoriak();
    Vector<Ref*> * queryMultzoak(int idKategoria);
    Vector<Ref*> * queryGalderak(int idMultzoa);
    
};

#endif /* defined(__TrivialJokoa__SQLHelper__) */
