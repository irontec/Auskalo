//
//  BukatutaScene.cpp
//  TrivialJokoa
//
//  Created by Iker Mendilibar Fernandez on 11/01/13.
//
//

#include "BukatutaScene.h"
#include "BukatutaLayer.h"
#include "CCNodeLoaderLibrary.h"

BukatutaScene::~BukatutaScene()
{
    
}

BukatutaScene::BukatutaScene()
{
    
}


// on "init" you need to initialize your instance
bool BukatutaScene::init()
{
    CCNodeLoaderLibrary *pNodeLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    pNodeLibrary->registerCCNodeLoader("BukatutaLayer", BukatutaLayerLoader::loader());
    
    CCBReader *pReader = new CCBReader(pNodeLibrary);
    
    BukatutaLayer *node = (BukatutaLayer*) pReader->readNodeGraphFromFile("ccbi/Bukatuta.ccbi");
    
    node->multzoa = this->multzoa;
    
    this->addChild(node);
    
	return true;
}
