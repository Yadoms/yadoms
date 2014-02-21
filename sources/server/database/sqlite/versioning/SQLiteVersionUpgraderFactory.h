#pragma once

#include "ISQLiteVersionUpgrade.h"

namespace server { 
   namespace database { 
      namespace sqlite { 
         namespace versioning { 


            class CSQLiteVersionUpgraderFactory
            {
            public:
               static boost::shared_ptr<ISQLiteVersionUpgrade> GetUpgrader();

            private:
               CSQLiteVersionUpgraderFactory();
               virtual ~CSQLiteVersionUpgraderFactory();
            };

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 
} //namespace server
