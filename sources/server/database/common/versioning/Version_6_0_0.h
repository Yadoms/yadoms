#pragma once
#include "Version_5_2_0.h"
#include "database/IDatabaseRequester.h"

namespace database::common::versioning
{
   //
   /// \brief Database update manager
   //
   // ReSharper disable once CppInconsistentNaming
   class CVersion_6_0_0 : public CVersion_5_2_0
   {
   public:
      CVersion_6_0_0() = default;
      ~CVersion_6_0_0() override = default;

      CVersion_6_0_0(const CVersion_6_0_0&) = delete;
      CVersion_6_0_0(CVersion_6_0_0&&) = delete;
      CVersion_6_0_0& operator=(const CVersion_6_0_0&) = delete;
      CVersion_6_0_0& operator=(CVersion_6_0_0&&) = delete;

      // ISQLiteVersionUpgrade implementation
      void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                           const shared::versioning::CSemVer& currentVersion) override;
      // [END] ISQLiteVersionUpgrade implementation

   private:
      static const shared::versioning::CSemVer Version;

      // ReSharper disable once CppInconsistentNaming
      static void updateFrom5_2_0(const boost::shared_ptr<IDatabaseRequester>& requester);
   };
}
