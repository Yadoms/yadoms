#include "stdafx.h"
#include "LinuxSystemDisksList.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CLinuxSystemDisksList::CLinuxSystemDisksList(void)
{
}

CLinuxSystemDisksList::~CLinuxSystemDisksList()
{}

std::vector<std::string>& CLinuxSystemDisksList::getList()
{
    return DrivesList;
}


