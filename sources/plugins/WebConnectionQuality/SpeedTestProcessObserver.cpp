#include "stdafx.h"
#include "SpeedTestProcessObserver.h"
#include "SpeedTestEventData.h"
#include <utility>


CSpeedTestProcessObserver::CSpeedTestProcessObserver(shared::event::CEventHandler& pluginEventHandler,
                                                     int eventId,
                                                     boost::shared_ptr<CSpeedTestProcessLogger> speedTestProcessLogger)
   : m_pluginEventHandler(pluginEventHandler),
     m_eventId(eventId),
     m_speedTestProcessLogger(std::move(speedTestProcessLogger))
{
}

void CSpeedTestProcessObserver::onStart()
{
}

void CSpeedTestProcessObserver::onFinish(int returnCode,
                                         const std::string& error)
{
   m_pluginEventHandler.postEvent(m_eventId,
                                  boost::make_shared<CSpeedTestEventData>(returnCode,
                                                                          error,
                                                                          m_speedTestProcessLogger->stdoutContent()));
}
