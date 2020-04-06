#pragma once
#include "Version_5_0_0.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 4.3.0 update manager
         //
         class CVersion_5_1_0 : public CVersion_5_0_0
         {
         public:
            CVersion_5_1_0() = default;
            virtual ~CVersion_5_1_0() = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CVersion& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CVersion Version;

            static void updateFrom5_0_0(const boost::shared_ptr<IDatabaseRequester>& requester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
