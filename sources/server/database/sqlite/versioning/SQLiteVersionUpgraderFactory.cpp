#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "SQLiteVersionUpgraderFactory.h"


CSQLiteVersionUpgraderFactory::CSQLiteVersionUpgraderFactory() 
{
}

CSQLiteVersionUpgraderFactory::~CSQLiteVersionUpgraderFactory() 
{
}

boost::shared_ptr<ISQLiteVersionUpgrade> CSQLiteVersionUpgraderFactory::GetUpgrader() 
{ 
   //change this line when a new SQlite version is released
   return boost::shared_ptr<ISQLiteVersionUpgrade>(new CSQLiteVersion1()); 
}

