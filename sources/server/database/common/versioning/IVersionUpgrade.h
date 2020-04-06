#pragma once

#include "database/IDatabaseRequester.h"
#include <shared/versioning/Version.h>


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Interface for database version upgrade
         //
         class IVersionUpgrade
         {
         public:
            //-------------------------------------------------------------------
            ///\brief  Destructor
            //-------------------------------------------------------------------
            virtual ~IVersionUpgrade()
            {
            }

            //-------------------------------------------------------------------
            ///\brief      Check for an upgrade of the database. If needed apply upgrade script
            ///\param [in] pRequester shared pointer to SQLite requester
            ///\param [in] currentVersion version of the actual database
            ///\throw      CSQLiteVersionException if upgrade failed
            //-------------------------------------------------------------------
            virtual void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                         const shared::versioning::CVersion& currentVersion) =0;
         };
      } //namespace versioning
   } //namespace common
} //namespace database 


