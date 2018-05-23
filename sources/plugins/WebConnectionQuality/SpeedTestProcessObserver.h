#pragma once
#include <shared/process/IProcessObserver.h>
#include <shared/event/EventHandler.hpp>
#include "SpeedTestProcessLogger.h"


class CSpeedTestProcessObserver : public shared::process::IProcessObserver
{
public:
   explicit CSpeedTestProcessObserver(shared::event::CEventHandler& pluginEventHandler,
                                      int eventId,
                                      boost::shared_ptr<CSpeedTestProcessLogger> speedTestProcessLoggerlogger);
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
   boost::shared_ptr<CSpeedTestProcessLogger> m_speedTestProcessLoggerlogger;
};
