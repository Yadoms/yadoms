#pragma once

#include "yadomsApi/IYadomsApi.h"


namespace shared { namespace plugin
{

   //--------------------------------------------------------------
   /// \class General plugin interface
   //--------------------------------------------------------------
   class IPlugin
   {
   public:
      virtual ~IPlugin() {}

      //--------------------------------------------------------------
      /// \brief              Main plugin function
      /// \param [in]         yadoms  Plugin execution context (Yadoms API)
      /// \note               Do the work. This function runs in specific-thread context (independent from Yadoms main-thread).
      //                      Implementation should loop until end is asked by Yadoms.
      //                      Be careful to put some sleeps to prevent using too much system resources.
      //                      Use boost::this_thread::sleep as a watch stop point (a point where thread is able to stop).
      //--------------------------------------------------------------
      virtual void doWork(boost::shared_ptr<shared::plugin::yadomsApi::IYadomsApi> yadoms) = 0;

      //--------------------------------------------------------------
      /// \brief              Notify the plugin that its configuration was changed
      /// \par configuration  Plugin instance configuration values from database (Json)
      /// \note               Note that this function is asynchronous. It's the plugin
      ///                     responsibility to manage this notification thread-safety
      //--------------------------------------------------------------
      virtual void updateConfiguration(const std::string& configuration) = 0;

   };

} } // namespace shared::plugin
