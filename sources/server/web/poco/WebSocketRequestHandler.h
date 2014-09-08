#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include "notifications/NewAcquisitionNotification.h"
#include <Poco/Net/WebSocket.h>

namespace web { namespace poco {

   class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

   private:
      void handleNewData(const Poco::AutoPtr<notifications::CNewAcquisitionNotification> & notification);
      void finalizeServer();
      void updateFilters(std::vector<int> & newFilters);

      Poco::SharedPtr<Poco::Net::WebSocket> m_socketServer;
     
      
      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the events queue
      //--------------------------------------------------------------
      mutable boost::recursive_timed_mutex m_mutex;


      std::vector<int>  m_acquisitionKeywordFilters;
   };


} //namespace poco
} //namespace web

