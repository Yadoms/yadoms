#pragma once

#include <map>
#include <boost/function.hpp>
#include "server/server.hpp"

namespace http {
	namespace server {

		/**

		The link between the embedded web server and the application code


		* Copyright (c) 2008 by James Bremner
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
		class cWebem;
		typedef boost::function< char*() > webem_include_function;
		typedef boost::function< wchar_t*() > webem_include_function_w;
		typedef boost::function< char*( cWebem* ) > webem_action_function;


		/**

		The webem request handler.

		A specialization of the boost::asio request handler

		Application code should not use this class.

		*/
		class cWebemRequestHandler : public request_handler
		{
		public:
			/// Construct with a directory containing files to be served.
			cWebemRequestHandler( const std::string& doc_root ) :
			  request_handler( doc_root )
			  {}

			  void setWebem( cWebem* webem ) { myWebem = webem; }

			  /// Handle a request and produce a reply.
			  virtual void handle_request( const request& req, reply& rep);

		private:
			// Webem link to application code
			cWebem* myWebem;

		};

		/**

		The webem embedded web server.

		*/
		class cWebem
		{
		public:
			cWebem(
				const std::string& address,
				const std::string& port,
				const std::string& doc_root );

			void Run();

			void RegisterIncludeCode(
				const char* idname,
				webem_include_function fun );
			void RegisterIncludeCodeW(
				const char* idname,
				webem_include_function_w fun );
			void Include( std::string& reply );

			void RegisterActionCode(
				const char* idname,
				webem_action_function fun );
			void CheckForAction( request& req );
			std::string& FindValue( const char* name );
			std::string& Splash();

		private:
			/// store map between include codes and application functions
			std::map < std::string, webem_include_function > myIncludes;
			/// store map between include codes and application functions returning UTF-16 strings
			std::map < std::string, webem_include_function_w > myIncludes_w;
			/// store map between action codes and application functions
			std::map < std::string, webem_action_function > myActions;
			/// store name walue pairs for form submit action
			std::multimap  < std::string, std::string> myNameValues;
			/// request handler specialized to handle webem requests
			cWebemRequestHandler myRequestHandler;
			/// boost::asio web server
			server myServer;
			/// port server is listening on
			std::string myPort;


		};

	}
}


