#pragma once
#include <plugin_cpp_api/IPlugin.h>

namespace frames {
namespace outgoing {
   
   class IOutgoingFrame
   {
      public:
         //--------------------------------------------------------------
         /// \brief	               Virtual destructor
         //--------------------------------------------------------------
         virtual ~IOutgoingFrame() {}
         
         //--------------------------------------------------------------
         /// \brief	      Generate the ASCII command
         /// \return       The command as string
         //--------------------------------------------------------------
         virtual const std::string generateAsciiCommand(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) = 0;
   };

} //namespace incoming
} //namespace frames

