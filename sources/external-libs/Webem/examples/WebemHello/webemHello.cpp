// webemMT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../../cWebem.h"

/// An application class which says hello
class cHello
{
public:
	char * DisplayHTML()
	{
		return "<em>Hello World</em>";
	}
	/**
	Hello to the wide world, returning a wide character UTF-32 encoded string with chinese characters

	*/
	wchar_t * DisplayWWHello()
	{
		return L"Hello Wide World.  Here are some chinese characters: \x751f\x4ea7\x8bbe\x7f6e";
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize application code

	cHello hello;

	// Initialise web server.

    http::server::cWebem theServer(
		"0.0.0.0",						// address
		"1570",							// port
		".\\");							// document root

	// register application method
	// Whenever server sees <!--#webem hello -->
	// call cHello::DisplayHTML() and include the HTML returned

	theServer.RegisterIncludeCode( "hello", 
		boost::bind( 
		&cHello::DisplayHTML,	// member function
		&hello ) );			// instance of class

	// register wide character application method
 	// Whenever server sees <!--#webem wwwhello -->
	// call cHello::DisplayWWHello() and include the HTML returned, converted to UTF-8
   
	theServer.RegisterIncludeCodeW( "wwwhello", 
		boost::bind( 
		&cHello::DisplayWWHello,	// member function
		&hello ) );			// instance of class


	// Tell user where to find the cWebem GUI
	printf("%s",theServer.Splash().c_str());

	// run the server
	theServer.Run();

	return 0;
}

