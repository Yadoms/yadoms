// webemMT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../../cWebem.h"

/// An application class which says hello to the identified user

class cHelloForm
{
string UserName;
http::server::cWebem& myWebem;

public:
	cHelloForm( http::server::cWebem& webem ) :
	  myWebem( webem )
	{
		myWebem.RegisterIncludeCode( "hello", 
		boost::bind( 
		&cHelloForm::DisplayHTML,	// member function
		this ) );			// instance of class
		myWebem.RegisterActionCode( "name", 
		boost::bind( 
		&cHelloForm::Action,	// member function
		this ) );			// instance of class
	}
	char * DisplayHTML()
	{
		static char buf[1000];
		if( UserName.length() )
		sprintf_s( buf, 999, 
			"Hello, <em>%s</em>", UserName.c_str() );
		else
			buf[0] = '\0';
		return buf;
	}
	char * Action()
	{
		UserName = myWebem.FindValue("yourname");
		return "/index.html";
	}
};

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
	printf("		cWebem Hello Form Server\n\n"
		   "		Point browser at http://%s:%s\n",
		buf,
		port );
}


int _tmain(int argc, _TCHAR* argv[])
{

	// Initialise web server.

    http::server::cWebem theServer(
		"0.0.0.0",						// address
		"1570",							// port
		".\\");							// document root

	// Initialize application code

	cHelloForm hello( theServer );

	splash( "1570");

	// run the server
	theServer.Run();

	return 0;
}

