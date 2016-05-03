#pragma once
#include "PluginMain.h"


namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \class The plugin context
   //--------------------------------------------------------------
   class IPluginContext
   {
   public:
      virtual ~IPluginContext() { }

      //--------------------------------------------------------------
      /// \brief                 The main run method, where the plugin code is executed
      //--------------------------------------------------------------
      virtual void run() = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the return code, after run() returned
      /// \return                The return code
      //--------------------------------------------------------------
      virtual EReturnCode getReturnCode() const = 0;
   };

} // namespace plugin_cpp_api
