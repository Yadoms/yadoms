#pragma once
#include "Version_5_1_0.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database update manager
         //
         class CVersion_5_2_0 : public CVersion_5_1_0
         {
         public:
            CVersion_5_2_0() = default;
            virtual ~CVersion_5_2_0() = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CSemVer Version;

            static void updateFrom5_1_0(const boost::shared_ptr<IDatabaseRequester>& requester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
