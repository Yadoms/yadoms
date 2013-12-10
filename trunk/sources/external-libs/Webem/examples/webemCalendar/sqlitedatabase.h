/*
  SQLiteDatabase.h - CSQLiteDatabase interface, a wrapper for the SQLite database
*/
#pragma once

#include "sqlite3.h"

#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;

/// Access to SQLite databases
class CSQLiteDatabase
{
public:

/// the name of the file used to store the database on disk
	wstring myDBFName;

/// the number of rows returned by the last query on this database
	int myRowCount;				

/// the index number of the last row added
	int myLastID;		

/// the values returned by the last query
	vector<string> myRows;
	vector<wstring> myRows16;

/// if not null, points to last error message
	char * myError;

	CSQLiteDatabase(const wchar_t* dbfname );		// constructor
	~CSQLiteDatabase(void);

/// Query - apply SQL query ( null terminated character string ) to database 
	int Query( const char* szQ ) { return Query( string( szQ ) ); }

///  Query - apply SQL query ( string ) to database.
	int Query( string & sQ );

/// Query - apply SQL query ( unicode CString ) to database
	int Query16( wstring const & q );

///  Enable speed optimization ( synchronous off ).
	void setFast();

protected:

	void Open();
	void Close();

/// sqlite's pointer to the database
	sqlite3 *db;						


private:


	static int RowCount;			// the rows returned by current query on any database

	// callback function used by SQLite each time it returns one row
	static int callback(void *myRows, int argc, char **argv, char **azColName);

};
/**************************************************************************

//   Designed and coded by James Bremner, Raven's Point Consulting,
//   james@ravenspoint.com

  This file is under cvs version control

  If it has been released, the release name will be here: $Name: tip $

  Below is more detailed information describing the precise version

  $Header: C:\Documents and Settings\james\My Documents\code\webem\kenai/src/sqlite/sqlitedatabase.h,v 380e7e273298 2008/09/09 21:28:57 james $

*/

