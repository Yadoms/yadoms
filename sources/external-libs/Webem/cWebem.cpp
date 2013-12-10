
/**

@mainpage WEBEM

Detailed class and method documentation of the WEBEM C++ embedded web server source code.
<p>

Quit source documentation, return to <a href="http://66.199.140.183/cgi-bin/webem.cgi">WEBEM home page </a>


 Copyright (c) 20010 by James Bremner
 * All rights reserved.
 *
 * Use license: Modified from standard BSD license.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation, advertising
 * materials, Web server pages, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by James Bremner. The name "James Bremner" may not be used to
 * endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.


*/

#include "stdafx.h"
#include "cWebem.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server/reply.hpp"
#include "server/request.hpp"

namespace http {
	namespace server {

	typedef std::multimap  < std::string, std::string> webem_mmp_name_value;
	typedef std::multimap  < std::string, std::string>::iterator webem_iter_name_value;

/**

Webem constructor

@param[in] address  IP address.  In general, use  "0.0.0.0"
@param[in] port     port to listen on for browser requests e.g. "8080"
@param[in] doc_root path to folder containing html e.g. "./"

*/
cWebem::cWebem(
	   const std::string& address,
	   const std::string& port,
	   const std::string& doc_root ) :
myRequestHandler( doc_root ), myPort( port ),
myServer( address, port, myRequestHandler )
{
	myRequestHandler.setWebem( this );
}

/**

Start the server.

This does not return.

If application needs to continue, start new thread with call to this method.

*/

void cWebem::Run() { myServer.run(); }


/**

Create a link between a string ID and a function to calculate the dynamic content of the string

The function should return a pointer to a character buffer.  This should be contain only ASCII characters
( Unicode code points 1 to 127 )

@param[in] idname  string identifier
@param[in] fun pointer to function which calculates the string to be displayed

*/

void cWebem::RegisterIncludeCode( const char* idname, webem_include_function fun )
{
	myIncludes.insert( std::pair<std::string, webem_include_function >( std::string(idname), fun  ) );
}
/**

Create a link between a string ID and a function to calculate the dynamic content of the string

The function should return a pointer to wide character buffer.  This should contain a wide character UTF-16 encoded unicode string.
WEBEM will convert the string to UTF-8 encoding before sending to the browser.

@param[in] idname  string identifier
@param[in] fun pointer to function which calculates the string to be displayed

*/

void cWebem::RegisterIncludeCodeW( const char* idname, webem_include_function_w fun )
{
	myIncludes_w.insert( std::pair<std::string, webem_include_function_w >( std::string(idname), fun  ) );
}
/**

Specify link between form and application function to run when form submitted

@param[in] idname string identifier
@param[in] fun fpointer to function

*/
void cWebem::RegisterActionCode( const char* idname, webem_action_function fun )
{
	myActions.insert( std::pair<std::string, webem_action_function >( std::string(idname), fun  ) );
}

		/**

		Conversion between UTF-8 and UTF-16 strings.

		UTF-8 is used by web pages.  It is a variable byte length encoding
		of UNICODE characters which is independant of the byte order in a computer word.

		UTF-16 is the native Windows UNICODE encoding.

		The class stores two copies of the string, one in each encoding,
		so should only exist briefly while conversion is done.

		This is a wrapper for the WideCharToMultiByte and MultiByteToWideChar
		*/
		class cUTF
		{
			wchar_t * myString16;		///< string in UTF-16
			char * myString8;			///< string in UTF-6
		public:
			/// Construct from UTF-16
			cUTF( const wchar_t * ws );
			///  Construct from UTF8
			cUTF( const char * s );
			/// get UTF16 version
			const wchar_t * get16() { return myString16; }
			/// get UTF8 version
			const char * get8() { return myString8; }
			/// free buffers
			~cUTF() { free(myString8); free(myString16); }
		};

