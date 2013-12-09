#pragma once

#include "ISQLiteVersionUpgrade.h"

class CSQLiteVersionUpgraderFactory
{
public:
   static boost::shared_ptr<ISQLiteVersionUpgrade> GetUpgrader();

private:
   CSQLiteVersionUpgraderFactory();
   virtual ~CSQLiteVersionUpgraderFactory();
};
