// cWebemCalendar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../../cWebem.h"
#include "../../cWebemtable.h"

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

#include "sqlitedatabase.h"

/**

  Connection between database table of dates and cWebem table list and add controls

*/
class cTable
{
	cWebemTable myWebemTable;

	http::server::cWebem& myWebem;
public:
	cTable(CSQLiteDatabase& db,
		http::server::cWebem& webem );
	char * Display() { return myWebemTable.Display(); }
	char * AddAction( http::server::cWebem* Webem );
	char * DeleteAction( http::server::cWebem* Webem );
};
/**

  Construct dates table

  @param[in] db  the open SQLITE database

*/
cTable::cTable( CSQLiteDatabase& db,
	http::server::cWebem& webem ) :
myWebem( webem )
{
	// Register methods with webem request handler

	// Whenever server sees <!--#webem date_table -->
	// call cTable::Display() and include the HTML returned
	myWebem.RegisterIncludeCode( "date_table", 
		boost::bind( 
		&cTable::Display,	// member function
		this ) );			// instance of class

	myWebem.RegisterActionCode( "add_date",
		boost::bind( 
		&cTable::AddAction,	// member function
		this,					// instance of class
		_1 ) );	

	myWebem.RegisterActionCode( "table_dates_action",
		boost::bind( 
		&cTable::DeleteAction,	// member function
		this,					// instance of class
		_1 ) );	


	// tell cWebemTable about the dates table
	// so that it can be displayed properly

	myWebemTable.pdb        = &db;
	myWebemTable.myName     = "dates";
	myWebemTable.myColCount = 4;
	myWebemTable.myHeads    += string("Name"),string("Date"),string("Project"),string("People");

	//  If the database does not contain a dates table, create one

	if( db.Query( "SELECT * FROM dates;") == -1 )
		db.Query( string("CREATE TABLE dates ( name, date, project, people );" ));

}

/**

  Add a record to the table

  @param[in] Webem pointer to  webem 

*/
char * cTable::AddAction( http::server::cWebem* Webem )
{
	// extract field values
	string name, date, project, people;
	name    = Webem->FindValue("your_name");
	date    = Webem->FindValue("date");
	project = Webem->FindValue("project");
	people  = Webem->FindValue("people");

	char * ret = "/index.html";

	// do not add empty names
	if( ! name.length() )
		return ret;

	// do not add duplicate names
	char q[1000];
	sprintf(q,"SELECT * FROM dates WHERE name='%s';",
		name.c_str());
	if( myWebemTable.pdb->Query( q ) > 0 )
		return ret;

	// add
	sprintf(q,"INSERT INTO dates VALUES ( '%s','%s','%s', '%s' );",
		name.c_str(), date.c_str(), project.c_str(), people.c_str() );
	myWebemTable.pdb->Query( string(q));


	return ret;

}
/**

  Delete a record from the table

*/
char * cTable::DeleteAction( http::server::cWebem* Webem )
{
	string select = Webem->FindValue("select");
	
	char q[1000];
	sprintf(q,"DELETE FROM dates WHERE name='%s';",select.c_str());
	myWebemTable.pdb->Query( string(q));

	return "/index.html";
}
/**

  Tell user twhere to find the cWebem

*/
void splash( char * port )
{
	char buf[1000];
	DWORD bufsize = 999;
	GetComputerNameEx( ComputerNameDnsHostname,
		buf,
		&bufsize );
	printf("		cWebem Calendar Server\n\n"
		   "		Point browser at http://%s:%s\n",
		buf,
		port );
}

int _tmain(int argc, _TCHAR* argv[])
{
	// The server listens on port entered on command line, default 1570

	char * port = "1570";	// default port
	if( argc == 2 )
		port = argv[1];		// port on command line

	// Construct the webem embeded web server
	http::server::cWebem theWebem( "0.0.0.0", port, ".\\" );

	// Open database
	CSQLiteDatabase db( L"calendar.dat" );

	// Construct dates table in database
	cTable theDates ( db, theWebem );

	// Tell user the URL for the cWebem
	splash( port);

    // Run the server until stopped.
    theWebem.Run();

	return 0;
}

