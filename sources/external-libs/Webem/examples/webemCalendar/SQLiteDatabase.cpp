/*
  SQLiteDatabase.cpp - CSQLiteDatabase implementation, a wrapper for the SQLite database
*/

#include "StdAfx.h"
#include ".\sqlitedatabase.h"

///
///  Access a SQLite database.
///
/// \param[in] dbfname database filename, null terminated char string

CSQLiteDatabase::CSQLiteDatabase(const wchar_t * dbfname )
{
	// remember the dtabase filename
	myDBFName = dbfname;
	// open the database
	Open();
	// allocate room for pointers to 10,000 values returned from a query
//	myRows = new string[ 10000 ];
	// clear the error
	myError = 0;
}
/*****************************************************************
  CSQLiteDatabase destructor.
*/
CSQLiteDatabase::~CSQLiteDatabase(void)
{
	Close();
//	delete[] myRows;
	myRows.clear();
}
/*****************************************************************
  Open.

  This opens, creating if neccessary, a UTF-16 database

*/
void CSQLiteDatabase::Open()
{
	int rc = sqlite3_open16(myDBFName.c_str(), &db);
	if( rc ) {
		printf("Cannot open database %s",myDBFName.c_str());
		exit(1);
	}
}
/*****************************************************************
  Close.
*/
void CSQLiteDatabase::Close()
{
	sqlite3_close(db);

}
/*****************************************************************
  callback - called by sqlite for each row returned by a query

  In:
  myRows - pointer to CString array to store the results
  argc - number of columns
  argv - pointer to string array containing results
  azColName - pointer to array of column names

  Notes:
  Assumes that the column count is always constant for each query
  Will not store more than 1000 values
  RowCount must be zeroed before each query

*/
int CSQLiteDatabase::RowCount;
int CSQLiteDatabase::callback(void *myRows, int argc, char **argv, char **azColName)
{
	for(int kcol=0; kcol<argc; kcol++){
		if( ! argv[kcol] ) {
			// a "null" value - we will just return an empty string
			((vector<string> *) myRows)->push_back(string(""));
		} else {
			((vector<string> *) myRows)->push_back(string( argv[kcol] ));
		}

	}
	RowCount++;

	return 0;
}
/**
  Apply SQL query to database.

 \param[in] sQ SQL query string

 \return 
  the number of rows returned by the query.  
  -1 indicates an error
  -2 too many rows

  The data returned by the query, if any, will be in myRows
  To access the second column in the third row
  myRows[ 2 * cols + 1 ]

*/
int CSQLiteDatabase::Query( string & sQ )
{
	RowCount = 0;
	myRows.clear();
	int rc = sqlite3_exec(db, sQ.c_str(), callback, &myRows, &myError);
	if( rc!=SQLITE_OK ){
		if( rc==SQLITE_ABORT ) {
			myError = "Too many rows returned";
			return -2;
		}
		return -1;
	}
	myLastID = (int)sqlite3_last_insert_rowid( db );

	myRowCount = RowCount;
	return myRowCount;

}
/** ***************************************************************
	Enable speed optimization.

When synchronous is FULL (2), the SQLite database engine will pause at critical moments to make sure that data has actually been written to the disk surface before continuing. 
This ensures that if the operating system crashes or if there is a power failure, the database will be uncorrupted after rebooting. 
FULL synchronous is very safe, but it is also slow. When synchronous is NORMAL, the SQLite database engine will still pause at the most critical moments, 
but less often than in FULL mode. There is a very small (though non-zero) chance that a power failure at just the wrong time could corrupt the database in NORMAL mode. 
But in practice, you are more likely to suffer a catastrophic disk failure or some other unrecoverable hardware fault. 
With synchronous OFF (0), SQLite continues without pausing as soon as it has handed data off to the operating system. 
If the application running SQLite crashes, the data will be safe,
but the database might become corrupted if the operating system crashes or the computer loses power before that data has been written to the disk surface. 
On the other hand, some operations are as much as 50 or more times faster with synchronous OFF.

In SQLite version 2, the default value is NORMAL. For version 3, the default was changed to FULL.



*/
void CSQLiteDatabase::setFast()
{
	Query("PRAGMA synchronous = OFF;");
}

/**

  Submit unicode query, retrieve unicode data.

  @param[in] q  unicode query
  @return  number of rows retrieved, -1 for error

  the row data is stored in CSQLiteDatabase::myRows16

*/
int CSQLiteDatabase::Query16( wstring const & q )
{
	myError = 0;
	// submit query - just one
	void const * tail = 0;
	sqlite3_stmt * st = 0;
	if( sqlite3_prepare16_v2( db, q.c_str(), -1, &st, &tail ) != SQLITE_OK ) {
		// error in query - provide human readable msg
		int nErrMsg = 1 + (int)strlen(sqlite3_errmsg(db));
		myError = (char*)sqlite3_malloc(nErrMsg);
		if( myError ){
			memcpy((void*)myError, sqlite3_errmsg(db), nErrMsg);
		} else {
			myError = "out of memory";
		}
		return -1;
	}
	int col_count = sqlite3_column_count( st );

	// loop over rows returned
	myRowCount = 0;
	myRows16.clear();
	while( sqlite3_step( st ) == SQLITE_ROW ) {
		myRowCount++;
		// loop over columns
		for( int kcol = 0; kcol < col_count; kcol++ ) {
			if( sqlite3_column_type( st, kcol ) == SQLITE_NULL )
				myRows16.push_back( wstring(L""));
			else
				myRows16.push_back( wstring( ( wchar_t * )sqlite3_column_text16(st, kcol)) );
		}
	}

	// clean up
	sqlite3_finalize( st );
	myLastID = (int)sqlite3_last_insert_rowid( db );
	return myRowCount;
}


/**************************************************************************

//   Designed and coded by James Bremner, Raven's Point Consulting,
//   james@ravenspoint.com

  This file is under cvs version control

  If it has been released, the release name will be here: $Name: tip $

  Below is more detailed information describing the precise version

  $Header: C:\Documents and Settings\james\My Documents\code\webem\kenai/src/sqlite/SQLiteDatabase.cpp,v 380e7e273298 2008/09/09 21:28:57 james $

*/



