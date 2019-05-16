#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/WebSocket.h>
#include <shared/event/EventHandler.hpp>
#include "web/ws/FrameBase.h"

namespace web
{
   namespace poco
   {
      class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
      {
      public:
         CWebSocketRequestHandler();
         virtual ~CWebSocketRequestHandler();

         void handleRequest(Poco::Net::HTTPServerRequest& request,
                            Poco::Net::HTTPServerResponse& response) override;

      private:
		  //unique ID (debug purpose only)
		  std::string m_wsIdDebug;

         // Internal events
         enum
         {
            kConnectionLost = shared::event::kUserFirstId,
            kPing,
            kPongReceived,
            kPongTimeout,
            kReceived,
            kReceivedException,
            kFirstMinute,
            kEveryMinute,
            kNewAcquisition,
            kNewAcquisitionSummary,
            kNewDevice,
            kNewLogEvent,
            kTaskProgression,
            kDeleteKeyword,
            kDeleteDevice,
            kNewKeyword,
            kBlackListDevice
         };

         static void receiverThreaded(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                               boost::shared_ptr<Poco::Net::WebSocket> webSocket);

         static bool send(boost::shared_ptr<Poco::Net::WebSocket> webSocket,
                          const ws::CFrameBase& toSend);
         static bool sendPing(boost::shared_ptr<Poco::Net::WebSocket> webSocket);
      };
   } //namespace poco
} //namespace web


