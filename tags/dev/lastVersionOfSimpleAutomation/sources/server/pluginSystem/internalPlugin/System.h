#pragma once
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include "IApplicationStopHandler.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem { namespace internalPlugin {

      //--------------------------------------------------------------
      /// \brief	This class is the internal system plugin entry point
      //--------------------------------------------------------------
      class CSystem : public shared::plugin::IPlugin
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param [in]   applicationStopHandler  The object used to request application stop
         //--------------------------------------------------------------
         CSystem(IApplicationStopHandler& applicationStopHandler);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CSystem();

         // IPlugin implementation
         virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
         // [END] IPlugin implementation

      private:
         //--------------------------------------------------------------
         /// \brief			The object used to request application stop
         //--------------------------------------------------------------
         IApplicationStopHandler& m_applicationStopHandler;
      };


} } // namespace pluginSystem::internalPlugin


