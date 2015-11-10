#pragma once

#include "ISQLiteVersionUpgrade.h"
#include "database/sqlite/SQLiteRequester.h"


   namespace database { 
      namespace sqlite { 
         namespace versioning { 

            //
            /// \brief SQLite version 1.0 update manager
            //
            class CSQLiteVersion1 : public ISQLiteVersionUpgrade
            {
            public:
               //
               /// \brief Constructor
               //
               CSQLiteVersion1();

               //
               /// \brief Destructor
               //
               virtual ~CSQLiteVersion1();

               // ISQLiteVersionUpgrade implementation
               virtual void checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester, const shared::versioning::CVersion & currentVersion);
               // [END] ISQLiteVersionUpgrade implementation

            private:
               //-----------------------------------
               /// \brief     The version (1.0.0.0)
               //-----------------------------------
               shared::versioning::CVersion m_version;

               //-----------------------------------
               /// \brief     Create the database (when tables are not found)
               ///\param [in] pRequester : database requester object
               //-----------------------------------
               void CreateDatabase(const boost::shared_ptr<CSQLiteRequester> & pRequester);
            };

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 

