//
//  ImageManager.h
//  IkasGame
//
//  Created by Sergio Garcia on 29/1/15.
//
//

#ifndef __IkasCocosGame__ImageManager__
#define __IkasCocosGame__ImageManager__

#include "cocos2d.h"
#include <string>

using namespace cocos2d;

class ImageManager
{
public:
    static std::string getImage(const std::string fileName);
    static std::string getImage(const std::string fileName, const std::string extension);
};
#endif /* defined(__IkasCocosGame__ImageManager__) */
