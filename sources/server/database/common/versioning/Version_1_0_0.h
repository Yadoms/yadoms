#pragma once

#include "IVersionUpgrade.h"
#include "database/IDatabaseRequester.h"
#include <shared/currentTime/Provider.h>


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 1.0 update manager
         //
         // ReSharper disable once CppInconsistentNaming
         class CVersion_1_0_0 : public IVersionUpgrade
         {
         public:
            CVersion_1_0_0() = default;
            ~CVersion_1_0_0() override = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         protected:
            static void updateDatabaseVersion(const boost::shared_ptr<IDatabaseRequester> requester,
                                              const shared::versioning::CSemVer& newVersion,
                                              const boost::posix_time::ptime& insertDate = shared::currentTime::Provider().now());

         private:
            //-----------------------------------
            /// \brief     The version (1.0.0.0)
            //-----------------------------------
            static const shared::versioning::CSemVer Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            static void createDatabase(const boost::shared_ptr<IDatabaseRequester>& pRequester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
