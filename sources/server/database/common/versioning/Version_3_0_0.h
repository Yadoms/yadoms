#pragma once

#include "Version_2_0_0.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 3.0 update manager
         //
         class CVersion_3_0_0 : public CVersion_2_0_0
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_3_0_0();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_3_0_0();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester, const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            //-----------------------------------
            /// \brief     The version (3.0.0)
            //-----------------------------------
            static const shared::versioning::CSemVer Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            void UpdateFrom2_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester) const;
         };
      } //namespace versioning
   } //namespace common
} //namespace database 


