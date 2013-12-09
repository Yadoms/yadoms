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
   virtual void checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester);
   // [END] ISQLiteVersionUpgrade implementation
};