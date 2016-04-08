#pragma once
#include "IPlugin.h"


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
      enum EReturnCode
      {
         kOk = 0,
         kRuntimeError,
         kUnexpectedStop
      };
      virtual EReturnCode getReturnCode() const = 0;
   };

   boost::shared_ptr<IPluginContext> CreatePluginContext(int argc, char **argv, boost::shared_ptr<IPlugin> plugin);

} // namespace plugin_cpp_api
