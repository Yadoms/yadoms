#pragma once
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/HTTPServer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSigfoxHTTPServer
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CSigfoxHTTPServer(shared::event::CEventHandler& receiveDataEventHandler,
                     int receiveDataEventId,
                     const unsigned short port);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSigfoxHTTPServer();

   void start();
   void stop();

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;
};