//
// System signals handler
//
#pragma once

#include <shared/event/EventHandler.hpp>
#include "IApplicationStopHandler.h"

//-----------------------------------------------------------------------------
/// \class              Application stop handler
//-----------------------------------------------------------------------------
class CApplicationStopHandler : public IApplicationStopHandler
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   /// \param[in] targetEventHandler   Event handler to notify
   /// \param[in] eventId              Event ID to send when stop occurs
   //-----------------------------------------------------------------------------
   CApplicationStopHandler(boost::shared_ptr<shared::event::CEventHandler> targetEventHandler, int eventId);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CApplicationStopHandler();

   //-----------------------------------------------------------------------------
   /// \brief		                     Returns the stop mode
   /// \return                         Stop mode (only significative after request to stop)
   //-----------------------------------------------------------------------------
   EStopMode stopMode() const;

   // IApplicationStopHandler implementation
   virtual void requestToStop(EStopMode stopMode);
   virtual void registerForAppEnds(boost::shared_ptr<shared::event::CEventHandler> & handler, const int code);
   // [END] IApplicationStopHandler implementation

   //-----------------------------------------------------------------------------
   /// \brief		                     Notify application is ending
   //-----------------------------------------------------------------------------
   void NotifyApplicationEnds();

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     Internal interruption handler
   /// \param[in] signal               Signal source of interruption
   //-----------------------------------------------------------------------------
   static void handleInternal(int signal);

   //-----------------------------------------------------------------------------
   /// \brief		                     The main m_thread method
   //-----------------------------------------------------------------------------
   void doWork();


private:
   //-----------------------------------------------------------------------------
   /// \brief		                     Event handler to notify
   //-----------------------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_targetEventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     Event ID to send when stop occurs
   //-----------------------------------------------------------------------------
   int m_eventId;

   //-----------------------------------------------------------------------------
   /// \brief		                     The requested stop mode
   //-----------------------------------------------------------------------------
   EStopMode m_stopMode;

   //-----------------------------------------------------------------------------
   /// \brief		                     The thread waiting for system signals
   //-----------------------------------------------------------------------------
   boost::shared_ptr<boost::thread> m_thread;

   //-----------------------------------------------------------------------------
   /// \brief		                     Handler to call when app is about to end
   //-----------------------------------------------------------------------------
   std::vector< std::pair< boost::shared_ptr<shared::event::CEventHandler>, int > > m_appendHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The flag used between interruption and thread
   //-----------------------------------------------------------------------------
   static volatile sig_atomic_t StopRequested;
};
