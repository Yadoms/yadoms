#pragma once

#include "database/sqlite/SQLiteRequester.h"
//
/// \brief Interface for database version upgrade
//
class ISQLiteVersionUpgrade
{
public:
   //
   ///\brief      Check for an upgrade of the database. If needed apply upgrade script
   ///\param [in] pRequester shared pointer to SQLite requester
   ///\throw      CSQLiteVersionException if upgrade failed
   //-------------------------------------------------------------------
   virtual void checkForUpgrade(const boost::shared_ptr<CSQLiteRequester> & pRequester) =0;
};
