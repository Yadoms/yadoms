#include "stdafx.h"
#include "SQLiteVersion1.h"
#include "SQLiteVersionUpgraderFactory.h"

namespace server { 
   namespace database { 
      namespace sqlite { 
         namespace versioning { 

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

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 
} //namespace server
