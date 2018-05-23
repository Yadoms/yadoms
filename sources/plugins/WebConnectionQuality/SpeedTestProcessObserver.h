#pragma once
#include <shared/process/IProcessObserver.h>
#include <shared/event/EventHandler.hpp>


class CSpeedTestProcessObserver : public shared::process::IProcessObserver
{
public:
   explicit CSpeedTestProcessObserver(shared::event::CEventHandler& pluginEventHandler,
                                      int eventId);
   virtual ~CSpeedTestProcessObserver();

protected:
   // IProcessObserver Implementation
   void onStart() override;
   void onFinish(int returnCode,
                 const std::string& error) override;
   // [END] IProcessObserver Implementation

private:
   shared::event::CEventHandler& m_pluginEventHandler;
   int m_eventId;
};
