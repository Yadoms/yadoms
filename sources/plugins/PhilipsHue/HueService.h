#pragma once
#include "shared/event/EventHandler.hpp"
#include "IHueService.h"
#include "HueBridgeDiscovery.h"
#include "UrlManager.h"

class CHueService : public IHueService
{
public:
   CHueService(shared::event::CEventHandler& mainEventHandler, CHueBridgeDiscovery::HueInformations &hubInformations, int evtKeyStateReceived, int evtKeyStateTimeout);
   virtual ~CHueService();

   void startReadingBridgeButtonState() override;
   void closeReadingBridgeButtonState() override;

private:
   boost::shared_ptr<boost::thread> m_readBridgeButtonStateThread;
   static std::string buildAuthorizedUsernameBody();
   static std::map<std::string, std::string> buildCommonHeaderParameters();

   void requestUsername() const;
   shared::event::CEventHandler& m_mainEventHandler;
   CHueBridgeDiscovery::HueInformations m_hubInformations;
   int m_mainEvtKeyStateReceived;
   int m_mainEvtKeyStateTimeout;

   boost::shared_ptr<CUrlManager> m_urlManager;

};
