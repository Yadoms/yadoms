#pragma once

//
//\brief Interface for web server
//
class IWebServer
{
public:
	virtual ~IWebServer()
	{
	}

   //
   //\brief Start the webserver
   //
   virtual void start() = 0;

   //
   //\brief Stop the webserver
   //
   virtual void stop() = 0;
};


