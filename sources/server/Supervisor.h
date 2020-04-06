#pragma once
#include <shared/event/EventHandler.hpp>
#include <Poco/Runnable.h>
#include "IPathProvider.h"

//-----------------------------------------------------------------------------
/// \class              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor : public Poco::Runnable
{
   //--------------------------------------------------------------
   /// \brief	Event IDs
   //--------------------------------------------------------------
   enum
   {
      kStopRequested = shared::event::kUserFirstId,   // Yadoms stop was required
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                        Constructor
   /// \param[in] pathProvider            The Yadoms paths provider
   //-----------------------------------------------------------------------------
   explicit CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor();

   //-----------------------------------------------------------------------------
   /// \brief		                     The main method (blocking, returns at Yadoms exit)
   //-----------------------------------------------------------------------------
   void run() override;

   //-----------------------------------------------------------------------------
   /// \brief  Ask the supervisor to stop
   //-----------------------------------------------------------------------------
   void requestToStop();

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     The supervisor event handler
   //-----------------------------------------------------------------------------
   shared::event::CEventHandler m_EventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The Yadoms paths provider
   //-----------------------------------------------------------------------------
   boost::shared_ptr<const IPathProvider> m_pathProvider;
};

