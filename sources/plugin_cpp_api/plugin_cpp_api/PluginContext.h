#pragma once

// Include directly CPP yPluginApi interface //TODO conserver l'interface en commun avec shared ?
#include <shared/plugin/yPluginApi/IYPluginApi.h>


namespace plugin_cpp_api
{

   //--------------------------------------------------------------
   /// \class The plugin context
   //--------------------------------------------------------------
   class CPluginContext
   {
   public:
      CPluginContext(int argc, char **argv);

      virtual ~CPluginContext();

      //--------------------------------------------------------------
      /// \brief              Get the plugin execution context (Yadoms API)
      /// \return             Plugin execution context (Yadoms API)
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api() const;

   private:
      boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> m_api;
   };

} // namespace plugin_cpp_api
