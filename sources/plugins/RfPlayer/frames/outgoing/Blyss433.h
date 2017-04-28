#pragma once
#include "IOutgoingFrame.h"

namespace frames {
namespace outgoing {
   
   class CBlyss433 : public IOutgoingFrame
   {
      public:
         //--------------------------------------------------------------
         /// \brief              Constructor
         //--------------------------------------------------------------
         CBlyss433();

         //--------------------------------------------------------------
         /// \brief              Destructor
         //--------------------------------------------------------------
         virtual ~CBlyss433();
         
         // IOutgoingFrame implementation
         virtual const std::string generateAsciiCommand(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) override;
         // [END] - IOutgoingFrame implementation
   };
} //namespace incoming
} //namespace frames

