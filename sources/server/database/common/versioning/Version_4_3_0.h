#pragma once
#include "Version_4_2_0.h"
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
         // ReSharper disable once CppInconsistentNaming
         class CVersion_4_3_0 : public CVersion_4_2_0
         {
         public:
            CVersion_4_3_0() = default;
            ~CVersion_4_3_0() override = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CSemVer Version;

            // ReSharper disable once CppInconsistentNaming
            static void updateFrom4_2_0(const boost::shared_ptr<IDatabaseRequester>& requester);

            static std::string convertDuration(const std::string& duration);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
