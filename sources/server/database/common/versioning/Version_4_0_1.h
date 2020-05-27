#pragma once

#include "Version_4_0_0.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 4.0.1 update manager
         //
         class CVersion_4_0_1 : public CVersion_4_0_0
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_4_0_1();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_4_0_1();

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
            void UpdateFrom4_0_0(const boost::shared_ptr<IDatabaseRequester>& pRequester) const;
         };
      } //namespace versioning
   } //namespace common
} //namespace database 