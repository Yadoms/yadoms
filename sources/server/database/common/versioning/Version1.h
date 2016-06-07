#pragma once

#include "IVersionUpgrade.h"
#include "database/IDatabaseRequester.h"


namespace database { namespace common { namespace versioning { 

   //
   /// \brief Database version 1.0 update manager
   //
   class CVersion1 : public IVersionUpgrade
   {
   public:
      //
      /// \brief Constructor
      //
      CVersion1();

      //
      /// \brief Destructor
      //
      virtual ~CVersion1();

      // ISQLiteVersionUpgrade implementation
      virtual void checkForUpgrade(const boost::shared_ptr<IDatabaseRequester> & pRequester, const shared::versioning::CVersion & currentVersion);
      // [END] ISQLiteVersionUpgrade implementation

   private:
      //-----------------------------------
      /// \brief     The version (1.0.0.0)
      //-----------------------------------
      shared::versioning::CVersion m_version;

      //-----------------------------------
      /// \brief     Create the database (when tables are not found)
      ///\param [in] pRequester : database requester object
      //-----------------------------------
      void CreateDatabase(const boost::shared_ptr<IDatabaseRequester> & pRequester);
   };

} //namespace versioning
} //namespace common
} //namespace database 

