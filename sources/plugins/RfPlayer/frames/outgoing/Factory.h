#pragma once
#include "IOutgoingFrame.h"

namespace frames {
namespace outgoing {
   
   class CFactory
   {
      public:
         static boost::shared_ptr<IOutgoingFrame> make(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command);
         
   };
} //namespace incoming
} //namespace frames

