#pragma once
#include "Version_4_3_0.h"
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
         class CVersion_4_4_0 : public CVersion_4_3_0
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_4_4_0();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_4_4_0();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CSemVer Version;

            static void updateFrom4_3_0(const boost::shared_ptr<IDatabaseRequester>& requester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
