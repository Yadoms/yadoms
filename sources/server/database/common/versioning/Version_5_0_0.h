#pragma once
#include "Version_4_4_0.h"
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
         class CVersion_5_0_0 : public CVersion_4_4_0
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_5_0_0();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_5_0_0();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester>& requester,
                                 const shared::versioning::CSemVer& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            static const shared::versioning::CSemVer Version;

            static void updateFrom4_4_0(const boost::shared_ptr<IDatabaseRequester>& requester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
