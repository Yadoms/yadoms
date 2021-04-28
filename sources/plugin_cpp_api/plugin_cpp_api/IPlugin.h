#pragma once

// Include directly CPP yPluginApi interface
#include <shared/plugin/yPluginApi/IYPluginApi.h>


namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \brief General plugin interface
   //--------------------------------------------------------------
   class IPlugin
   {
   public:
      virtual ~IPlugin() = default;

      //--------------------------------------------------------------
      /// \brief              Main plugin function
      /// \param [in] api     Plugin execution context (Yadoms API)
      /// \note               Do the work. This function runs in specific-thread context (independent from Yadoms main-thread).
      //                      Implementation should loop until end is asked by Yadoms.
      //                      Be careful to put some sleeps to prevent using too much system resources.
      //                      Use boost::this_thread::sleep as a watch stop point (a point where thread is able to stop).
      //--------------------------------------------------------------
      virtual void doWork(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api) = 0;
   };
} // namespace plugin_cpp_api
