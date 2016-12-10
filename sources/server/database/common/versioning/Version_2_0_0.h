#pragma once

#include "Version_1_0_0.h"
#include "database/IDatabaseRequester.h"


namespace database { namespace common { namespace versioning { 

   //
   /// \brief Database version 1.0 update manager
   //
   class CVersion_2_0_0 : public CVersion_1_0_0
   {
   public:
      //
      /// \brief Constructor
      //
      CVersion_2_0_0();

      //
      /// \brief Destructor
      //
      virtual ~CVersion_2_0_0();

      // ISQLiteVersionUpgrade implementation
      virtual void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester> & pRequester, const shared::versioning::CVersion & currentVersion);
      // [END] ISQLiteVersionUpgrade implementation

   private:
      //-----------------------------------
      /// \brief     The version (2.0.0)
      //-----------------------------------
      shared::versioning::CVersion m_version;

      //-----------------------------------
      /// \brief     Create the database (when tables are not found)
      ///\param [in] pRequester : database requester object
      //-----------------------------------
      void UpdateFrom1_0_0(const boost::shared_ptr<IDatabaseRequester> & pRequester) const;
   };

} //namespace versioning
} //namespace common
} //namespace database 

