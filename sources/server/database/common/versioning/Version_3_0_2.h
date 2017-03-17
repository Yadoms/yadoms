#pragma once

#include "Version_3_0_1.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 3.0.1 update manager
         //
         class CVersion_3_0_2 : public CVersion_3_0_1
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_3_0_2();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_3_0_2();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& pRequester,
                                 const shared::versioning::CVersion& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            //-----------------------------------
            /// \brief     The version
            //-----------------------------------
            static const shared::versioning::CVersion Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            static void UpdateFrom3_0_1(const boost::shared_ptr<IDatabaseRequester>& pRequester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 