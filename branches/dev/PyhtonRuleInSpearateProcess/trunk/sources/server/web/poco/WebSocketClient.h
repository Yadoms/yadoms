#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/WebSocket.h>

namespace web { namespace poco {

class CWebSocketClient : public shared::CThreadBase
{
public:
   CWebSocketClient(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, shared::event::CEventHandler& eventHandler, int eventId);
   
   virtual ~CWebSocketClient();

   int sendFrame(const void* buffer, int length, int flags = Poco::Net::WebSocket::FRAME_TEXT);
   
private:
   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

   shared::event::CEventHandler& m_eventHandler;
   int m_eventId;
   Poco::Net::WebSocket m_socket;
   Poco::Net::HTTPServerResponse& m_response;
};

} //namespace poco
} //namespace web

