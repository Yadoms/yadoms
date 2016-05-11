#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem
{
   namespace internalPlugin
   {
      //--------------------------------------------------------------
      /// \brief	This class is the internal system plugin entry point
      //--------------------------------------------------------------
      class CSystem
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         //--------------------------------------------------------------
         CSystem();

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CSystem();

         //--------------------------------------------------------------
         /// \brief	            Main job
         /// \param[in] api      The plugin API used to dial with Yadoms
         //--------------------------------------------------------------
         virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> api);
      };
   }
} // namespace pluginSystem::internalPlugin


