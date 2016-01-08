#include "stdafx.h"
#include "FileSystemPathHelper.h"
#include <shared/FileSystemExtension.h>

static void functionLocator()
{
    
}

boost::filesystem::path CFileSystemPathHelper::getExecutingPath()
{
    return shared::CFileSystemExtension::getModulePath((void*)functionLocator);
}

