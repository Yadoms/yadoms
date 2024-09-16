#pragma once
#include "Version_4_0_1.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 4.1.0 update manager
         //
         // ReSharper disable once CppInconsistentNaming
         class CVersion_4_1_0 : public CVersion_4_0_1
         {
         public:
            CVersion_4_1_0() = default;
            ~CVersion_4_1_0() override = default;

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            //-----------------------------------
            /// \brief     The version
            //-----------------------------------
            static const shared::versioning::CSemVer Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            // ReSharper disable once CppInconsistentNaming
            static void UpdateFrom4_0_1(const boost::shared_ptr<IDatabaseRequester>& pRequester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 