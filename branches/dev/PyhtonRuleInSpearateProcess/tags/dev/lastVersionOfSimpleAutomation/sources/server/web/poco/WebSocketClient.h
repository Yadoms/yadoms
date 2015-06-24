#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <shared/notification/NotificationObserver.h>
#include <shared/ThreadBase.h>
#include <Poco/Net/WebSocket.h>

namespace web { namespace poco {

class CWebSocketClient : public shared::CThreadBase
{
public:
   CWebSocketClient(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, boost::shared_ptr<shared::notification::CNotificationObserver> observer, const int observerEvent);
   
   virtual ~CWebSocketClient();

   int sendFrame(const void* buffer, int length, int flags = Poco::Net::WebSocket::FRAME_TEXT);
   
private:
   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

   boost::shared_ptr<shared::notification::CNotificationObserver> m_observer;
   int m_observerEvent;
   Poco::Net::WebSocket m_socket;
   Poco::Net::HTTPServerResponse& m_response;
};

} //namespace poco
} //namespace web

