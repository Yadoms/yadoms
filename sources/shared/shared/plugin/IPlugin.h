#pragma once

#include "yPluginApi/IYPluginApi.h"


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
      /// \param [in] context Plugin execution context (Yadoms API)
      /// \note               Do the work. This function runs in specific-thread context (independent from Yadoms main-thread).
      //                      Implementation should loop until end is asked by Yadoms.
      //                      Be careful to put some sleeps to prevent using too much system resources.
      //                      Use boost::this_thread::sleep as a watch stop point (a point where thread is able to stop).
      //--------------------------------------------------------------
      virtual void doWork(boost::shared_ptr<yPluginApi::IYPluginApi> context) = 0;
   };

} } // namespace shared::plugin
