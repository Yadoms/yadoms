#include "stdafx.h"
#include "FileSystemPathHelper.h"
#include <shared/FileSystemExtension.h>

boost::filesystem::path CFileSystemPathHelper::getExecutingPath()
{
    return shared::CFileSystemExtension::getModulePath();
}

