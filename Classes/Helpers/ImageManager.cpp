//
//  ImageManager.cpp
//  IkasGame
//
//  Created by Sergio Garcia on 29/1/15.
//
//

#include "ImageManager.h"

static std::string defaultExtension = "png";

std::string ImageManager::getImage(const std::string fileName)
{
    return ImageManager::getImage(fileName, defaultExtension);
}

std::string ImageManager::getImage(const std::string fileName, const std::string extension)
{
    std::string imagePath = FileUtils::getInstance()->fullPathForFilename(fileName + "." + extension);
    return imagePath;
}
