#pragma once

namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \class The plugin context
   //--------------------------------------------------------------
   class IPluginContext
   {
   public:
      enum EProcessReturnCode
      {
         kOk = 0,
         kStartError,         // Plugin failed to start
         kRuntimeError,       // Plugin crashed at runtime
         kUnexpectedStop      // Plugin has stopped without stop requested
      };

      virtual ~IPluginContext() { }

      //--------------------------------------------------------------
      /// \brief                 The main run method, where the plugin code is executed
      //--------------------------------------------------------------
      virtual void run() = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the return code, after run() returned
      /// \return                The return code
      //--------------------------------------------------------------
      virtual EProcessReturnCode getReturnCode() const = 0;
   };

} // namespace plugin_cpp_api
