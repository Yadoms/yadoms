#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/versioning/SemVer.h>
#include <Poco/Runnable.h>
#include "IPathProvider.h"


//-----------------------------------------------------------------------------
/// \brief              Yadoms supervisor
//-----------------------------------------------------------------------------
class CSupervisor : public Poco::Runnable
{
   //--------------------------------------------------------------
   /// \brief	Event IDs
   //--------------------------------------------------------------
   enum
   {
      kStopRequested = shared::event::kUserFirstId,
      // Yadoms stop was required
   };

public:
   //-----------------------------------------------------------------------------
   /// \brief		                        Constructor
   /// \param[in] pathProvider            The Yadoms paths provider
   /// \param[in] yadomsVersion           The Yadoms version
   //-----------------------------------------------------------------------------
   explicit CSupervisor(boost::shared_ptr<const IPathProvider> pathProvider,
                        const shared::versioning::CSemVer& yadomsVersion);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CSupervisor() = default;

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
   shared::event::CEventHandler m_eventHandler;

   //-----------------------------------------------------------------------------
   /// \brief		                     The Yadoms paths provider
   //-----------------------------------------------------------------------------
   boost::shared_ptr<const IPathProvider> m_pathProvider;

   const shared::versioning::CSemVer m_yadomsVersion;
};