		/// Construct from UTF-16
		cUTF::cUTF( const wchar_t * ws )
		{
			// store copy of UTF16
			myString16 = (wchar_t * ) malloc( wcslen( ws ) * 2 + 2 );
			wcscpy( myString16, ws );
			// How long will the UTF-8 string be
			int len = WideCharToMultiByte(CP_UTF8, 0,
				ws, wcslen( ws ),
				NULL, NULL, NULL, NULL );
			// allocate a buffer
			myString8 = (char * ) malloc( len + 1 );
			// convert to UTF-8
			WideCharToMultiByte(CP_UTF8, 0,
				ws, wcslen( ws ),
				myString8, len, NULL, NULL);
			// null terminate
			*(myString8+len) = '\0';
		}
		///  Construct from UTF8
		cUTF::cUTF( const char * s )
		{
			myString8 = (char * ) malloc( strlen( s ) + 1 );
			strcpy( myString8, s );
			// How long will the UTF-16 string be
			int len = MultiByteToWideChar(CP_UTF8, 0,
				s, strlen( s ),
				NULL, NULL );
			// allocate a buffer
			myString16 = (wchar_t * ) malloc( len * 2 + 2 );
			// convert to UTF-16
			MultiByteToWideChar(CP_UTF8, 0,
				s, strlen( s ),
				myString16, len);
			// null terminate
			*(myString16+len) = '\0';
		}


/**

  Do not call from application code, used by server to include generated text.

  @param[in/out] reply  text to include generated

  The text is searched for "<!--#cWebemX-->".
  The X can be any string not containing "-->"

  If X has been registered with cWebem then the associated function
  is called to generate text to be inserted.


*/
void cWebem::Include( std::string& reply )
{
	int p = 0;
	while( 1 ) {
		// find next request for generated text
		p = reply.find("<!--#webem",p);
		if( p == -1 ) {
			break;
		}
		int q = reply.find("-->",p);
		if( q == -1 )
			break;;
		q += 3;

		int reply_len = reply.length();

		// code identifying text generator
		std::string code = reply.substr( p+11, q-p-15 );

		// find the function associated with this code
		std::map < std::string, webem_include_function >::iterator pf = myIncludes.find( code );
		if( pf != myIncludes.end() ) {
			// insert generated text
			reply.insert( p, pf->second() );
		} else {
			// no function found, look for a wide character fuction
			std::map < std::string, webem_include_function_w >::iterator pf = myIncludes_w.find( code );
			if( pf != myIncludes_w.end() ) {
				// function found
				// get return string and convert from UTF-16 to UTF-8
				cUTF utf( pf->second() );
				// insert generated text
				reply.insert( p, utf.get8() );
			}
		}

		// adjust pointer into text for insertion
		p = q + reply.length() - reply_len;
	}
}
/**

Do not call from application code,
used by server  to handle form submissions.

*/
void cWebem::CheckForAction( request& req )
{
	std::string uri = req.uri;
	std::string code;
	int q = 0;

	// look for webem click action request
	q = uri.find("/webem_");
	if( q != -1 ) {
		code = uri.substr(q+7);
		req.uri = uri.substr(0,q);

	} else {

		// look for cWebem form action request
		if( req.method != "POST" ) {
			q = uri.find(".webem?");
			if( q == -1 )
				return;
		} else {
			q = uri.find(".webem");
			if( q == -1 )
				return;
		}
		code = uri.substr(1,q-1);
	}

	// find function matching action code

	std::map < std::string, webem_action_function >::iterator
		pfun = myActions.find(  code );
	if( pfun == myActions.end() )
		return;

	// decode the values

	if( req.method == "POST" ) {
		uri = req.content;
		q = 0;
	} else {
		q += 7;
	}


	myNameValues.clear();
	std::string name;
	std::string value;

	int p = q;
	int flag_done = 0;
	while( ! flag_done ) {
		q = uri.find("=",p);
		name = uri.substr(p,q-p);
		p = q + 1;
		q = uri.find("&",p);
		if( q != -1 )
			value = uri.substr(p,q-p);
		else {
			value = uri.substr(p);
			flag_done = 1;
		}
		// the browser sends blanks as +
		while( 1 ) {
			int p = value.find("+");
			if( p == -1 )
				break;
			value.replace( p, 1, " " );
		}

		myNameValues.insert( std::pair< std::string,std::string > ( name, value ) );
		p = q+1;
	}

	// call the function
	req.uri = pfun->second( this );

	return;
}
/**

  Find the value of a name set by a form submit action

*/
std::string& cWebem::FindValue( const char* name )
{
	static std::string ret;
	ret = "";
	webem_iter_name_value iter = myNameValues.find( name );
	if( iter != myNameValues.end() )
		ret = iter->second;

	return ret;
}

/**

  Tell user where to find the cWebem GUI

*/
std::string& cWebem::Splash()
{
	static std::string ret;
	char buf[1000];
	DWORD bufsize = 999;
	GetComputerNameEx( ComputerNameDnsHostname,
		buf,
		&bufsize );
	char buf2[1000];
	sprintf(buf2,"		cWebem: Embedded Web Server by Raven's Point\n\n"
		   "		Point browser at http://%s:%s\n",
		buf,
		myPort.c_str() );
	ret = buf2;
	return ret;
}


void cWebemRequestHandler::handle_request( const request& req, reply& rep)
{
	// check for webem action request
	request req_modified = req;

	myWebem->CheckForAction( req_modified );

	// do normal handling
	request_handler::handle_request( req_modified, rep);

	// Find and include any special cWebem strings
	myWebem->Include( rep.content );

	// adjust content length header
	// ( Firefox ignores this, but apparently some browsers truncate display without it.
	// fix provided by http://www.codeproject.com/Members/jaeheung72 )

	rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());

	// tell browser that we are using UTF-8 encoding
	rep.headers[1].value = "text/html;charset=UTF-8";

}

	}
}

