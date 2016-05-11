#pragma once
//TODO #include <shared/plugin/IPlugin.h>
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
      class CSystem/*TODO : public shared::plugin::IPlugin*/
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         //--------------------------------------------------------------
         CSystem();

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CSystem();

         // IPlugin implementation
         virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> api);
         // [END] IPlugin implementation
      };
   }
} // namespace pluginSystem::internalPlugin


