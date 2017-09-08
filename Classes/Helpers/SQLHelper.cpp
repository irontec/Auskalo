//
//  SQLHelper.cpp
//  TrivialJokoa
//
//  Created by iker on 05/12/12.
//
//

#include "SQLHelper.h"

SQLHelper * SQLHelper::_instance = NULL;

SQLHelper * SQLHelper::getInstance()
{
    if(!_instance) _instance = new SQLHelper();
    return _instance;
    
}

SQLHelper::SQLHelper()
{
    int result;
    //Check database if exist
    auto fileUtils = FileUtils::getInstance();
    auto _dbPath = fileUtils->getWritablePath();
    _dbPath.append("TrivialJokoa.db");
    std::ifstream f(_dbPath.c_str());
    
    if(!f)
    {
        //Database not exist
        result = sqlite3_open(_dbPath.c_str(),&_db);
        createDB();
    } else {
        log("DB EXIST");
        result = sqlite3_open(_dbPath.c_str(),&_db);
    }
    
    if (result != SQLITE_OK)
        log("OPENING WRONG, %d", result);
}

SQLHelper::~SQLHelper()
{
}

void SQLHelper::createDB()
{
    createGameDataTables();
    createUserTables();
}

void SQLHelper::createGameDataTables()
{
    char* errMsg = NULL;
    int result;
    
    log("CREATING GAME TABLES");
    
    //Data tables for JSON data
    result = sqlite3_exec(_db, "CREATE TABLE Kategoria (idKategoria INTEGER, privateCode TEXT, izena TEXT)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        log("DB error with result %d Msg: %s", result, errMsg);
    
    result = sqlite3_exec(_db, "CREATE TABLE Multzoa (idMultzoa INTEGER, idKategoria INTEGER, privateCode TEXT, izena TEXT, zailtasuna INTEGER)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        log("DB error with result %d Msg: %s", result, errMsg);
    
    //result = sqlite3_exec(_db, "CREATE TABLE Galdera (idGaldera INTEGER, idMultzoa INTEGER, privateCode TEXT, galdera TEXT, zuzena TEXT, erantzuna1 TEXT, erantzuna2 TEXT, erantzuna3 TEXT)", NULL, 0, &errMsg);
    result = sqlite3_exec(_db, "CREATE TABLE Galdera (idGaldera INTEGER, idMultzoa INTEGER, privateCode TEXT, galdera TEXT, zuzena TEXT, erantzuna1 TEXT, erantzuna2 TEXT)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        log("DB error with result %d Msg: %s", result, errMsg);
    
    sqlite3_free(errMsg);
}

void SQLHelper::createUserTables()
{
    char* errMsg = NULL;
    int result;
    
    log("CREATING USER TABLES");
    
    result = sqlite3_exec(_db, "CREATE TABLE Erabiltzailea_Multzoa (idMultzoa INTEGER, bukatuta INTEGER, kalifikazioa INTEGER, currentGaldera INTEGER)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        log("DB error with result %d Msg: %s", result, errMsg);
    
    result = sqlite3_exec(_db, "CREATE TABLE Erabiltzailea_Code (privateCode TEXT, privateToken TEXT)", NULL, 0, &errMsg);
    //result = sqlite3_exec(_db, "INSERT INTO Erabiltzailea_Code VALUES (\"kerker\", \"4eb09163257e8a840e829bbb8459b8dec2c1756b\")", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        log("DB error with result %d Msg: %s", result, errMsg);
    
    sqlite3_free(errMsg);
}

void SQLHelper::resetDB()
{
    log("RESET DB");
    
    //Drop Tables
    sqlite3_exec(_db, "DROP TABLE IF EXISTS Kategoria", NULL, 0, NULL);
    sqlite3_exec(_db, "DROP TABLE IF EXISTS Multzoa", NULL, 0, NULL);
    sqlite3_exec(_db, "DROP TABLE IF EXISTS Galdera", NULL, 0, NULL);
    
    
    //Create tables
    createDB();
}

void SQLHelper::resetGameData()
{
    log("RESET GAME DATA");
    
    //(idMultzoa INTEGER, bukatuta INTEGER, kalifikazioa INTEGER, currentGaldera INTEGER)"
    
    sqlite3_exec(_db,"UPDATE Erabiltzailea_multzoa SET bukatuta=0, kalifikazioa=0, currentGaldera=1", NULL, 0, NULL);
    
}

void SQLHelper::resetPrivateGameData()
{
    log("RESET PRIVATE GAME DATA");
    
    //Drop Tables
    sqlite3_exec(_db, "DELETE FROM Kategoria WHERE privateCode != 0", NULL, 0, NULL);
    sqlite3_exec(_db, "DDELETE FROM Multzoa WHERE privateCode != 0", NULL, 0, NULL);
    sqlite3_exec(_db, "DELETE FROM Galdera WHERE privateCode != 0", NULL, 0, NULL);
    
}

void SQLHelper::resetLevel(MultzoaModel *multzoa)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
    
    sqlite3_prepare_v2(_db,"UPDATE Erabiltzailea_multzoa SET bukatuta=0, kalifikazioa=0, currentGaldera=1 WEHRE idMultzoa = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, ++colIndex, multzoa->idMultzoa);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/*
 *  TODO: Borrar las preguntas que ya no existen
 */

void SQLHelper::updateUserData()
{
    sqlite3_stmt *stmt;
    
    //Update Erabiltzaile_Multzoa
    sqlite3_prepare_v2(_db, "SELECT * FROM Multzoa", -1, &stmt, NULL);
    
    auto pTotalMulArray = new Vector<Ref*>();
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        MultzoaModel *pMultzo = new MultzoaModel();
        pMultzo->idMultzoa = sqlite3_column_int(stmt, 0);
        /*pMultzo->izena = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
         pMultzo->zailtasuna = sqlite3_column_int(stmt, 2);*/
        pTotalMulArray->pushBack(pMultzo);
    }
    
    for (auto item: *pTotalMulArray)
    {
        MultzoaModel *pTotalMultzoa = (MultzoaModel*) item;
        
        sqlite3_prepare_v2(_db, "SELECT COUNT(*) FROM Erabiltzailea_Multzoa WHERE idMultzoa = ?", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, pTotalMultzoa->idMultzoa);
        
        sqlite3_step(stmt);
        
        int num = sqlite3_column_int(stmt, 0);
        
        //Ez bada existitzen sortu multzoa 0 baloreekin
        if(num == 0) {
            sqlite3_prepare_v2(_db,"INSERT INTO Erabiltzailea_Multzoa VALUES (?,?,?,?)", -1, &stmt, NULL);
            sqlite3_bind_int(stmt, 1, pTotalMultzoa->idMultzoa);
            sqlite3_bind_int(stmt, 2, 0);
            sqlite3_bind_int(stmt, 3, 0);
            sqlite3_bind_int(stmt, 4, 1);
            
            if (sqlite3_step(stmt) != SQLITE_DONE)
                log("ERROR INSERTING ROW: idMultzoa: %i IN Erabiltzailea_Multzoa", pTotalMultzoa->idMultzoa);
        }
    }
    
    sqlite3_finalize(stmt);
    
}

void SQLHelper::updatePrivateCode(PrivateCode *privateCode)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
    
    sqlite3_prepare_v2(_db,"UPDATE Erabiltzailea_Code SET privateToken=? WHERE privateCode = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, ++colIndex, privateCode->privateToken, -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, privateCode->privateCode, -1, NULL);
    
    if (sqlite3_step(stmt) != SQLITE_DONE)
        log("ERROR INSERTING ROW: code: %s", privateCode->privateCode);
    
    sqlite3_finalize(stmt);
}

void SQLHelper::initUserData()
{
    sqlite3_stmt *stmt;
    
    //Initialize Erabiltzaile_Multzoa
    sqlite3_prepare_v2(_db, "SELECT * FROM Multzoa", -1, &stmt, NULL);
    
    auto pTotalMulArray = new Vector<Ref*>();
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        MultzoaModel *pMultzo = new MultzoaModel();
        pMultzo->idMultzoa = sqlite3_column_int(stmt, 0);
        /*pMultzo->izena = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
         pMultzo->zailtasuna = sqlite3_column_int(stmt, 2);*/
        pTotalMulArray->pushBack(pMultzo);
    }
    
    for (auto item: *pTotalMulArray)
    {
        MultzoaModel *pTotalMultzoa = (MultzoaModel*) item;
        
        sqlite3_prepare_v2(_db,"INSERT INTO Erabiltzailea_Multzoa VALUES (?,?,?,?)", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, pTotalMultzoa->idMultzoa);
        sqlite3_bind_int(stmt, 2, 0);
        sqlite3_bind_int(stmt, 3, 0);
        sqlite3_bind_int(stmt, 4, 1);
        
        if (sqlite3_step(stmt) != SQLITE_DONE)
            log("ERROR INSERTING ROW: idMultzoa: %i IN Erabiltzailea_Multzoa" , pTotalMultzoa->idMultzoa);
    }
    
    sqlite3_finalize(stmt);
}

void SQLHelper::insertKategoria(Vector<Ref*> kategoriak)
{
    clock_t cStartClock;
    cStartClock = clock();
    sqlite3_stmt *stmt;
    
    sqlite3_prepare_v2(_db,"INSERT INTO Kategoria VALUES (?,?,?)", -1, &stmt, NULL);
    char * sErrMsg = 0;
    sqlite3_exec(_db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    KategoriaModel *pKategoria;
    for (int i = 0; i < kategoriak.size(); i++) {
        pKategoria = (KategoriaModel*) kategoriak.at(i);
        sqlite3_bind_int(stmt, 1, pKategoria->idKategoria);
        sqlite3_bind_text(stmt, 2, pKategoria->privateCode.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, pKategoria->izena.c_str(), -1, SQLITE_TRANSIENT);
        int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            log("ERROR SQLITE %d INSERTING ROW: idGaldera: %i, Galdera %s ", result, pKategoria->idKategoria, pKategoria->izena.c_str());
        }
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    log("Insert messages: %s", sErrMsg);
    printf("KATEGORIA - Imported %zd records in %4.2f seconds\n",kategoriak.size(), (clock() - cStartClock) / (double)CLOCKS_PER_SEC);
    sqlite3_finalize(stmt);
    
}

void SQLHelper::insertGaldera(Vector<Ref*> galderak)
{
    clock_t cStartClock;
    cStartClock = clock();
    sqlite3_stmt *stmt;
    char * sErrMsg = 0;
    
    sqlite3_prepare_v2(_db,"INSERT INTO Galdera VALUES (?,?,?,?,?,?,?)", -1, &stmt, NULL);
    
    sqlite3_exec(_db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    GalderaModel* pGaldera;
    for (int i = 0; i < galderak.size(); i++) {
        pGaldera = (GalderaModel*) galderak.at(i);
        sqlite3_bind_int(stmt, 1, pGaldera->idGaldera);
        sqlite3_bind_int(stmt, 2, pGaldera->idMultzoa);
        sqlite3_bind_text(stmt, 3, pGaldera->privateCode.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, pGaldera->galdera.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, pGaldera->zuzena.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, pGaldera->erantzuna1.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, pGaldera->erantzuna2.c_str(), -1, SQLITE_TRANSIENT);
        
        int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            log("ERROR SQLITE %d INSERTING ROW: idGaldera: %i, Galdera %s ", result, pGaldera->idGaldera, pGaldera->galdera.c_str());
        }
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    log("Insert messages: %s", sErrMsg);
    printf("GALDERAK - Imported %zd records in %4.2f seconds\n",galderak.size(), (clock() - cStartClock) / (double)CLOCKS_PER_SEC);
    sqlite3_finalize(stmt);
}

void SQLHelper::insertMultzoa(Vector<Ref*> multzoak)
{
    clock_t cStartClock;
    cStartClock = clock();
    sqlite3_stmt *stmt;
    
    sqlite3_prepare_v2(_db,"INSERT INTO Multzoa VALUES (?,?,?,?,?)", -1, &stmt, NULL);
    char * sErrMsg = 0;
    sqlite3_exec(_db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    MultzoaModel* pMultzoa;
    for (int i = 0; i < multzoak.size(); i++) {
        pMultzoa = (MultzoaModel*) multzoak.at(i);
        sqlite3_bind_int(stmt, 1, pMultzoa->idMultzoa);
        sqlite3_bind_int(stmt, 2, pMultzoa->idKategoria);
        sqlite3_bind_text(stmt, 3, pMultzoa->privateCode.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, pMultzoa->izena.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, pMultzoa->zailtasuna);
        
        int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            log("ERROR SQLITE %d INSERTING ROW: idGaldera: %i, Galdera %s ", result, pMultzoa->idMultzoa, pMultzoa->izena.c_str());
        }
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    log("Insert messages: %s", sErrMsg);
    printf("GALDERAK - Imported %zd records in %4.2f seconds\n", multzoak.size(), (clock() - cStartClock) / (double)CLOCKS_PER_SEC);
    sqlite3_finalize(stmt);
}

void SQLHelper::updateErabiltzaileMultzoa(MultzoaModel* pMultzoa)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
    
    sqlite3_prepare_v2(_db,"UPDATE Erabiltzailea_Multzoa SET bukatuta = ?, kalifikazioa = ?, currentGaldera = ? WHERE idMultzoa = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, ++colIndex, pMultzoa->bukatuta);
    sqlite3_bind_int(stmt, ++colIndex, pMultzoa->kalifikazioa);
    sqlite3_bind_int(stmt, ++colIndex, pMultzoa->currentGaldera);
    sqlite3_bind_int(stmt, ++colIndex, pMultzoa->idMultzoa);
    
    if (sqlite3_step(stmt) != SQLITE_DONE)
        log("ERROR UPDATING ROW: idMultzoa: %i", pMultzoa->idMultzoa);
    
    sqlite3_finalize(stmt);
}

void SQLHelper::insertPrivateCode(PrivateCode *privateCode)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
    
    sqlite3_prepare_v2(_db,"INSERT INTO Erabiltzailea_Code VALUES (?,?)", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, ++colIndex, privateCode->privateCode, -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, privateCode->privateToken, -1, NULL);
    
    if (sqlite3_step(stmt) != SQLITE_DONE)
        log("ERROR INSERTING ROW: code: %s", privateCode->privateCode);
    
    sqlite3_finalize(stmt);
}


std::vector<PrivateCode *> SQLHelper::queryPrivateCodes()
{
    sqlite3_stmt *stmt;
    
    //Initialize Erabiltzaile_Multzoa
    sqlite3_prepare_v2(_db, "SELECT * FROM Erabiltzailea_Code", -1, &stmt, NULL);
    
    std::vector<PrivateCode *> vector;
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        //char *privateCode = strdup((const char*)sqlite3_column_text(stmt, 0));
        //char *privatetoken;// = strdup((const char*)sqlite3_column_text(stmt, 1));
        
        PrivateCode *pc = (PrivateCode*) malloc(sizeof(PrivateCode));
        strcpy(pc->privateCode, strdup((const char*)sqlite3_column_text(stmt, 0)));
        strcpy(pc->privateToken, strdup((const char*)sqlite3_column_text(stmt, 1)));
        pc->lastCode = false;
        vector.push_back(pc);
    }
    
    ((PrivateCode*)vector.back())->lastCode = true;
    
    sqlite3_finalize(stmt);
    
    return vector;
}

Vector<KategoriaModel*> * SQLHelper::queryKategoriak()
{
    auto fileUtils = FileUtils::getInstance();
    auto tmp = fileUtils->getWritablePath();
    tmp.append("TrivialJokoa.db");
    sqlite3_open(tmp.c_str(),&_db);
    //    _dbPath = fileUtils->getWritablePath();
    //    _dbPath.append("TrivialJokoa.db");
    //    sqlite3_open(_dbPath.c_str(),&_db);
    
    sqlite3_stmt *stmt;
    
    sqlite3_prepare_v2(_db, "SELECT * FROM Kategoria", -1, &stmt, NULL);
    
    auto pKategoriak = new Vector<KategoriaModel*>();
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        KategoriaModel *pKategoria = new KategoriaModel();
        pKategoria->idKategoria = sqlite3_column_int(stmt, 0);
        pKategoria->privateCode = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        pKategoria->izena = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        pKategoriak->pushBack(pKategoria);
    }
    
    sqlite3_finalize(stmt);
    
    return pKategoriak;
}

Vector<Ref*> * SQLHelper::queryMultzoak(int idKategoria)
{
    
    auto fileUtils = FileUtils::getInstance();
    auto _dbPath = fileUtils->getWritablePath();
    _dbPath.append("TrivialJokoa.db");
    sqlite3_open(_dbPath.c_str(),&_db);
    
    sqlite3_stmt *stmt;
    int nResult;
    if (idKategoria == -1) {
        nResult = sqlite3_prepare_v2(_db, "SELECT * FROM Multzoa INNER JOIN Erabiltzailea_Multzoa ON Erabiltzailea_Multzoa.idMultzoa = Multzoa.idMultzoa GROUP BY idMultzoa ORDER BY Multzoa.zailtasuna ASC", -1, &stmt, NULL);
    } else {
        nResult = sqlite3_prepare_v2(_db,
                                     "SELECT * FROM Multzoa INNER JOIN Erabiltzailea_Multzoa ON Erabiltzailea_Multzoa.idMultzoa = Multzoa.idMultzoa  WHERE idKategoria = ? GROUP BY idMultzoa ORDER BY Multzoa.zailtasuna ASC", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, idKategoria);
    }
    
    auto pMultzoak = new Vector<Ref*>();
    
    if ( nResult == SQLITE_OK)
    {
        
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            MultzoaModel *pMultzoa = new MultzoaModel();
            pMultzoa->idMultzoa = sqlite3_column_int(stmt, 0);
            pMultzoa->idKategoria = sqlite3_column_int(stmt, 1);
            pMultzoa->privateCode = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            pMultzoa->izena = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            pMultzoa->zailtasuna = sqlite3_column_int(stmt, 4);
            pMultzoa->bukatuta = sqlite3_column_int(stmt, 6);
            pMultzoa->kalifikazioa = sqlite3_column_int(stmt, 7);
            pMultzoa->currentGaldera = sqlite3_column_int(stmt, 8);
            pMultzoak->pushBack(pMultzoa);
        }
    }
    
    sqlite3_finalize(stmt);
    
    return pMultzoak;
}

Vector<Ref*> * SQLHelper::queryGalderak(int idMultzoa)
{
    
    auto fileUtils = FileUtils::getInstance();
    auto _dbPath = fileUtils->getWritablePath();
    _dbPath.append("TrivialJokoa.db");
    sqlite3_open(_dbPath.c_str(),&_db);
    
    sqlite3_stmt *stmt;
    
    if (idMultzoa == -1) {
        sqlite3_prepare_v2(_db, "SELECT * FROM Galdera", -1, &stmt, NULL);
    } else {
        sqlite3_prepare_v2(_db, "SELECT * FROM Galdera WHERE idMultzoa = ?", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, idMultzoa);
    }
    
    auto pGalderak = new Vector<Ref*>();
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        int colIndex = 0;
        GalderaModel *pGaldera = new GalderaModel();
        pGaldera->idGaldera = sqlite3_column_int(stmt, colIndex++);
        pGaldera->idMultzoa = sqlite3_column_int(stmt, colIndex++);
        pGaldera->privateCode = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        pGaldera->galdera = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        pGaldera->zuzena = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        pGaldera->erantzuna1 = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        pGaldera->erantzuna2 = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        //pGaldera->erantzuna3 = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex++)));
        pGalderak->pushBack(pGaldera);
    }
    
    sqlite3_finalize(stmt);
    
    return pGalderak;
}
