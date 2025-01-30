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
         /// \brief Database version update manager
         //
         // ReSharper disable once CppInconsistentNaming
         class CVersion_5_1_0 : public CVersion_5_0_0
         {
         public:
            CVersion_5_1_0() = default;
            ~CVersion_5_1_0() override = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CSemVer Version;

            // ReSharper disable once CppInconsistentNaming
            static void updateFrom5_0_0(const boost::shared_ptr<IDatabaseRequester>& requester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
