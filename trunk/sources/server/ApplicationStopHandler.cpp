#include "stdafx.h"
#include "ApplicationStopHandler.h"
#include <shared/Log.h>

volatile sig_atomic_t CApplicationStopHandler::StopRequested = false;


CApplicationStopHandler::CApplicationStopHandler(boost::shared_ptr<shared::event::CEventHandler> targetEventHandler, int eventId)
   :m_targetEventHandler(targetEventHandler), m_eventId(eventId), m_stopMode(kYadomsOnly)
{
   BOOST_ASSERT_MSG(!m_thread, "CApplicationStopHandler::configure must not be called twice");

   signal(SIGINT, handleInternal);   // CTRL+C signal
   signal(SIGTERM,handleInternal);   // Termination request

   // Start signal wait thread
   StopRequested = false;
   m_thread.reset(new boost::thread(boost::bind(&CApplicationStopHandler::doWork, this)));
}

CApplicationStopHandler::~CApplicationStopHandler()
{
   m_thread->join();
   m_thread.reset();
}

IApplicationStopHandler::EStopMode CApplicationStopHandler::stopMode() const
{
   return m_stopMode;
}

void CApplicationStopHandler::requestToStop(EStopMode stopMode) const
{
   //m_stopMode = stopMode;
   //StopRequested = true;
}

void CApplicationStopHandler::doWork()
{
   while (!StopRequested)
   {
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
   }

   m_targetEventHandler->postEvent(m_eventId);
}

void CApplicationStopHandler::handleInternal(int signal)
{
   switch(signal)
   {
   case  SIGINT :
   case  SIGTERM :
      StopRequested = true;
      break;
   default:
      YADOMS_LOG(warning) << "CApplicationStopHandler::handleInternal - no handler for #%d signal " << signal;
      break;
   }
}