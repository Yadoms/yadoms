#pragma once

#include "database/sqlite/SQLiteRequester.h"
#include "tools/Version.h"


   namespace database { 
      namespace sqlite { 
         namespace versioning { 

            //
            /// \brief Interface for database version upgrade
            //
            class ISQLiteVersionUpgrade
            {
            public:
               //
               ///\brief      Check for an upgrade of the database. If needed apply upgrade script
               ///\param [in] pRequester shared pointer to SQLite requester
               ///\param [in] currentVersion version of the actual database
               ///\throw      CSQLiteVersionException if upgrade failed
               //-------------------------------------------------------------------
               virtual void checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester, const tools::CVersion & currentVersion) =0;
            };

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 

