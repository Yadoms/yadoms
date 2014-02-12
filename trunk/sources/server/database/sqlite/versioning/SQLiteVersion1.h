#pragma once

#include "ISQLiteVersionUpgrade.h"
#include "database/sqlite/SQLiteRequester.h"

//
/// \brief SQLite version 1.0 update manager
//
class CSQLiteVersion1 : public ISQLiteVersionUpgrade
{
public:
   //
   /// \brief Constructor
   //
   CSQLiteVersion1();

   //
   /// \brief Destructor
   //
   virtual ~CSQLiteVersion1();
   
   // ISQLiteVersionUpgrade implementation
   virtual void checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester, const CVersion & currentVersion);
   // [END] ISQLiteVersionUpgrade implementation

private:
   //-----------------------------------
   /// \brief     Create the database (when tables are not found)
   ///\param [in] pRequester : database requester object
   //-----------------------------------
   void CreateDatabase(const boost::shared_ptr<CSQLiteRequester> & pRequester);
};