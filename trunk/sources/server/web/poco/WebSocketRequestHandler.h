#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <shared/notification/NotificationCenter.h>

namespace web { namespace poco {

   class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      CWebSocketRequestHandler(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);
      virtual ~CWebSocketRequestHandler();

      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

   private:
      //--------------------------------------------------------------
      /// \brief           The notification center
      //--------------------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;
   };


} //namespace poco
} //namespace web

