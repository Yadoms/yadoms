#include "stdafx.h"
#include "SpeedTestProcessObserver.h"
#include "SpeedTestEventData.h"


CSpeedTestProcessObserver::CSpeedTestProcessObserver(shared::event::CEventHandler& pluginEventHandler,
                                                     int eventId)
   : m_pluginEventHandler(pluginEventHandler),
     m_eventId(eventId)
{
}

CSpeedTestProcessObserver::~CSpeedTestProcessObserver()
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
                                                                          error));
}
