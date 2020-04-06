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
         class CVersion_1_0_0 : public IVersionUpgrade
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_1_0_0();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_1_0_0();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                 const shared::versioning::CVersion& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         protected:
            static void updateDatabaseVersion(const boost::shared_ptr<IDatabaseRequester> requester,
                                              const shared::versioning::CVersion& newVersion,
                                              const boost::posix_time::ptime& insertDate = shared::currentTime::Provider().now());

         private:
            //-----------------------------------
            /// \brief     The version (1.0.0.0)
            //-----------------------------------
            static const shared::versioning::CVersion Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            static void CreateDatabase(const boost::shared_ptr<IDatabaseRequester>& pRequester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 


