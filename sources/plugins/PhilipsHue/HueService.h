#pragma once
#include "shared/event/EventHandler.hpp"
#include "IHueService.h"
#include "UrlManager.h"

class CHueService : public IHueService
{
public:
   CHueService(shared::event::CEventHandler& mainEventHandler,
               int evtKeyStateReceived,
               int evtKeyStateTimeout,
               boost::shared_ptr<CUrlManager>& urlManager);

   virtual ~CHueService();

   void startReadingBridgeButtonState() override;
   void closeReadingBridgeButtonState() override;
private:
   boost::shared_ptr<boost::thread> m_readBridgeButtonStateThread;
   static std::string buildAuthorizedUsernameBody();
   static std::map<std::string, std::string> buildCommonHeaderParameters();

   void requestUsername() const;
   shared::event::CEventHandler& m_mainEventHandler;
   int m_mainEvtKeyStateReceived;
   int m_mainEvtKeyStateTimeout;

   enum { kDefaultTimeoutSeconds = 30 };

   boost::shared_ptr<CUrlManager> m_urlManager;

};
